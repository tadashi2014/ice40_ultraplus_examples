#!/usr/bin/env python3
"""
Generate a small COMET II SPI IN/OUT echo test image.

Image layout:
  mem[0]  = reset vector
  mem[1]  = SVC 1 vector
  mem[2]  = SVC 2 vector
  mem[31] = initial SP

Program behavior:
  - SVC 1 reads one character from SPI MMIO into BUF
  - SVC 2 writes that character back to SPI MMIO
  - loop forever
  - SVC saves PR on the stack and the handlers return with RET
"""

from pathlib import Path
import struct


OUT_BIN = Path(__file__).with_name("comet2_inout_main.bin")
OUT_TXT = Path(__file__).with_name("comet2_inout_main.words.txt")


def op(opcode: int, r: int = 0, x: int = 0) -> int:
    return ((opcode & 0xFF) << 8) | ((r & 0x0F) << 4) | (x & 0x0F)


words = {}


def emit(addr: int, value: int, comment: str) -> None:
    words[addr] = (value & 0xFFFF, comment)


emit(0x0000, 0x0020, "reset vector -> MAIN")
emit(0x0001, 0x0040, "SVC 1 vector -> SVC1_IN")
emit(0x0002, 0x0080, "SVC 2 vector -> SVC2_OUT")
emit(0x001F, 0xBFF0, "initial SP")

emit(0x0020, op(0x12, 1, 0), "LAD GR1,BUF")
emit(0x0021, 0x00A0, "BUF")
emit(0x0022, op(0x10, 2, 0), "LD GR2,ONE")
emit(0x0023, 0x00A1, "ONE")
emit(0x0024, op(0xF0, 0, 0), "SVC 1")
emit(0x0025, 0x0001, "vector low byte = 1")
emit(0x0026, op(0x12, 1, 0), "LAD GR1,BUF")
emit(0x0027, 0x00A0, "BUF")
emit(0x0028, op(0x10, 2, 0), "LD GR2,ONE")
emit(0x0029, 0x00A1, "ONE")
emit(0x002A, op(0xF0, 0, 0), "SVC 2")
emit(0x002B, 0x0002, "vector low byte = 2")
emit(0x002C, op(0x64, 0, 0), "JUMP MAIN")
emit(0x002D, 0x0020, "MAIN")

emit(0x0040, op(0x10, 3, 0), "LD GR3,#C000")
emit(0x0041, 0xC000, "SPI status")
emit(0x0042, op(0x30, 3, 0), "AND GR3,ONE")
emit(0x0043, 0x00A1, "ONE")
emit(0x0044, op(0x63, 0, 0), "JZE SVC1_IN")
emit(0x0045, 0x0040, "SVC1_IN")
emit(0x0046, op(0x10, 3, 0), "LD GR3,#C001")
emit(0x0047, 0xC001, "SPI RX low")
emit(0x0048, op(0x30, 3, 0), "AND GR3,MASK8")
emit(0x0049, 0x00A3, "MASK8")
emit(0x004A, op(0x40, 3, 0), "CPA GR3,OP_IN")
emit(0x004B, 0x00A4, "OP_IN")
emit(0x004C, op(0x62, 0, 0), "JNZ SVC1_DROP")
emit(0x004D, 0x0060, "SVC1_DROP")
emit(0x004E, op(0x10, 3, 0), "LD GR3,#C002")
emit(0x004F, 0xC002, "SPI RX high")
emit(0x0050, op(0x30, 3, 0), "AND GR3,MASK8")
emit(0x0051, 0x00A3, "MASK8")
emit(0x0052, op(0x11, 3, 1), "ST GR3,0,GR1")
emit(0x0053, 0x0000, "0")
emit(0x0054, op(0x12, 1, 1), "LAD GR1,1,GR1")
emit(0x0055, 0x0001, "1")
emit(0x0056, op(0x21, 2, 0), "SUBA GR2,ONE")
emit(0x0057, 0x00A1, "ONE")
emit(0x0058, op(0x12, 4, 0), "LAD GR4,1")
emit(0x0059, 0x0001, "1")
emit(0x005A, op(0x11, 4, 0), "ST GR4,#C003")
emit(0x005B, 0xC003, "SPI RX ack")
emit(0x005C, op(0x62, 0, 0), "JNZ SVC1_IN")
emit(0x005D, 0x0040, "SVC1_IN")
emit(0x005E, op(0x81, 0, 0), "RET")
emit(0x005F, 0x0000, "padding")
emit(0x0060, op(0x12, 4, 0), "LAD GR4,1")
emit(0x0061, 0x0001, "1")
emit(0x0062, op(0x11, 4, 0), "ST GR4,#C003")
emit(0x0063, 0xC003, "SPI RX ack")
emit(0x0064, op(0x64, 0, 0), "JUMP SVC1_IN")
emit(0x0065, 0x0040, "SVC1_IN")

