from __future__ import annotations

from dataclasses import dataclass
from enum import IntEnum


class InstrClass(IntEnum):
    MACHINE = 0
    PSEUDO = 1
    MACRO = 2


@dataclass(slots=True)
class InstrEntry:
    klass: InstrClass
    value: int


class SymbolTable:
    def __init__(self) -> None:
        self.instrs: dict[str, InstrEntry] = {}
        self.labels: dict[tuple[str, int], int] = {}
        self.xdef_labels: dict[str, int] = {}

    def register_instr(self, name: str, klass: InstrClass, value: int) -> None:
        self.instrs[name.upper()] = InstrEntry(klass, value)

    def search_instr(self, name: str) -> InstrEntry | None:
        return self.instrs.get(name)

    def define_label(self, name: str, scope: int, address: int) -> None:
        key = (name, scope * 2)
        if key in self.labels and self.labels[key] >= 0:
            raise ValueError(f"label '{name}' multiply defined in scope {scope}")
        self.labels[key] = address

    def define_xdef_label(self, name: str, address: int) -> None:
        if name in self.xdef_labels:
            raise ValueError(f"XDEF label '{name}' multiply defined")
        self.xdef_labels[name] = address

    def resolve_label(self, name: str, scope: int) -> int | None:
        if name in self.xdef_labels:
            return self.xdef_labels[name]
        return self.labels.get((name, scope * 2))

    def clear_labels(self) -> None:
        self.labels.clear()
        self.xdef_labels.clear()
