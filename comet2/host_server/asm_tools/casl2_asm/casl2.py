from __future__ import annotations

import os
import sys
from pathlib import Path

from casl2 import Assembler, parse_options_from_args, print_usage
from casl2.parse_options import HelpRequested


def derive_obj_name(src: str) -> str:
    path = Path(src)
    return str(path.with_suffix(".obj"))


def main(argv: list[str] | None = None) -> int:
    argv = list(sys.argv if argv is None else argv)
    prog = argv[0]
    if len(argv) < 2:
        print("アセンブルするCasl2ソースファイルを指定ください。", file=sys.stderr)
        print_usage(prog)
        return 1
    try:
        opts = parse_options_from_args(argv[1:], prog)
    except HelpRequested:
        return 0
    except Exception as exc:
        print(f"オプション解析エラー: {exc}", file=sys.stderr)
        print_usage(prog)
        return 1
    if not opts.source_files:
        print("アセンブルするCasl2ソースファイルを指定ください。", file=sys.stderr)
        print_usage(prog)
        return 1
    asm = Assembler(opts)
    out_file = opts.output_file
    if out_file is None and len(opts.source_files) > 1:
        out_file = derive_obj_name(opts.source_files[0])
    next_offset = -1
    exit_code = 0
    for idx, src in enumerate(opts.source_files):
        try:
            next_offset = asm.assemble_file(src, next_offset, out_file, idx > 0)
        except Exception:
            exit_code = 1
            break
    return exit_code


if __name__ == "__main__":
    raise SystemExit(main())