emit(0x0080, op(0x10, 3, 0), "LD GR3,#C000")
emit(0x0081, 0xC000, "SPI status")
emit(0x0082, op(0x30, 3, 0), "AND GR3,TWO")
emit(0x0083, 0x00A2, "TWO")
emit(0x0084, op(0x62, 0, 0), "JNZ SVC2_OUT")
emit(0x0085, 0x0080, "SVC2_OUT")
emit(0x0086, op(0x10, 3, 1), "LD GR3,0,GR1")
emit(0x0087, 0x0000, "0")
emit(0x0088, op(0x30, 3, 0), "AND GR3,MASK8")
emit(0x0089, 0x00A3, "MASK8")
emit(0x008A, op(0x52, 3, 0), "SLL GR3,8")
emit(0x008B, 0x0008, "8")
emit(0x008C, op(0x31, 3, 0), "OR GR3,OP_OUT")
emit(0x008D, 0x00A5, "OP_OUT")
emit(0x008E, op(0x11, 3, 0), "ST GR3,#C004")
emit(0x008F, 0xC004, "SPI TX low")
emit(0x0090, op(0x12, 4, 0), "LAD GR4,0")
emit(0x0091, 0x0000, "0")
emit(0x0092, op(0x11, 4, 0), "ST GR4,#C005")
emit(0x0093, 0xC005, "SPI TX commit")
emit(0x0094, op(0x12, 1, 1), "LAD GR1,1,GR1")
emit(0x0095, 0x0001, "1")
emit(0x0096, op(0x21, 2, 0), "SUBA GR2,ONE")
emit(0x0097, 0x00A1, "ONE")
emit(0x0098, op(0x62, 0, 0), "JNZ SVC2_OUT")
emit(0x0099, 0x0080, "SVC2_OUT")
emit(0x009A, op(0x81, 0, 0), "RET")
emit(0x009B, 0x0000, "padding")

emit(0x00A0, 0x0000, "BUF")
emit(0x00A1, 0x0001, "ONE")
emit(0x00A2, 0x0002, "TWO")
emit(0x00A3, 0x00FF, "MASK8")
emit(0x00A4, 0x0010, "OP_IN")
emit(0x00A5, 0x0020, "OP_OUT")

max_addr = max(words)
image = [0] * (max_addr + 1)
for addr, (value, _) in words.items():
    image[addr] = value

OUT_BIN.write_bytes(b"".join(struct.pack("<H", value) for value in image))

with OUT_TXT.open("w", encoding="ascii") as f:
    for addr, value in enumerate(image):
      comment = words.get(addr, (None, ""))[1]
      if comment:
          f.write(f"{addr:04X}: {value:04X}  ; {comment}\n")
      else:
          f.write(f"{addr:04X}: {value:04X}\n")

print(f"wrote {OUT_BIN} ({len(image) * 2} bytes)")
print(f"wrote {OUT_TXT}")