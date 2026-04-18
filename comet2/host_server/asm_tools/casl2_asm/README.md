# Python CASL2 Assembler

`C` / `Go` 実装を参照して移植した Python 版の CASL2 アセンブラです。

## 使い方

```sh
cd Python
python3 casl2.py ../CASL2_ASM_SAMPLE/P01.cas
```

オプションは既存実装と同じです。

## リンカ

```sh
cd Python
python3 casl2.py -x ../foo.cas
python3 casl2.py -x ../bar.cas
python3 link.py ../foo.obj ../bar.obj
```

リンクに使う `.obj` は `_XREF_` / `_XDEF_` を含む必要があるため、アセンブル時に `-x` を付けて生成してください。`-B`, `-E`, `-O` は C 版 `casl2linker` と同じです。

## FPGA 用 `.bin` 生成

`host_server/asm_tools/obj2bin.py` は、ユーザープログラムに `../firmware/init.obj` と
`../firmware/inout.obj` を自動リンクし、COMET II FPGA がそのまま読める
16-bit little-endian の `.bin` を生成します。

```sh
cd host_server/firmware
python3 ../asm_tools/obj2bin.py hanoi.obj -o hanoi.bin
```

`.cas` を直接渡した場合は、必要なら自動で `.obj` を再生成してからリンクします。
