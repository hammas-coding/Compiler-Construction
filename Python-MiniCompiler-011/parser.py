class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.current_index = 0
        self.current_token = self.tokens[self.current_index]

    def consume(self, expected_type):
        if self.current_token[0] == expected_type:
            self.current_index += 1
            if self.current_index < len(self.tokens):
                self.current_token = self.tokens[self.current_index]
        else:
            raise RuntimeError(f'Expected {expected_type} but got {self.current_token[0]}')

    def parse(self):
        statements = []
        while self.current_index < len(self.tokens):
            statements.append(self.statement())
        return ('statements', statements)

    def statement(self):
        node = self.assignment()
        self.consume('END')
        return node

    def assignment(self):
        left = self.current_token
        self.consume('ID')
        self.consume('ASSIGN')
        right = self.expression()
        return ('assign', left, right)

    def expression(self):
        node = self.term()
        while self.current_token[0] in ('OP',) and self.current_token[1] in ('+', '-'):
            token = self.current_token
            self.consume('OP')
            node = ('binop', token, node, self.term())
        return node

    def term(self):
        node = self.factor()
        while self.current_token[0] in ('OP',) and self.current_token[1] in ('*', '/'):
            token = self.current_token
            self.consume('OP')
            node = ('binop', token, node, self.factor())
        return node

    def factor(self):
        token = self.current_token
        if token[0] == 'NUMBER':
            self.consume('NUMBER')
            return ('num', token)
        elif token[0] == 'LPAREN':
            self.consume('LPAREN')
            node = self.expression()
            self.consume('RPAREN')
            return node
        elif token[0] == 'ID':
            self.consume('ID')
            return ('id', token)
        else:
            raise RuntimeError(f'Unexpected token: {token}')
