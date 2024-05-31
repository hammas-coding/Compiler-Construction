# lexer.py
import re

class Lexer:
    def __init__(self, code):
        self.code = code
        self.tokens = []
        self.token_specification = [
            ('NUMBER',   r'\d+(\.\d*)?'),  # Integer or decimal number
            ('ASSIGN',   r'='),            # Assignment operator
            ('END',      r';'),            # Statement terminator
            ('ID',       r'[A-Za-z]+'),    # Identifiers
            ('OP',       r'[+\-*/]'),      # Arithmetic operators
            ('LPAREN',   r'\('),           # Left parenthesis
            ('RPAREN',   r'\)'),           # Right parenthesis
            ('NEWLINE',  r'\n'),           # Line endings
            ('SKIP',     r'[ \t]+'),       # Skip over spaces and tabs
            ('MISMATCH', r'.'),            # Any other character
        ]
        self.token_regex = '|'.join(f'(?P<{pair[0]}>{pair[1]})' for pair in self.token_specification)

    def tokenize(self):
        for match in re.finditer(self.token_regex, self.code):
            kind = match.lastgroup
            value = match.group(kind)
            if kind == 'NUMBER':
                value = float(value) if '.' in value else int(value)
            elif kind == 'ID' and value in {'if', 'else', 'while', 'for', 'def', 'return', 'class'}:
                kind = value.upper()
            elif kind == 'NEWLINE':
                continue
            elif kind == 'SKIP':
                continue
            elif kind == 'MISMATCH':
                raise RuntimeError(f'Unexpected character: {value}')
            self.tokens.append((kind, value))
        return self.tokens
