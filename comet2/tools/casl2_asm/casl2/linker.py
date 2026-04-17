from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path
import sys


MAX_NAME_LEN = 64


@dataclass(slots=True)
class Symbol:
    name: str
    addr: int


@dataclass(slots=True)
class Module:
    name: str = ""
    orig_start: int = 0
    orig_size: int = 0
    new_start: int = 0
    code_begin: int = 0
    code_end: int = 0
    xref: list[Symbol] = field(default_factory=list)
    xdef: list[Symbol] = field(default_factory=list)
    defs: list[Symbol] = field(default_factory=list)


class Linker:
    def __init__(self) -> None:
        self.modules: list[Module] = []
        self.lines: list[str] = []
        self.global_xdef: list[Symbol] = []
        self.opt_base = 0
        self.opt_base_set = False
        self.opt_entry = ""
        self.opt_output = ""

    @staticmethod
    def is_hex4(text: str) -> bool:
        return len(text) >= 4 and all(ch in "0123456789abcdefABCDEF" for ch in text[:4])

    @staticmethod
    def from_hex4(text: str) -> int:
        return int(text[:4], 16)

    @staticmethod
    def is_sym_char(ch: str) -> bool:
        return ch.isalnum() or ch in "_$"

    def lookup_xdef(self, name: str) -> int:
        for entry in self.global_xdef:
            if entry.name == name:
                return entry.addr
        return -1

    def add_sym(self, target: list[Symbol], name: str, addr: int) -> None:
        target.append(Symbol(name[: MAX_NAME_LEN - 1], addr))

    def parse_obj(self, path: str) -> None:
        sect = "NONE"
        module: Module | None = None
        in_header = False
        with open(path, "r", encoding="utf-8") as fp:
            for raw in fp:
                line = raw.rstrip("\n\r")
                if line.startswith("NAME:"):
                    if module is not None and sect == "CODE":
                        module.code_end = len(self.lines)
                    module = Module(name=line[5:], code_begin=len(self.lines), code_end=len(self.lines))
                    self.modules.append(module)
                    sect = "NONE"
                    in_header = True
                    continue
                if module is None:
                    continue
                if in_header:
                    if line.startswith("SIZE:"):
                        module.orig_size = self.from_hex4(line[5:])
                        continue
                    if line.startswith("START:"):
                        value = line[6:]
                        if value.endswith("R"):
                            value = value[:-1]
                        module.orig_start = self.from_hex4(value)
                        continue
                    if line == "":
                        in_header = False
                        sect = "CODE"
                    continue
                if line == "_XREF_:":
                    if sect == "CODE":
                        module.code_end = len(self.lines)
                    sect = "XREF"
                    continue
                if line == "_XDEF_:":
                    if sect == "CODE":
                        module.code_end = len(self.lines)
                    sect = "XDEF"
                    continue
                if line == "_DEF_:":
                    if sect == "CODE":
                        module.code_end = len(self.lines)
                    sect = "DEF"
                    continue
                if sect == "CODE":
                    self.lines.append(line)
                    continue
                if sect == "XREF":
                    idx = 0
                    while idx < len(line):
                        while idx < len(line) and line[idx] in " \t":
                            idx += 1
                        start = idx
                        while idx < len(line) and self.is_sym_char(line[idx]):
                            idx += 1
                        if idx > start:
                            self.add_sym(module.xref, line[start:idx], -1)
                    continue
                if sect in {"XDEF", "DEF"} and line:
                    name, sep, rest = line.partition(":")
                    if sep and name:
                        addr = self.from_hex4(rest)
                        self.add_sym(module.xdef if sect == "XDEF" else module.defs, name, addr)
        if module is not None and sect == "CODE":
            module.code_end = len(self.lines)

    def compute_addresses(self) -> None:
        if not self.modules:
            return
        self.modules[0].new_start = self.opt_base if self.opt_base_set else self.modules[0].orig_start
        for idx in range(1, len(self.modules)):
            prev = self.modules[idx - 1]
            self.modules[idx].new_start = prev.new_start + prev.orig_size

    def build_xdef_table(self) -> None:
        has_error = False
        for module in self.modules:
            offset = module.new_start - module.orig_start
            for entry in module.xdef:
                if self.lookup_xdef(entry.name) >= 0:
                    print(f"リンクエラー\nラベル<<{entry.name}>が複数箇所で宣言されています。", file=sys.stderr)
                    has_error = True
                    continue
                self.global_xdef.append(Symbol(entry.name, entry.addr + offset))
        if has_error:
            raise RuntimeError("duplicate XDEF labels")

    def check_unresolved_xrefs(self) -> bool:
        has_error = False
        seen: set[str] = set()
        for module in self.modules:
            for entry in module.xref:
                if self.lookup_xdef(entry.name) >= 0 or entry.name in seen:
                    continue
                print(f"リンクエラー\nラベル<<{entry.name}>が未定義です。", file=sys.stderr)
                seen.add(entry.name)
                has_error = True
        return has_error

    def lookup_all_syms(self, name: str) -> int:
        addr = self.lookup_xdef(name)
        if addr >= 0:
            return addr
        for module in self.modules:
            offset = module.new_start - module.orig_start
            for entry in module.defs:
                if entry.name == name:
                    return entry.addr + offset
        return -1

    def transform_line(self, line: str, offset: int) -> str:
        if len(line) < 5 or not self.is_hex4(line) or line[4] != ":":
            return line
        addr = self.from_hex4(line)
        out = f"{(addr + offset) & 0xFFFF:04X}:"
        pos = 5
        if len(line) >= pos + 4 and self.is_hex4(line[pos:]):
            out += line[pos : pos + 4]
            pos += 4
            if pos < len(line) and line[pos] == "-":
                out += "-"
                pos += 1
                if len(line) >= pos + 4 and self.is_hex4(line[pos:]):
                    has_r = len(line) > pos + 4 and line[pos + 4] == "R"
                    if has_r:
                        out += f"{(self.from_hex4(line[pos:]) + offset) & 0xFFFF:04X}R"
                        pos += 5
                    else:
                        out += line[pos : pos + 4]
                        pos += 4
                elif pos < len(line) and self.is_sym_char(line[pos]):
                    start = pos
                    while pos < len(line) and self.is_sym_char(line[pos]):
                        pos += 1
                    symbol = line[start:pos]
                    resolved = self.lookup_xdef(symbol)
                    out += f"{resolved & 0xFFFF:04X}" if resolved >= 0 else symbol
        elif pos < len(line) and self.is_sym_char(line[pos]):
            start = pos
            while pos < len(line) and self.is_sym_char(line[pos]):
                pos += 1
            symbol = line[start:pos]
            resolved = self.lookup_all_syms(symbol)
            if resolved >= 0:
                out += f"{resolved & 0xFFFF:04X}R    "
            else:
                out += symbol
        rest = line[pos:]
        stripped = rest.lstrip(" \t")
        if stripped.startswith("[") and len(out) <= 19:
            out += " " * (19 - len(out)) + stripped
        elif stripped.startswith("=>") and len(out) <= 41:
            out += " " * (41 - len(out)) + stripped
        else:
            out += rest
        return out

    @staticmethod
    def derive_exe_name(obj_path: str) -> str:
        return str(Path(obj_path).with_suffix(".exe"))

    def write_output(self, exe_path: str) -> None:
        total_size = sum(module.orig_size for module in self.modules)
        first = self.modules[0]
        start_addr = first.new_start
        if self.opt_entry:
            resolved = self.lookup_all_syms(self.opt_entry)
            if resolved >= 0:
                start_addr = resolved
            else:
                print(f"casl2linker: -E '{self.opt_entry}': ラベルが見つかりません", file=sys.stderr)
        with open(exe_path, "w", encoding="utf-8") as fp:
            fp.write(f"NAME:{first.name}\n")
            fp.write(f"SIZE:{total_size & 0xFFFF:04X}\n")
            fp.write(f"START:{start_addr & 0xFFFF:04X}R\n")
            fp.write("\n")
            for module in self.modules:
                offset = module.new_start - module.orig_start
                for idx in range(module.code_begin, module.code_end):
                    fp.write(self.transform_line(self.lines[idx], offset))
                    fp.write("\n")
            fp.write("_XREF_:\n")
            seen_xref: set[str] = set()
            for module in self.modules:
                for entry in module.xref:
                    if self.lookup_xdef(entry.name) >= 0 or entry.name in seen_xref:
                        continue
                    fp.write(f"{entry.name} \n")
                    seen_xref.add(entry.name)
            fp.write("_XDEF_:\n")
            for entry in self.global_xdef:
                fp.write(f"{entry.name}:{entry.addr & 0xFFFF:04X}R\n")
            fp.write("_DEF_:\n")
            for module in self.modules:
                offset = module.new_start - module.orig_start
                for entry in module.defs:
                    fp.write(f"{entry.name}:{(entry.addr + offset) & 0xFFFF:04X}R\n")
        print(f"casl2linker: '{exe_path}' を出力しました")

    def link(self, obj_files: list[str], base: int | None = None, entry: str | None = None, output: str | None = None) -> str:
        self.modules.clear()
        self.lines.clear()
        self.global_xdef.clear()
        self.opt_base_set = base is not None
        self.opt_base = 0 if base is None else base
        self.opt_entry = "" if entry is None else entry
        self.opt_output = "" if output is None else output
        for obj in obj_files:
            self.parse_obj(obj)
        if not self.modules:
            raise RuntimeError("入力ファイルにモジュールが見つかりません")
        self.compute_addresses()
        self.build_xdef_table()
        if self.check_unresolved_xrefs():
            raise RuntimeError("unresolved XREF labels")
        exe_path = self.opt_output or self.derive_exe_name(obj_files[0])
        self.write_output(exe_path)
        return exe_path


def parse_linker_args(args: list[str]) -> tuple[int | None, str | None, str | None, list[str]]:
    base = None
    entry = None
    output = None
    obj_files: list[str] = []
    idx = 0
    while idx < len(args):
        arg = args[idx]
        if arg == "-B":
            idx += 1
            if idx >= len(args):
                raise ValueError("-B の後に整数定数が必要です")
            base = int(args[idx], 0)
        elif arg == "-E":
            idx += 1
            if idx >= len(args):
                raise ValueError("-E の後にラベル名が必要です")
            entry = args[idx]
        elif arg == "-O":
            idx += 1
            if idx >= len(args):
                raise ValueError("-O の後にファイル名が必要です")
            output = args[idx]
        else:
            obj_files.append(arg)
        idx += 1
    return base, entry, output, obj_files


def print_linker_usage(prog: str) -> None:
    print(f"使い方: {prog} [-B 整数定数] [-E ラベル名] [-O ファイル名] file1.obj [file2.obj ...]", file=sys.stderr)
