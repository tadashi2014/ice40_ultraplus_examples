from __future__ import annotations

import os
from dataclasses import dataclass, field

from .defs import DEFAULT_MAX_CODE_ADDR, DEFAULT_START_ADDRESS


class HelpRequested(Exception):
    pass


@dataclass(slots=True)
class ParseOptions:
    generate_asm_list: bool = False
    generate_cross_ref: bool = False
    allow_lower_case: bool = False
    allow_null_string: bool = False
    c_style_string: bool = False
    p_style_string: bool = False
    no_optimize: bool = False
    start_address: int | None = None
    max_code_address: int | None = None
    output_file: str | None = None
    symbol_table_size: int | None = None
    user_macro_file: str | None = None
    user_mnemonic_file: str | None = None
    source_files: list[str] = field(default_factory=list)

    def effective_start_address(self) -> int:
        return DEFAULT_START_ADDRESS if self.start_address is None else self.start_address

    def effective_max_code_address(self) -> int:
        return DEFAULT_MAX_CODE_ADDR if self.max_code_address is None else self.max_code_address


def _get_opt_arg(flag_arg: str, args: list[str], index: int) -> tuple[str, int]:
    if len(flag_arg) > 2:
        return flag_arg[2:], index
    index += 1
    if index < len(args):
        return args[index], index
    raise ValueError(f"option '{flag_arg}' requires an argument")


def parse_options_from_args(args: list[str], prog: str | None = None) -> ParseOptions:
    opts = ParseOptions()
    i = 0
    while i < len(args):
        arg = args[i]
        if len(arg) > 1 and arg[0] == "-":
            flag = arg[1]
            if flag == "a":
                opts.generate_asm_list = True
            elif flag == "x":
                opts.generate_cross_ref = True
            elif flag == "l":
                opts.allow_lower_case = True
            elif flag == "z":
                opts.allow_null_string = True
            elif flag == "c":
                if opts.p_style_string:
                    raise ValueError("-c and -p cannot be used together")
                opts.c_style_string = True
            elif flag == "p":
                if opts.c_style_string:
                    raise ValueError("-p and -c cannot be used together")
                opts.p_style_string = True
            elif flag == "d":
                opts.no_optimize = True
            elif flag == "h":
                print_usage(prog or os.path.basename(__file__))
                raise HelpRequested()
            elif flag == "w":
                pass
            elif flag == "m":
                opts.user_macro_file, i = _get_opt_arg(arg, args, i)
            elif flag == "n":
                opts.user_mnemonic_file, i = _get_opt_arg(arg, args, i)
            elif flag == "o":
                opts.output_file, i = _get_opt_arg(arg, args, i)
            elif flag == "b":
                value, i = _get_opt_arg(arg, args, i)
                n = int(value, 10)
                if not 0 <= n < 65536:
                    raise ValueError(f"-b value must be 0 <= v < 65536, got {n}")
                opts.start_address = n
            elif flag == "u":
                value, i = _get_opt_arg(arg, args, i)
                n = int(value, 10)
                if not 0 < n < 65536:
                    raise ValueError(f"-u value must be 0 < v < 65536, got {n}")
                opts.max_code_address = n
            elif flag == "t":
                value, i = _get_opt_arg(arg, args, i)
                opts.symbol_table_size = int(value, 10)
            else:
                raise ValueError(f"unknown flag '-{flag}'")
        else:
            opts.source_files.append(arg)
        i += 1
    return opts


def print_usage(prog: str) -> None:
    print(
        f"Usage: {prog} [-h] [-a] [-x] [-w] [-l] [-z] [-c] [-p] [-d]\n"
        "            [-o 出力ファイル名] [-m マクロ命令定義ファイル名]\n"
        "            [-n 機械語命令定義ファイル名] [-t シンボルテーブルサイズ]\n"
        "            [-u メモリアドレスの上限値] [-b プログラム開始アドレス]\n"
        "            アセンブリ言語ソースファイル名1 [アセンブリ言語ソースファイル名2…]\n\n"
        "        h : 使い方を表示する（このメッセージ）\n"
        "        a : アセンブルリストを出力する\n"
        "        x : クロスリファレンスリストを出力する\n"
        "        w : 9文字以上のラベル名の使用を許可する - 2018年3月現在未対応\n"
        "        l : 小文字の使用を許可する\n"
        "        z : 空文字列を許可する\n"
        "        c : C言語形式の文字列を生成する\n"
        "        p : Pascal言語形式の文字列を生成する\n"
        "        d : リテラル定数の最適化を行わない（重複して生成する）\n"
        "        o : 出力ファイルを指定する\n"
        "        m : マクロ命令定義ファイルを指定する\n"
        "        n : 機械語命令定義ファイルを指定する\n"
        "        t : シンボルテーブルサイズを指定する\n"
        "        u : スタック領域を除くメモリアドレスの上限値を指定する（0 < 値 < 65536）\n"
        "        b : プログラムの開始アドレスを指定する（0 <= 値 < 65536）",
        file=os.sys.stderr,
    )
