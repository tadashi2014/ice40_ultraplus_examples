from __future__ import annotations

from dataclasses import dataclass, field
from enum import IntEnum
from pathlib import Path

from .defs import (
    CONSTANT_DATA,
    DATA_STORE,
    ERR_ADDRESS_RANGE_OVERFLOW,
    ERR_PARSE_OPERAND_HEXDECIMAL_OVERFLOW,
    ERR_PARSE_OPERAND_ILLEGAL_INDEX_REGISTER,
    ERR_PARSE_OPERAND_INVALID_OPERAND,
    ERR_PARSE_OPERAND_TOO_MANY_ARGS,
    ERR_PARSE_OPCODE_NOT_EXIST,
    ERR_PARSE_OPCODE_UNDEFINED,
    ERR_PROC_DC_HEXDECIMAL_OVERFLOW,
    ERR_PROC_DC_INVALID_ARG,
    ERR_PROC_DC_NEED_ARG,
    ERR_PROC_DC_NULL_ARG,
    ERR_PROC_DS_INVALID_ARG,
    ERR_PROC_DS_NEED_ARG,
    ERR_PROC_DS_NOT_ALLOW_NEGATIVE_SIZE,
    ERR_PROC_END_CANNOT_HAVE_LABEL,
    ERR_PROC_LABEL_INVALID_CHAR,
    ERR_PROC_LABEL_RSRVD_WORD,
    ERR_PROC_LABEL_TOO_LONG,
    ERR_PROC_MACHINE_INVALID_OPERAND_TYPE,
    ERR_PROC_MACRO_EXPANDED_LINE_TOO_LONG,
    ERR_PROC_MACRO_MISMATCH_ARG_COUNT,
    ERR_PROC_START_INVALID_START_ADDRESS,
    ERR_PROC_START_NO_START_LABEL,
    ERR_PROC_START_NOT_ALLOW_START_ADDRESS,
    ERR_PROC_START_START_ADDRESS_RANGE_OVER,
    ERR_SYMBOL_MULTI_DEF_LABEL,
    ERR_SYMBOL_UNDEFINED,
    MAX_LINE_LENGTH,
    MAX_LABEL_LENGTH,
    MODULE_END,
    MODULE_ENTRY,
    OPR_ADR,
    OPR_ADRADR,
    OPR_ADRX,
    OPR_NONE,
    OPR_R,
    OPR_R1R2,
    OPR_RADR,
    OPR_RADRX,
    UNDEFINED,
)
from .instruction_registry import (
    load_macros_from_file,
    load_mnemonics_from_file,
    register_default_instructions,
    register_default_macros,
    register_default_mnemonics,
)
from .parse_options import ParseOptions
from .symbol_table import InstrClass, SymbolTable
from .token import Token

OPR_TYPE = [
    OPR_NONE,
    OPR_R,
    OPR_R1R2,
    -1,
    -1,
    OPR_RADR,
    OPR_RADRX,
    -1,
    OPR_ADR,
    OPR_ADRX,
    -1,
    -1,
    OPR_ADRADR,
    -1,
    -1,
]


@dataclass(slots=True)
class Address:
    value: int = UNDEFINED
    label: str | None = None

    def is_set(self) -> bool:
        return self.value != UNDEFINED or self.label is not None


@dataclass(slots=True)
class Operand:
    op_type: int = OPR_NONE
    count: int = 0
    registers_field: int = 0
    has_adr: bool = False
    adr: Address = field(default_factory=Address)
    has_ext_adr: bool = False
    ext_adr: Address = field(default_factory=Address)


class CodeType(IntEnum):
    BLANK = 0
    MACHINE = 1
    START = 2
    END = 3
    DC = 4
    DS = 5


class LabelType(IntEnum):
    NORMAL = 0
    LITERAL = 1


@dataclass(slots=True)
class ObjectCode:
    line_number: int = 0
    src_line: str = ""
    passed: int = 0
    scope: int = 0
    code_address: int = 0
    code_type: CodeType = CodeType.BLANK
    value: int = 0
    label: str | None = None
    label_typ: LabelType = LabelType.NORMAL
    tokens: list[Token] = field(default_factory=list)
    address_const: Address = field(default_factory=Address)
    ext_address_const: Address = field(default_factory=Address)
    length: int = 0
    data_words: list[int] = field(default_factory=list)
    asm_error_col: int = 0
    asm_error_code: int = 0
    suppress_print: bool = False
    print_right_arrow: bool = False


@dataclass(slots=True)
class LiteralEntry:
    scope: int
    literal_str: str
    ref_line: int


@dataclass(slots=True)
class XrefEntry:
    name: str
    address: int
    is_xdef: bool
    def_line: int
    refs: list[int] = field(default_factory=list)


