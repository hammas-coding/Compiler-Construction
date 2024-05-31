class VirtualMachine:
    def __init__(self, instructions):
        self.instructions = instructions
        self.stack = []
        self.variables = {}
        self.instruction_pointer = 0

    def run(self):
        while self.instruction_pointer < len(self.instructions):
            instr = self.instructions[self.instruction_pointer]
            parts = instr.split()
            op = parts[0]

            if op == 'PUSH':
                self.stack.append(float(parts[1]))
            elif op == 'LOAD':
                var_name = parts[1]
                self.stack.append(self.variables[var_name])
            elif op == 'STORE':
                var_name = parts[1]
                value = self.stack.pop()
                self.variables[var_name] = value
            elif op == 'ADD':
                b = self.stack.pop()
                a = self.stack.pop()
                self.stack.append(a + b)
            elif op == 'SUB':
                b = self.stack.pop()
                a = self.stack.pop()
                self.stack.append(a - b)
            elif op == 'MUL':
                b = self.stack.pop()
                a = self.stack.pop()
                self.stack.append(a * b)
            elif op == 'DIV':
                b = self.stack.pop()
                a = self.stack.pop()
                self.stack.append(a / b)
            elif op == 'MOD':
                b = self.stack.pop()
                a = self.stack.pop()
                self.stack.append(a % b)
            elif op == 'POW':
                b = self.stack.pop()
                a = self.stack.pop()
                self.stack.append(a ** b)
            elif op == 'JZ':
                label = parts[1]
                if self.stack.pop() == 0:
                    self.instruction_pointer = self.labels[label]
            elif op == 'JMP':
                label = parts[1]
                self.instruction_pointer = self.labels[label]

            self.instruction_pointer += 1


# Example usage:
instructions = [
    'PUSH 5',
    'PUSH 3',
    'PUSH 2',
    'PUSH 8',
    'SUB',
    'MUL',
    'ADD',
    'STORE x'
]

vm = VirtualMachine(instructions)
vm.run()
print(vm.variables) 
