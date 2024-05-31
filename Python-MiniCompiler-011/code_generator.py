class CodeGenerator:
    def __init__(self, ast):
        self.ast = ast
        self.code = []

    def generate(self):
        self.visit(self.ast)
        return '\n'.join(self.code)

    def visit(self, node):
        node_type = node[0]
        if node_type == 'statements':
            for statement in node[1]:
                self.visit(statement)
        elif node_type == 'num':
            self.code.append(f'PUSH {node[1][1]}')
        elif node_type == 'id':
            self.code.append(f'LOAD {node[1][1]}')
        elif node_type == 'assign':
            self.visit(node[2])  # Visit the right-hand side (expression)
            self.code.append(f'STORE {node[1][1]}')
        elif node_type == 'binop':
            self.visit(node[2])  # Visit left operand
            self.visit(node[3])  # Visit right operand
            op = node[1][1]
            if op == '+':
                self.code.append('ADD')
            elif op == '-':
                self.code.append('SUB')
            elif op == '*':
                self.code.append('MUL')
            elif op == '/':
                self.code.append('DIV')