class Assembler:
    def __init__(self, opts: ParseOptions) -> None:
        self.opts = opts
        self.sym = SymbolTable()
        if opts.user_mnemonic_file and opts.user_macro_file:
            load_mnemonics_from_file(self.sym, opts.user_mnemonic_file)
            self.macro_table = load_macros_from_file(self.sym, opts.user_macro_file)
        elif opts.user_mnemonic_file:
            load_mnemonics_from_file(self.sym, opts.user_mnemonic_file)
            self.macro_table = register_default_macros(self.sym)
        elif opts.user_macro_file:
            register_default_instructions(self.sym)
            self.macro_table = load_macros_from_file(self.sym, opts.user_macro_file)
        else:
            self.macro_table = register_default_mnemonics(self.sym)
        self.literals: list[LiteralEntry] = []
        self.lit_base = 0
        self.pending_output: list[str] = []
        self.xref_table: list[XrefEntry] = []

    def assemble_file(self, src: str, code_offset: int, out_file: str | None, append: bool) -> int:
        start_addr = self.opts.effective_start_address() if code_offset < 0 else code_offset
        max_addr = self.opts.effective_max_code_address()
        print("\n**************************************************", file=__import__("sys").stderr)
        print(f"{src}のアセンブル処理を開始します。", file=__import__("sys").stderr)

        module_name = Path(src).stem
        path_no_ext = str(Path(src).with_suffix(""))
        code_size = 0
        start_address = Address()
        total_errors = 0
        final_address = start_addr

        for passed in range(2):
            if passed == 0:
                self.literals.clear()
                self.xref_table.clear()
            self.lit_base = 0
            code_address = start_addr
            scope = 0
            started = False
            ended = False
            dirty = False
            range_over = False
            obj_fp = None
            lst_fp = None
            if passed == 1:
                obj_path = out_file or f"{path_no_ext}.obj"
                mode = "a" if append else "w"
                obj_fp = open(obj_path, mode, encoding="utf-8")
                if self.opts.generate_asm_list:
                    lst_fp = open(f"{path_no_ext}.lst", "w", encoding="utf-8")
                if append:
                    obj_fp.write("\n")
                    if lst_fp:
                        lst_fp.write("\n")
                self.dump_header(obj_fp, module_name, code_size, start_address)
                if lst_fp:
                    self.dump_header(lst_fp, path_no_ext, code_size, start_address)

            try:
                with open(src, "r", encoding="utf-8") as src_fp:
                    line_number = 1
                    for raw_line in src_fp:
                        line = raw_line.rstrip("\n\r")
                        if len(line) > MAX_LINE_LENGTH:
                            total_errors += 1
                            print(
                                f" Line {line_number}: line too long (max {MAX_LINE_LENGTH} chars)",
                                file=__import__("sys").stderr,
                            )
                            line = line[:MAX_LINE_LENGTH]
                        oc = ObjectCode(
                            line_number=line_number,
                            src_line=line,
                            passed=passed,
                            scope=scope,
                            code_address=code_address,
                        )
                        line_number += 1
                        ok = self.asm_line(oc, obj_fp, lst_fp)
                        if ok:
                            if oc.code_type in (CodeType.MACHINE, CodeType.DC, CodeType.DS):
                                code_address += oc.length
                                dirty = True
                                ended = False
                            elif oc.code_type == CodeType.START and not dirty:
                                start_address = oc.address_const if oc.address_const.is_set() else Address(code_address)
                                started = True
                            elif oc.code_type == CodeType.END:
                                code_address = oc.code_address
                                ended = True
                                scope += 1
                                if passed == 0:
                                    code_size = code_address - start_addr
                        else:
                            total_errors += 1
                            self.display_asm_error(oc)
                        if code_address > max_addr:
                            range_over = True
                            break
            finally:
                if obj_fp:
                    obj_fp.close()
                if lst_fp:
                    lst_fp.close()

            if not started:
                print(" START命令がありません。", file=__import__("sys").stderr)
                total_errors += 1
            if range_over:
                print(" プログラムアドレスが上限値を超えました。", file=__import__("sys").stderr)
                total_errors += 1
            elif not ended:
                print(" END命令がありません。", file=__import__("sys").stderr)
                total_errors += 1
            if total_errors > 0:
                break
            if passed == 1:
                if self.opts.generate_cross_ref:
                    with open(out_file or f"{path_no_ext}.obj", "a", encoding="utf-8") as fp:
                        self.dump_symbol_table(fp)
                    if self.opts.generate_asm_list:
                        with open(f"{path_no_ext}.lst", "a", encoding="utf-8") as fp:
                            self.dump_xref_table(fp)
                final_address = code_address

        self.sym.clear_labels()
        self.literals.clear()
        self.lit_base = 0
        if total_errors == 0:
            print(f"\n{src}のアセンブル処理を終了しました。", file=__import__("sys").stderr)
            print("アセンブルエラーはありませんでした。", file=__import__("sys").stderr)
            print("**************************************************\n", file=__import__("sys").stderr)
            return final_address
        print(f"\n{src}のアセンブル処理を終了しました。", file=__import__("sys").stderr)
        print(f"{total_errors}個のアセンブルエラーがありました。", file=__import__("sys").stderr)
        print("**************************************************\n", file=__import__("sys").stderr)
        raise RuntimeError(f"{total_errors} errors in {src}")

    def asm_line(self, oc: ObjectCode, fp_obj, fp_lst) -> bool:
        self.pending_output.clear()
        ok = self.parse(oc)
        pending = list(self.pending_output)
        self.pending_output.clear()
        for line in pending:
            if fp_obj:
                fp_obj.write(line)
            if fp_lst:
                fp_lst.write(line)
        if ok and not oc.suppress_print:
            self.dump_object(oc, fp_obj, fp_lst)
        return ok

    def asm_single(self, oc: ObjectCode) -> bool:
        ok = self.parse(oc)
        if ok and not oc.suppress_print and oc.passed == 1:
            self.pending_output.append(self.format_object_line(oc))
        return ok

    def parse(self, oc: ObjectCode) -> bool:
        oc.tokens = tokenize(oc.src_line)
        if not oc.tokens or oc.tokens[0].is_comment():
            oc.code_type = CodeType.BLANK
            return True
        tok = oc.tokens[0]
        oc.asm_error_col = tok.offset
        tok_idx = 0
        if tok.offset == 0:
            if not self.proc_label(tok, oc):
                return False
            oc.label = tok.text
            oc.asm_error_col = tok.offset + len(tok.text)
            tok_idx += 1
        if tok_idx >= len(oc.tokens) or oc.tokens[tok_idx].is_comment():
            oc.asm_error_code = ERR_PARSE_OPCODE_NOT_EXIST
            return False
        opcode_tok = oc.tokens[tok_idx]
        name = opcode_tok.text.upper() if self.opts.allow_lower_case else opcode_tok.text
        entry = self.sym.search_instr(name)
        if entry is None:
            oc.asm_error_col = opcode_tok.offset
            oc.asm_error_code = ERR_PARSE_OPCODE_UNDEFINED
            return False
        oc.value = entry.value
        if entry.klass == InstrClass.MACHINE:
            return self.proc_machine(oc, tok_idx)
        if entry.klass == InstrClass.PSEUDO:
            return self.proc_pseudo(oc, tok_idx)
        if entry.klass == InstrClass.MACRO:
            return self.proc_macro(oc, tok_idx)
        return False

    def proc_label(self, tok: Token, oc: ObjectCode) -> bool:
        if oc.passed == 1:
            return True
        oc.asm_error_col = tok.offset
        if not tok.is_label(self.opts.allow_lower_case) and oc.label_typ != LabelType.LITERAL:
            oc.asm_error_code = ERR_PROC_LABEL_INVALID_CHAR
            return False
        if len(tok.text) > MAX_LABEL_LENGTH:
            oc.asm_error_code = ERR_PROC_LABEL_TOO_LONG
            return False
        if tok.is_reserved_word(self.opts.allow_lower_case):
            oc.asm_error_code = ERR_PROC_LABEL_RSRVD_WORD
            return False
        label_name = tok.text.upper() if self.opts.allow_lower_case else tok.text
        if self.do_label_def(label_name, oc.scope, oc.code_address) < 0:
            oc.asm_error_code = ERR_SYMBOL_MULTI_DEF_LABEL
            return False
        self.xref_define(label_name, oc.code_address, oc.line_number, False)
        return True

    def do_label_def(self, label: str, scope: int, address: int) -> int:
        try:
            self.sym.define_label(label, scope, address)
            return 0
        except ValueError:
            return ERR_SYMBOL_MULTI_DEF_LABEL

    def do_label_xdef(self, label: str, scope: int, address: int) -> int:
        try:
            current = self.sym.resolve_label(label, scope)
            if current is None:
                self.sym.define_label(label, scope, address)
            elif current != address:
                return ERR_SYMBOL_MULTI_DEF_LABEL
            self.sym.define_xdef_label(label, address)
            return 0
        except ValueError:
            return ERR_SYMBOL_MULTI_DEF_LABEL

    def do_label_ref(self, label: str, scope: int) -> int:
        value = self.sym.resolve_label(label, scope)
        return UNDEFINED if value is None else value

    def proc_machine(self, oc: ObjectCode, tok_idx: int) -> bool:
        oc.code_type = CodeType.MACHINE
        value = oc.value
        opcode_tok = oc.tokens[tok_idx]
        oc.asm_error_col = opcode_tok.offset + len(opcode_tok.text)
        operand_tok = oc.tokens[tok_idx + 1] if tok_idx + 1 < len(oc.tokens) else None
        op = Operand()
        if not self.parse_operand(operand_tok, op, oc):
            return False
        if op.count == 0 and op.op_type != OPR_NONE:
            oc.asm_error_code = ERR_PROC_MACHINE_INVALID_OPERAND_TYPE
            return False
        max_oprs = value & 0x03
        if op.count > max_oprs:
            oc.asm_error_code = ERR_PARSE_OPERAND_TOO_MANY_ARGS
            return False
        available = value & 0xFC
        if (available ^ op.op_type) == 0 or (available & op.op_type) != 0:
            oc.value = (value & 0xFF00) | op.registers_field
            oc.address_const = op.adr
            oc.ext_address_const = op.ext_adr
            if op.has_ext_adr:
                oc.length = 3
            elif op.has_adr:
                oc.length = 2
            else:
                oc.length = 1
                v = oc.value & 0xFFFF
                if 0x0000 < v < 0x7000:
                    oc.value |= 0x0400
            return True
        oc.asm_error_code = ERR_PROC_MACHINE_INVALID_OPERAND_TYPE
        return False

    def proc_pseudo(self, oc: ObjectCode, tok_idx: int) -> bool:
        dispatch = oc.value & 0xFF00
        if dispatch == MODULE_ENTRY:
            return self.proc_start(oc, tok_idx)
        if dispatch == MODULE_END:
            return self.proc_end(oc)
        if dispatch == DATA_STORE:
            return self.proc_ds(oc, tok_idx)
        if dispatch == CONSTANT_DATA:
            return self.proc_dc(oc, tok_idx)
        oc.asm_error_code = ERR_PARSE_OPCODE_UNDEFINED
        return False

    def proc_start(self, oc: ObjectCode, tok_idx: int) -> bool:
        oc.code_type = CodeType.START
        if oc.label is None:
            oc.asm_error_col = 0
            oc.asm_error_code = ERR_PROC_START_NO_START_LABEL
            return False
        if oc.passed == 0:
            label = oc.label.upper() if self.opts.allow_lower_case else oc.label
            result = self.do_label_xdef(label, oc.scope, oc.code_address)
            if result < 0:
                oc.asm_error_code = result
                return False
            self.xref_define(label, oc.code_address, oc.line_number, True)
            if tok_idx + 1 < len(oc.tokens) and not oc.tokens[tok_idx + 1].is_comment():
                tok = oc.tokens[tok_idx + 1]
                if tok.is_decimal():
                    oc.address_const.value = tok.decimal_to_integer() & 0xFFFF
                elif tok.is_hex_decimal():
                    value = tok.hex_decimal_to_integer()
                    if value > 0xFFFF:
                        oc.asm_error_code = ERR_PROC_START_START_ADDRESS_RANGE_OVER
                        return False
                    oc.address_const.value = value
                elif tok.is_label(self.opts.allow_lower_case):
                    oc.address_const.label = tok.text
                else:
                    oc.asm_error_code = ERR_PROC_START_INVALID_START_ADDRESS
                    return False
            else:
                oc.address_const.value = oc.code_address
        return True

    def proc_end(self, oc: ObjectCode) -> bool:
        oc.code_type = CodeType.END
        if oc.label is not None:
            oc.asm_error_col = 0
            oc.asm_error_code = ERR_PROC_END_CANNOT_HAVE_LABEL
            return False
        oc.code_address += self.do_literal_def(oc)
        return True

    def proc_ds(self, oc: ObjectCode, tok_idx: int) -> bool:
        oc.code_type = CodeType.DS
        if tok_idx + 1 >= len(oc.tokens) or oc.tokens[tok_idx + 1].is_comment():
            oc.asm_error_code = ERR_PROC_DS_NEED_ARG
            return False
        size_tok = oc.tokens[tok_idx + 1]
        oc.asm_error_col = size_tok.offset
        if not size_tok.is_decimal():
            oc.asm_error_code = ERR_PROC_DS_INVALID_ARG
            return False
        num = int(size_tok.text, 10)
        if num < 0:
            oc.asm_error_code = ERR_PROC_DS_NOT_ALLOW_NEGATIVE_SIZE
            return False
        if oc.code_address + num > 0xFFFF:
            oc.asm_error_code = ERR_ADDRESS_RANGE_OVERFLOW
            return False
        oc.length = num
        return True

    def proc_dc(self, oc: ObjectCode, tok_idx: int) -> bool:
        oc.code_type = CodeType.DC
        if tok_idx + 1 >= len(oc.tokens) or oc.tokens[tok_idx + 1].is_comment():
            oc.asm_error_code = ERR_PROC_DC_NEED_ARG
            return False
        dc_tok = oc.tokens[tok_idx + 1]
        operands = tokenize_operand(dc_tok.text, 64)
        if len(operands) > 1:
            if oc.passed == 1:
                blank = ObjectCode(
                    line_number=oc.line_number,
                    src_line=oc.src_line,
                    passed=oc.passed,
                    scope=oc.scope,
                    code_address=oc.code_address,
                    code_type=CodeType.BLANK,
                    value=oc.value,
                    label=oc.label,
                    label_typ=oc.label_typ,
                    tokens=list(oc.tokens),
                    address_const=oc.address_const,
                    ext_address_const=oc.ext_address_const,
                    length=oc.length,
                    data_words=list(oc.data_words),
                    asm_error_col=oc.asm_error_col,
                    asm_error_code=oc.asm_error_code,
                    suppress_print=False,
                    print_right_arrow=oc.print_right_arrow,
                )
                self.pending_output.append(self.format_object_line(blank))
            total_len = 0
            base_addr = oc.code_address
            for op in operands:
                if op.text == "":
                    oc.asm_error_code = ERR_PROC_DC_NULL_ARG
                    return False
                sub = ObjectCode(src_line=f" DC {op.text}", line_number=UNDEFINED, passed=oc.passed, scope=oc.scope, code_address=base_addr + total_len)
                self.asm_single(sub)
                if sub.asm_error_code != 0:
                    oc.asm_error_code = ERR_PROC_DC_INVALID_ARG
                    return False
                total_len += sub.length
            oc.suppress_print = True
            oc.length = total_len
            return True
        if not operands or operands[0].text == "":
            oc.asm_error_code = ERR_PROC_DC_NULL_ARG
            return False
        data_tok = operands[0]
        if data_tok.is_string(self.opts.allow_null_string):
            length = data_tok.get_casl2_string_length(self.opts.c_style_string, self.opts.p_style_string)
            if length == 0:
                oc.code_type = CodeType.BLANK
                return True
            oc.length = length
            if oc.passed == 1:
                oc.data_words = data_tok.gen_casl2_string_code(self.opts.c_style_string, self.opts.p_style_string)
            return True
        oc.length = 1
        if data_tok.is_decimal():
            if oc.passed == 1:
                oc.data_words = [int(data_tok.text, 10) & 0xFFFF]
            return True
        if data_tok.is_hex_decimal():
            value = data_tok.hex_decimal_to_integer()
            if value > 0xFFFF:
                oc.asm_error_col = dc_tok.offset + data_tok.offset + 5
                oc.asm_error_code = ERR_PROC_DC_HEXDECIMAL_OVERFLOW
                return False
            if oc.passed == 1:
                oc.data_words = [value]
            return True
        if data_tok.is_label(self.opts.allow_lower_case):
            label_name = data_tok.text.upper() if self.opts.allow_lower_case else data_tok.text
            if oc.passed == 0:
                self.xref_reference(label_name, oc.line_number)
            addr = UNDEFINED if oc.passed == 0 else self.do_label_ref(label_name, oc.scope)
            oc.address_const = Address(addr, label_name)
            if oc.passed == 1:
                oc.data_words = [addr & 0xFFFF]
            return True
        oc.asm_error_col = dc_tok.offset + data_tok.offset
        oc.asm_error_code = ERR_PROC_DC_INVALID_ARG
        return False

    def proc_macro(self, oc: ObjectCode, tok_idx: int) -> bool:
        macro_entry = oc.value >> 8
        argc = oc.value & 0xFF
        macro_tok = oc.tokens[tok_idx]
        oc.asm_error_col = macro_tok.offset + len(macro_tok.text)
        args: list[str] = []
        if tok_idx + 1 < len(oc.tokens) and not oc.tokens[tok_idx + 1].is_comment():
            args_tok = oc.tokens[tok_idx + 1]
            oc.asm_error_col = args_tok.offset
            args = [token.text for token in tokenize_operand(args_tok.text, 64)]
        if len(args) != argc:
            oc.asm_error_code = ERR_PROC_MACRO_MISMATCH_ARG_COUNT
            return False
        if oc.passed == 0:
            for arg in args:
                name = arg.upper() if self.opts.allow_lower_case else arg
                dummy = Token(0, name)
                if dummy.is_label(False) and not dummy.is_reserved_word(False):
                    self.xref_reference(name, oc.line_number)
        if macro_entry >= len(self.macro_table):
            oc.asm_error_code = ERR_PARSE_OPCODE_UNDEFINED
            return False
        if oc.passed == 1:
            blank = ObjectCode(
                line_number=oc.line_number,
                src_line=oc.src_line,
                passed=oc.passed,
                scope=oc.scope,
                code_address=oc.code_address,
                code_type=CodeType.BLANK,
                value=oc.value,
                label=oc.label,
                label_typ=oc.label_typ,
                tokens=list(oc.tokens),
                address_const=oc.address_const,
                ext_address_const=oc.ext_address_const,
                length=oc.length,
                data_words=list(oc.data_words),
                asm_error_col=oc.asm_error_col,
                asm_error_code=oc.asm_error_code,
                suppress_print=False,
                print_right_arrow=oc.print_right_arrow,
            )
            self.pending_output.append(self.format_object_line(blank))
        total_length = 0
        base_addr = oc.code_address
        arg_idx = 0
        for template in self.macro_table[macro_entry]:
            expanded = f" {template}"
            while "%%" in expanded and arg_idx < len(args):
                arg = args[arg_idx]
                arg_idx += 1
                expanded = expanded.replace("%%", arg, 1)
                if len(expanded) > MAX_LINE_LENGTH:
                    oc.asm_error_code = ERR_PROC_MACRO_EXPANDED_LINE_TOO_LONG
                    return False
            sub = ObjectCode(src_line=expanded, line_number=UNDEFINED, passed=oc.passed, scope=oc.scope, code_address=base_addr + total_length, print_right_arrow=True)
            self.asm_single(sub)
            if sub.asm_error_code != 0:
                oc.asm_error_code = sub.asm_error_code
                return False
            total_length += sub.length
        oc.length = total_length
        oc.code_address = base_addr
        oc.code_type = CodeType.MACHINE
        oc.suppress_print = True
        return True

    def do_literal_ref(self, lit_tok: Token, scope: int, passed: int, ref_line: int) -> str:
        lit_str = lit_tok.literal_value().text
        if not self.opts.no_optimize:
            for entry in self.literals:
                if entry.scope == scope and entry.literal_str == lit_str:
                    return f"$LT{entry.ref_line:05d}"
        if passed == 0:
            self.literals.append(LiteralEntry(scope, lit_str, ref_line))
        return f"$LT{ref_line:05d}"

    def do_literal_def(self, oc: ObjectCode) -> int:
        code_len = 0
        idx = self.lit_base
        while idx < len(self.literals) and self.literals[idx].scope == oc.scope:
            entry = self.literals[idx]
            lit_oc = ObjectCode(
                src_line=f"$LT{entry.ref_line:05d} DC {entry.literal_str}",
                line_number=UNDEFINED,
                passed=oc.passed,
                scope=oc.scope,
                code_address=oc.code_address + code_len,
                label_typ=LabelType.LITERAL,
            )
            self.asm_single(lit_oc)
            code_len += lit_oc.length
            idx += 1
        self.lit_base = idx
        return code_len

    def parse_operand(self, opr_token: Token | None, op: Operand, oc: ObjectCode) -> bool:
        op.op_type = OPR_NONE
        op.adr = Address()
        op.ext_adr = Address()
        if opr_token is None or opr_token.is_comment():
            return True
        oc.asm_error_col = opr_token.offset
        operands = tokenize_operand(opr_token.text, 4)
        if len(operands) > 3:
            fourth = operands[3]
            oc.asm_error_col += fourth.offset + len(fourth.text)
            oc.asm_error_code = ERR_PARSE_OPERAND_TOO_MANY_ARGS
            return False
        op.count = len(operands)
        sel = 0
        coef = 8
        for token in operands:
            if token.is_comment():
                op.count -= 1
                break
            if token.is_gr(self.opts.allow_lower_case):
                sel += 1
            elif token.is_decimal() or token.is_hex_decimal() or token.is_label(self.opts.allow_lower_case) or token.is_literal(self.opts.allow_null_string):
                sel += coef
            else:
                oc.asm_error_col += token.offset if not (not token.is_string(self.opts.allow_null_string) and ";" in token.text) else token.text.index(";") + token.offset
                oc.asm_error_code = ERR_PARSE_OPERAND_INVALID_OPERAND
                return False
            coef //= 2
        if sel >= len(OPR_TYPE) or OPR_TYPE[sel] < 0:
            oc.asm_error_code = ERR_PARSE_OPERAND_INVALID_OPERAND
            return False
        op.op_type = OPR_TYPE[sel]
        rev_ops = list(reversed(operands))
        def pop_token() -> Token:
            return rev_ops.pop(0)
        if op.op_type in (OPR_ADRX, OPR_RADRX):
            idx_tok = pop_token()
            gr = idx_tok.get_gr_num(self.opts.allow_lower_case)
            if gr in (None, 0):
                oc.asm_error_col += idx_tok.offset
                oc.asm_error_code = ERR_PARSE_OPERAND_ILLEGAL_INDEX_REGISTER
                return False
            op.registers_field = gr
            op.has_adr = True
            if not self.retrieve_adr(pop_token(), op.adr, oc):
                return False
            if op.op_type == OPR_RADRX:
                r_tok = pop_token()
                op.registers_field |= (r_tok.get_gr_num(self.opts.allow_lower_case) or 0) << 4
        elif op.op_type in (OPR_ADR, OPR_RADR):
            op.has_adr = True
            if not self.retrieve_adr(pop_token(), op.adr, oc):
                return False
            if op.op_type == OPR_RADR:
                r_tok = pop_token()
                op.registers_field = (r_tok.get_gr_num(self.opts.allow_lower_case) or 0) << 4
        elif op.op_type == OPR_R:
            op.registers_field = (pop_token().get_gr_num(self.opts.allow_lower_case) or 0) << 4
        elif op.op_type == OPR_R1R2:
            op.registers_field = pop_token().get_gr_num(self.opts.allow_lower_case) or 0
            op.registers_field |= (pop_token().get_gr_num(self.opts.allow_lower_case) or 0) << 4
        elif op.op_type == OPR_ADRADR:
            op.has_ext_adr = True
            if not self.retrieve_adr(pop_token(), op.ext_adr, oc):
                return False
            op.has_adr = True
            if not self.retrieve_adr(pop_token(), op.adr, oc):
                return False
        return True

    def retrieve_adr(self, tok: Token, adr: Address, oc: ObjectCode) -> bool:
        if tok.is_label(self.opts.allow_lower_case):
            name = tok.text.upper() if self.opts.allow_lower_case else tok.text
            if oc.passed == 0:
                self.xref_reference(name, oc.line_number)
            adr.label = name
            adr.value = UNDEFINED if oc.passed == 0 else self.do_label_ref(name, oc.scope)
            return True
        if tok.is_hex_decimal():
            value = tok.hex_decimal_to_integer()
            if value > 0xFFFF:
                oc.asm_error_col += tok.offset + 5
                oc.asm_error_code = ERR_PARSE_OPERAND_HEXDECIMAL_OVERFLOW
                return False
            adr.value = value
            return True
        if tok.is_decimal():
            adr.value = int(tok.text, 10) & 0xFFFF
            return True
        if tok.is_literal(self.opts.allow_null_string):
            label = self.do_literal_ref(tok, oc.scope, oc.passed, oc.line_number)
            adr.label = label
            adr.value = UNDEFINED if oc.passed == 0 else self.do_label_ref(label, oc.scope)
            return True
        oc.asm_error_col += tok.offset
        oc.asm_error_code = ERR_PARSE_OPERAND_INVALID_OPERAND
        return False

    def dump_header(self, fp, name: str, code_size: int, start: Address) -> None:
        fp.write(f"NAME:{name}\n")
        fp.write(f"SIZE:{code_size & 0xFFFF:04X}\n")
        start_val = start.value
        if start.label is not None:
            resolved = self.sym.resolve_label(start.label, 0)
            if resolved is not None:
                start_val = resolved
        if start_val == UNDEFINED:
            fp.write(f"START:{start.label or ''}R\n")
        else:
            fp.write(f"START:{start_val & 0xFFFF:04X}R\n")

    def dump_symbol_table(self, fp) -> None:
        fp.write("\n_XREF_:\n")
        for entry in self.xref_table:
            if entry.def_line == UNDEFINED and not entry.name.startswith("$"):
                fp.write(f"{entry.name}\n")
        fp.write("_XDEF_:\n")
        for entry in self.xref_table:
            if entry.is_xdef:
                fp.write(f"{entry.name}:{entry.address & 0xFFFF:04X}R\n")
        fp.write("_DEF_:\n")
        for entry in self.xref_table:
            if entry.def_line != UNDEFINED and not entry.is_xdef:
                fp.write(f"{entry.name}:{entry.address & 0xFFFF:04X}R\n")

    def dump_xref_table(self, fp) -> None:
        fp.write("\n\n クロスリファレンス")
        fp.write("\n ==============================================")
        fp.write("\n    ラベル   |値(16進)| 定義行 | 参照行")
        fp.write("\n ------------|--------|--------|---------------")
        for entry in sorted(self.xref_table, key=lambda item: item.name):
            prefix = " *" if entry.is_xdef else "  "
            fp.write(f"\n{prefix} {entry.name:<9}|")
            if entry.def_line == UNDEFINED:
                fp.write("   *    |  ----- |")
            else:
                fp.write(f"  {entry.address & 0xFFFF:04X}  |  {entry.def_line:05d} |")
            for ref in entry.refs:
                fp.write(f"  {ref:05d}")
        fp.write("\n ----------------------------------------------\n\n")

    def xref_define(self, name: str, address: int, line_number: int, is_xdef: bool) -> None:
        if name.startswith("$"):
            return
        for entry in self.xref_table:
            if entry.name == name:
                entry.address = address
                entry.def_line = line_number
                if is_xdef:
                    entry.is_xdef = True
                return
        self.xref_table.append(XrefEntry(name, address, is_xdef, line_number, []))

    def xref_reference(self, name: str, line_number: int) -> None:
        if line_number == UNDEFINED or name.startswith("$"):
            return
        for entry in self.xref_table:
            if entry.name == name:
                entry.refs.append(line_number)
                return
        self.xref_table.append(XrefEntry(name, UNDEFINED, False, UNDEFINED, [line_number]))

    def dump_object(self, oc: ObjectCode, fp_obj, fp_lst) -> None:
        if oc.passed == 0:
            return
        line = self.format_object_line(oc)
        if fp_obj:
            fp_obj.write(line)
        if fp_lst:
            fp_lst.write(line)

    def format_object_line(self, oc: ObjectCode) -> str:
        parts: list[str] = []
        parts.append(f"\n{oc.code_address & 0xFFFF:04X}:" if oc.code_address != UNDEFINED else "\n     ")
        if oc.code_type in (CodeType.BLANK, CodeType.START, CodeType.END, CodeType.DS):
            parts.append(self.format_address_const(oc.address_const))
            parts.append("     ")
        elif oc.code_type == CodeType.MACHINE:
            parts.append(f"{oc.value & 0xFFFF:04X}")
            parts.append("-" if oc.address_const.is_set() else " ")
            parts.append(self.format_address_const(oc.address_const))
        elif oc.code_type == CodeType.DC:
            if oc.address_const.label is not None:
                parts.append(self.format_address_const(oc.address_const))
                parts.append("    ")
            elif oc.data_words:
                parts.append(f"{oc.data_words[0] & 0xFFFF:04X}")
                parts.append(self.format_address_const(oc.address_const))
            else:
                parts.append("    ")
                parts.append(self.format_address_const(oc.address_const))
            parts.append(" ")
        else:
            parts.append("              ")
        parts.append(self.format_source_line(oc))
        if oc.code_type == CodeType.DC and len(oc.data_words) > 1:
            for idx, word in enumerate(oc.data_words[1:], start=1):
                parts.append(f"\n{(oc.code_address + idx) & 0xFFFF:04X}:{word & 0xFFFF:04X}")
        return "".join(parts)

    @staticmethod
    def format_address_const(adr: Address) -> str:
        if adr.value != UNDEFINED:
            return f"{adr.value & 0xFFFF:04X}" + ("R    " if adr.label is not None else "     ")
        if adr.label is not None:
            return f"{adr.label:<9}"
        return "         "

    @staticmethod
    def format_source_line(oc: ObjectCode) -> str:
        prefix = f"{'':20}  => " if oc.print_right_arrow or oc.line_number == UNDEFINED else f"[{oc.line_number:05d}]  "
        return prefix + oc.src_line

    @staticmethod
    def display_asm_error(oc: ObjectCode) -> None:
        if oc.line_number == UNDEFINED:
            return
        src_line = oc.src_line or ""
        err_pos = max(0, min(oc.asm_error_col, len(src_line)))
        marked_line = f"{src_line[:err_pos]}(^){src_line[err_pos:]}"
        print(
            f" {oc.line_number}行目 : {get_asm_error_message(oc.asm_error_code)}\n {marked_line}",
            file=__import__("sys").stderr,
        )


