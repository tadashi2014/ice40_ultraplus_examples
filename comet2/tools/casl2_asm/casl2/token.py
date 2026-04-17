from __future__ import annotations

from dataclasses import dataclass

from .defs import MAX_LABEL_LENGTH


@dataclass(slots=True)
class Token:
    offset: int
    text: str

    def is_comment(self) -> bool:
        return self.text.startswith(";")

    def is_gr(self, allow_lower: bool) -> bool:
        s = self.text.upper() if allow_lower else self.text
        return len(s) == 3 and s[:2] == "GR" and "0" <= s[2] <= "7"

    def get_gr_num(self, allow_lower: bool) -> int | None:
        if not self.is_gr(allow_lower):
            return None
        s = self.text.upper() if allow_lower else self.text
        return ord(s[2]) - ord("0")

    def is_label(self, allow_lower: bool) -> bool:
        if not self.text:
            return False
        first = self.text[0]
        if allow_lower:
            first_ok = first.isalpha()
        else:
            first_ok = "A" <= first <= "Z"
        if not first_ok:
            return False
        for ch in self.text[1:]:
            if allow_lower:
                if not (ch.isalpha() or ch.isdigit()):
                    return False
            elif not (("A" <= ch <= "Z") or ch.isdigit()):
                return False
        return True

    def is_decimal(self) -> bool:
        if not self.text:
            return False
        digits = self.text
        if digits[0] in "+-":
            digits = digits[1:]
        return bool(digits) and digits.isdigit()

    def is_hex_decimal(self) -> bool:
        if not self.text.startswith("#"):
            return False
        hex_part = self.text[1:]
        return 0 < len(hex_part) <= 4 and all(ch in "0123456789ABCDEF" for ch in hex_part)

    def is_digit(self) -> bool:
        return self.is_decimal() or self.is_hex_decimal()

    def is_string(self, allow_null: bool) -> bool:
        if len(self.text) < 2 or self.text[0] != "'" or self.text[-1] != "'":
            return False
        inner = self.text[1:-1]
        in_escape = False
        for ch in inner:
            if in_escape:
                if ch != "'":
                    return False
                in_escape = False
            elif ch == "'":
                in_escape = True
        if in_escape:
            return False
        return allow_null if len(self.text) == 2 else True

    def is_literal(self, allow_null: bool) -> bool:
        if not self.text.startswith("="):
            return False
        inner = Token(self.offset + 1, self.text[1:])
        return (
            inner.is_string(allow_null)
            or inner.is_decimal()
            or inner.is_hex_decimal()
            or inner.is_label(False)
            or inner.is_label(True)
        )

    def literal_value(self) -> "Token":
        return Token(self.offset + 1, self.text[1:])

    def is_reserved_word(self, allow_lower: bool) -> bool:
        return self.is_gr(allow_lower)

    def decimal_to_integer(self) -> int:
        value = int(self.text, 10)
        return value & 0xFFFFFFFF if value < 0 else value

    def hex_decimal_to_integer(self) -> int:
        return int(self.text[1:], 16)

    def get_casl2_string_length(self, c_style: bool, p_style: bool) -> int:
        inner = self.text[1:-1]
        length = 0
        in_escape = False
        for ch in inner:
            if in_escape:
                in_escape = False
            else:
                length += 1
                if ch == "'":
                    in_escape = True
        if c_style or p_style:
            length += 1
        return length

    def gen_casl2_string_code(self, c_style: bool, p_style: bool) -> list[int]:
        total = self.get_casl2_string_length(c_style, p_style)
        result = [0] * total
        inner = self.text[1:-1]
        dst = 0
        if p_style:
            result[dst] = total - 1
            dst += 1
        limit = total - 1 if c_style else total
        in_escape = False
        for ch in inner:
            if dst >= limit:
                break
            if in_escape:
                in_escape = False
            else:
                result[dst] = ord(ch)
                dst += 1
                if ch == "'":
                    in_escape = True
        return result

    def label_length_ok(self) -> bool:
        return len(self.text) <= MAX_LABEL_LENGTH
