#ifndef CHECKMATE_MAIN_H
#define CHECKMATE_MAIN_H
#include <stdint.h>

enum Instruction {
    DUP,
    PUSH,
    POP,
    PRINT,
    CUSTOM = 4
};

struct CustomInstruction {
    enum Instruction opcode;
    void (*handler)(int32_t value, int32_t* stack, size_t* index, size_t stack_size);
};

int32_t push_stack(const int32_t value, size_t* index, int32_t* stack, const size_t stack_size);
int32_t peek_stack(const size_t index, const int32_t* stack);
int32_t print_stack(const int32_t* stack, const size_t index);
int32_t pop_stack(size_t* index);

void register_custom_instruction(enum Instruction opcode, void (*handler)(int32_t, int32_t*, size_t*, size_t));
void interpreter(enum Instruction* bytecode, int32_t* stack, size_t* index, size_t stack_size, size_t code_size);
#endif //CHECKMATE_MAIN_H