def split_by_delimiter(input_text: str, is_delim, max_tokens: int, skip_delimiters: bool) -> list[Token]:
    tokens: list[Token] = []
    effective_len = len(input_text)
    if not skip_delimiters and effective_len > 0 and is_delim(input_text[-1]):
        effective_len -= 1
    col = 0
    in_string = False
    in_token = False
    token_start = 0
    while col < effective_len and len(tokens) < max_tokens:
        ch = input_text[col]
        if in_string:
            if ch == "'":
                in_string = False
            col += 1
            continue
        if not in_token and not (is_delim(ch) and skip_delimiters):
            in_token = True
            token_start = col
        if ch == "'":
            in_string = True
        if is_delim(ch) and in_token:
            tokens.append(Token(token_start, input_text[token_start:col]))
            in_token = False
        col += 1
    if in_token and len(tokens) < max_tokens:
        tokens.append(Token(token_start, input_text[token_start:]))
    return tokens


def tokenize(line: str) -> list[Token]:
    return split_by_delimiter(line, lambda c: c in " \t", 4, True)


def tokenize_operand(operand: str, max_tokens: int) -> list[Token]:
    return split_by_delimiter(operand, lambda c: c == ",", max_tokens, False)


ERROR_MESSAGES = {
    ERR_PROC_LABEL_INVALID_CHAR: "アドレスラベルとして使用できない文字が含まれています",
    ERR_PROC_LABEL_TOO_LONG: "アドレスラベルの最大長8文字を超えています",
    ERR_PROC_LABEL_RSRVD_WORD: "アドレスラベルに予約語を使用することはできません",
    ERR_PARSE_OPCODE_NOT_EXIST: "命令語が必要です",
    ERR_PARSE_OPCODE_UNDEFINED: "未定義の命令語です",
    ERR_PARSE_OPERAND_ILLEGAL_INDEX_REGISTER: "インデックスレジスタはGR1〜GR7の何れかでなければいけません",
    ERR_PARSE_OPERAND_INVALID_OPERAND: "オペランドが正しくありません",
    ERR_PARSE_OPERAND_TOO_MANY_ARGS: "オペランド数が多すぎます",
    ERR_PROC_MACHINE_INVALID_OPERAND_TYPE: "この命令語では使用できないオペランド形式です",
    ERR_PARSE_OPERAND_HEXDECIMAL_OVERFLOW: "16進数の範囲（#FFFF）を超えています",
    ERR_SYMBOL_UNDEFINED: "未定義のシンボルです",
    ERR_SYMBOL_MULTI_DEF_LABEL: "アドレスラベルの多重定義です",
    ERR_PROC_START_NO_START_LABEL: "START命令にはアドレスラベルが必要です",
    ERR_PROC_START_INVALID_START_ADDRESS: "START命令のオペランドは10進数、16進数またはアドレスラベルでなければいけません",
    ERR_PROC_START_START_ADDRESS_RANGE_OVER: "START命令の開始アドレスが範囲外です",
    ERR_PROC_START_NOT_ALLOW_START_ADDRESS: "START命令に開始アドレスを指定することはできません",
    ERR_PROC_END_CANNOT_HAVE_LABEL: "END命令にはアドレスラベルを使用することはできません",
    ERR_PROC_DS_NEED_ARG: "DS命令にはオペランドが必要です",
    ERR_PROC_DS_INVALID_ARG: "DS命令のオペランド書式が不正です",
    ERR_PROC_DS_NOT_ALLOW_NEGATIVE_SIZE: "DS命令のオペランドは正の整数でなければいけません",
    ERR_ADDRESS_RANGE_OVERFLOW: "アドレスが許容範囲を超えています",
    ERR_PROC_DC_NEED_ARG: "DC命令にはオペランドが必要です",
    ERR_PROC_DC_INVALID_ARG: "DC命令のオペランド書式が不正です",
    ERR_PROC_DC_NULL_ARG: "DC命令にはオペランドが必要です",
    ERR_PROC_DC_HEXDECIMAL_OVERFLOW: "DC命令の整数リテラルが16進整数の数範囲(#FFFF)を超えています",
    ERR_PROC_MACRO_MISMATCH_ARG_COUNT: "マクロ命令の引数の数が定義と一致しません",
    ERR_PROC_MACRO_EXPANDED_LINE_TOO_LONG: "マクロ展開行が最大長255バイトを超えています",
}


def get_asm_error_message(err_code: int) -> str:
    return ERROR_MESSAGES.get(err_code, f"未定義エラー！！？ {err_code}")
