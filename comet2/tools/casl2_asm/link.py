from __future__ import annotations

import sys

from casl2.linker import Linker, parse_linker_args, print_linker_usage


def main(argv: list[str] | None = None) -> int:
    argv = list(sys.argv if argv is None else argv)
    if len(argv) < 2:
        print_linker_usage(argv[0])
        return 1
    try:
        base, entry, output, obj_files = parse_linker_args(argv[1:])
    except Exception as exc:
        print(f"casl2linker: {exc}", file=sys.stderr)
        return 1
    if not obj_files:
        print_linker_usage(argv[0])
        return 1
    try:
        Linker().link(obj_files, base=base, entry=entry, output=output)
    except Exception as exc:
        if not str(exc).startswith(("duplicate XDEF", "unresolved XREF", "入力ファイル")):
            print(f"casl2linker: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
