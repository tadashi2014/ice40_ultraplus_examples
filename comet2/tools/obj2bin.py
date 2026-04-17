#!/usr/bin/env python3
from __future__ import annotations

import argparse
import re
import struct
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path


TOOLS_DIR = Path(__file__).resolve().parent
CASL2_ASM_DIR = TOOLS_DIR / "casl2_asm"
if str(CASL2_ASM_DIR) not in sys.path:
    sys.path.insert(0, str(CASL2_ASM_DIR))

from casl2 import Assembler, Linker  # noqa: E402
from casl2.parse_options import ParseOptions  # noqa: E402


CODE_RE = re.compile(r"^([0-9A-Fa-f]{4}):(.*)$")


@dataclass(slots=True)
class LinkedImage:
    start: int
    size: int
    words: list[int]


def default_runtime_path(name: str) -> Path:
    return TOOLS_DIR / "sample_asm_src" / name


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="CASL2 .obj/.cas を COMET II FPGA 用 .bin に変換します",
    )
    parser.add_argument(
        "inputs",
        nargs="+",
        help="リンク対象のユーザーモジュール (.obj または .cas)",
    )
    parser.add_argument(
        "-o",
        "--output",
        help="出力 .bin ファイル名 (省略時は最初の入力名.bin)",
    )
    parser.add_argument(
        "--base",
        type=lambda value: int(value, 0),
        default=0,
        help="リンク時の先頭アドレス (既定値: 0)",
    )
    parser.add_argument(
        "--entry",
        help="リンク時のエントリラベル (-E 相当)",
    )
    parser.add_argument(
        "--init",
        default=str(default_runtime_path("init.cas")),
        help="自動リンクする init モジュール (.obj または .cas)",
    )
    parser.add_argument(
        "--inout",
        default=str(default_runtime_path("inout.cas")),
        help="自動リンクする inout モジュール (.obj または .cas)",
    )
    parser.add_argument(
        "--no-runtime",
        action="store_true",
        help="init/inout の自動リンクを行わない",
    )
    parser.add_argument(
        "--exe-output",
        help="中間のリンク済み .exe を保存する",
    )
    parser.add_argument(
        "--words-output",
        help="アドレス付きダンプテキストを保存する",
    )
    return parser.parse_args(argv)


def derive_output_path(inputs: list[str], output: str | None) -> Path:
    if output:
        return Path(output)
    return Path(inputs[0]).with_suffix(".bin")


def derive_obj_path(source: Path) -> Path:
    return source.with_suffix(".obj")


def needs_rebuild(source: Path, target: Path) -> bool:
    return not target.exists() or source.stat().st_mtime > target.stat().st_mtime


def assemble_source(source: Path, output: Path) -> Path:
    opts = ParseOptions(generate_cross_ref=True, output_file=str(output))
    asm = Assembler(opts)
    asm.assemble_file(str(source), -1, str(output), False)
    return output


def ensure_object_file(path_str: str) -> Path:
    path = Path(path_str)
    if path.suffix.lower() == ".obj":
        if not path.exists():
            raise FileNotFoundError(f"オブジェクトファイルが見つかりません: {path}")
        return path
    if path.suffix.lower() != ".cas":
        raise ValueError(f".obj または .cas を指定してください: {path}")
    if not path.exists():
        raise FileNotFoundError(f"CASL2 ソースファイルが見つかりません: {path}")
    output = derive_obj_path(path)
    if needs_rebuild(path, output):
        print(f"assembling: {path} -> {output}", file=sys.stderr)
        assemble_source(path, output)
    return output


def parse_machine_words(field: str) -> list[int]:
    text = field.strip()
    if not text:
        return []
    parts = text.split("-")
    words: list[int] = []
    for part in parts[:2]:
        token = part.rstrip("R")
        if not token:
            continue
        if len(token) != 4 or any(ch not in "0123456789abcdefABCDEF" for ch in token):
            return []
        words.append(int(token, 16))
    return words


def parse_linked_image(exe_path: Path) -> LinkedImage:
    start = 0
    size = 0
    highest_used = -1
    parsed_words: dict[int, int] = {}

    with exe_path.open("r", encoding="utf-8") as fp:
        for raw in fp:
            line = raw.rstrip("\r\n")
            if line.startswith("START:"):
                token = line[6:].rstrip("R")
                start = int(token, 16)
                continue
            if line.startswith("SIZE:"):
                size = int(line[5:], 16)
                continue
            match = CODE_RE.match(line)
            if not match:
                continue
            addr = int(match.group(1), 16)
            code = match.group(2).split(None, 1)[0] if match.group(2).strip() else ""
            words = parse_machine_words(code)
            for offset, word in enumerate(words):
                parsed_words[addr + offset] = word
                highest_used = max(highest_used, addr + offset)

    image_words = max(start + size, highest_used + 1, 0)
    image = [0] * image_words
    for addr, word in parsed_words.items():
        if 0 <= addr < image_words:
            image[addr] = word
    return LinkedImage(start=start, size=size, words=image)


def write_bin(path: Path, words: list[int]) -> None:
    path.write_bytes(b"".join(struct.pack("<H", word & 0xFFFF) for word in words))


def write_words_dump(path: Path, words: list[int]) -> None:
    with path.open("w", encoding="ascii") as fp:
        for addr, word in enumerate(words):
            fp.write(f"{addr:04X}: {word:04X}\n")


def link_modules(obj_files: list[Path], base: int, entry: str | None, exe_output: Path | None) -> Path:
    linker = Linker()
    if exe_output is not None:
        linker.link([str(path) for path in obj_files], base=base, entry=entry, output=str(exe_output))
        return exe_output

    with tempfile.NamedTemporaryFile(prefix="obj2bin_", suffix=".exe", delete=False) as tmp:
        temp_path = Path(tmp.name)
    try:
        linker.link([str(path) for path in obj_files], base=base, entry=entry, output=str(temp_path))
        return temp_path
    except Exception:
        temp_path.unlink(missing_ok=True)
        raise


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    output_path = derive_output_path(args.inputs, args.output)
    words_output = Path(args.words_output) if args.words_output else output_path.with_suffix(".words.txt")
    exe_output = Path(args.exe_output) if args.exe_output else None

    try:
        obj_files: list[Path] = []
        if not args.no_runtime:
            obj_files.append(ensure_object_file(args.init))
        obj_files.extend(ensure_object_file(path) for path in args.inputs)
        if not args.no_runtime:
            obj_files.append(ensure_object_file(args.inout))

        exe_path = link_modules(obj_files, base=args.base, entry=args.entry, exe_output=exe_output)
        cleanup_exe = exe_output is None
        try:
            image = parse_linked_image(exe_path)
            write_bin(output_path, image.words)
            write_words_dump(words_output, image.words)
        finally:
            if cleanup_exe:
                exe_path.unlink(missing_ok=True)
    except Exception as exc:
        print(f"obj2bin: {exc}", file=sys.stderr)
        return 1

    print(f"obj2bin: wrote {output_path} ({len(image.words) * 2} bytes)")
    print(f"obj2bin: wrote {words_output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
