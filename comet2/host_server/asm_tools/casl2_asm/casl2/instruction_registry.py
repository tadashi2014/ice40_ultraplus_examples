from __future__ import annotations

import json
from pathlib import Path

from .defs import (
    CONSTANT_DATA,
    DATA_STORE,
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
)
from .symbol_table import InstrClass, SymbolTable

DEFAULT_MACROS = [
    ("IN", 2, ["PUSH 0,GR1", "PUSH 0,GR2", "LAD GR1,%%", "LAD GR2,%%", "SVC 1", "POP GR2", "POP GR1"]),
    ("OUT", 2, ["PUSH 0,GR1", "PUSH 0,GR2", "LAD GR1,%%", "LAD GR2,%%", "SVC 2", "POP GR2", "POP GR1"]),
    ("RPUSH", 0, ["PUSH 0,GR1", "PUSH 0,GR2", "PUSH 0,GR3", "PUSH 0,GR4", "PUSH 0,GR5", "PUSH 0,GR6", "PUSH 0,GR7"]),
    ("RPOP", 0, ["POP GR7", "POP GR6", "POP GR5", "POP GR4", "POP GR3", "POP GR2", "POP GR1"]),
]

MACHINE_DEFS = [
    ("LD", 0x1000, OPR_RADR | OPR_RADRX | OPR_R1R2, 3),
    ("ST", 0x1100, OPR_RADR | OPR_RADRX, 3),
    ("LAD", 0x1200, OPR_RADR | OPR_RADRX, 3),
    ("ADDA", 0x2000, OPR_RADR | OPR_RADRX | OPR_R1R2, 3),
    ("SUBA", 0x2100, OPR_RADR | OPR_RADRX | OPR_R1R2, 3),
    ("ADDL", 0x2200, OPR_RADR | OPR_RADRX | OPR_R1R2, 3),
    ("SUBL", 0x2300, OPR_RADR | OPR_RADRX | OPR_R1R2, 3),
    ("AND", 0x3000, OPR_RADR | OPR_RADRX | OPR_R1R2, 3),
    ("OR", 0x3100, OPR_RADR | OPR_RADRX | OPR_R1R2, 3),
    ("XOR", 0x3200, OPR_RADR | OPR_RADRX | OPR_R1R2, 3),
    ("CPA", 0x4000, OPR_RADR | OPR_RADRX | OPR_R1R2, 3),
    ("CPL", 0x4100, OPR_RADR | OPR_RADRX | OPR_R1R2, 3),
    ("SLA", 0x5000, OPR_RADR | OPR_RADRX, 3),
    ("SRA", 0x5100, OPR_RADR | OPR_RADRX, 3),
    ("SLL", 0x5200, OPR_RADR | OPR_RADRX, 3),
    ("SRL", 0x5300, OPR_RADR | OPR_RADRX, 3),
    ("JMI", 0x6100, OPR_ADR | OPR_ADRX, 2),
    ("JNZ", 0x6200, OPR_ADR | OPR_ADRX, 2),
    ("JZE", 0x6300, OPR_ADR | OPR_ADRX, 2),
    ("JUMP", 0x6400, OPR_ADR | OPR_ADRX, 2),
    ("JPL", 0x6500, OPR_ADR | OPR_ADRX, 2),
    ("JOV", 0x6600, OPR_ADR | OPR_ADRX, 2),
    ("PUSH", 0x7000, OPR_ADR | OPR_ADRX, 2),
    ("POP", 0x7100, OPR_R, 1),
    ("CALL", 0x8000, OPR_ADR | OPR_ADRX, 2),
    ("RET", 0x8100, OPR_NONE, 0),
    ("SVC", 0xF000, OPR_ADR | OPR_ADRX, 2),
    ("NOP", 0x0000, OPR_NONE, 0),
]

PSEUDO_DEFS = [
    ("DC", CONSTANT_DATA, 64),
    ("DS", DATA_STORE, 1),
    ("START", MODULE_ENTRY, 1),
    ("END", MODULE_END, 0),
]

ADDR_MODE_MAP = {
    "OPR_NONE": OPR_NONE,
    "OPR_R": OPR_R,
    "OPR_R1R2": OPR_R1R2,
    "OPR_RADR": OPR_RADR,
    "OPR_RADRX": OPR_RADRX,
    "OPR_ADR": OPR_ADR,
    "OPR_ADRX": OPR_ADRX,
    "OPR_ADRADR": OPR_ADRADR,
    "MODULE_ENTRY": MODULE_ENTRY,
    "MODULE_END": MODULE_END,
    "DATA_STORE": DATA_STORE,
    "CONSTANT_DATA": CONSTANT_DATA,
}

CLASS_MAP = {"MACHINE": InstrClass.MACHINE, "PSEUDO": InstrClass.PSEUDO, "MACRO": InstrClass.MACRO}


def register_default_mnemonics(sym: SymbolTable) -> list[list[str]]:
    register_default_instructions(sym)
    return register_default_macros(sym)


def register_default_instructions(sym: SymbolTable) -> None:
    for name, opcode, modes, max_ops in MACHINE_DEFS:
        sym.register_instr(name, InstrClass.MACHINE, opcode | modes | max_ops)
    for name, type_const, max_ops in PSEUDO_DEFS:
        sym.register_instr(name, InstrClass.PSEUDO, type_const | OPR_NONE | max_ops)


def register_default_macros(sym: SymbolTable) -> list[list[str]]:
    macro_table: list[list[str]] = []
    for idx, (name, argc, lines) in enumerate(DEFAULT_MACROS):
        sym.register_instr(name, InstrClass.MACRO, (idx << 8) | argc)
        macro_table.append(list(lines))
    return macro_table


def load_mnemonics_from_file(sym: SymbolTable, path: str) -> None:
    entries = json.loads(Path(path).read_text(encoding="utf-8"))
    for name, definition in entries.items():
        klass = CLASS_MAP[definition["CLASS"]]
        opcode = int(definition["OP_CODE"], 0)
        modes = 0
        for mode in definition["ADR_MODE"]:
            if mode:
                modes |= ADDR_MODE_MAP[mode]
        sym.register_instr(name, klass, opcode | modes | definition["MAX_OPRS"])


def load_macros_from_file(sym: SymbolTable, path: str) -> list[list[str]]:
    entries = json.loads(Path(path).read_text(encoding="utf-8"))
    macro_table: list[list[str]] = []
    for idx, (name, definition) in enumerate(entries.items()):
        sym.register_instr(name, InstrClass.MACRO, (idx << 8) | definition["ARGC"])
        macro_table.append(list(definition["CODE"]))
    return macro_table
