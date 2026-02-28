#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include <stdbool.h>
#include <limits.h>
#include <string.h>

void (*user_custom_handler)(int32_t value, int32_t* stack, size_t* index, size_t stack_size) = NULL;
/*
 * Check if stack is full
*/
bool is_full(size_t index, size_t stack_size) {
    return index >= stack_size;
}

/*
 *Push to stack helper, pushes value to stack
*/
int32_t push_stack(const int32_t value, size_t* index, int32_t* stack, const size_t stack_size) {
    //If full
    if (is_full(*index, stack_size)) {
        printf("[checkmate] Stack is full. Cannot push to stack.\n");
        return 1;
    }

    stack[*index] = value;
    *index += 1;
    return 0;
}

/*
 *Peek at stack helper, returns top-most value
*/
int32_t peek_stack(const size_t index, const int32_t* stack) {
    //current value is always stack[index-1]
    return stack[index-1];
}

/*
 * Print top-most value
*/
int32_t print_stack(const int32_t* stack, const size_t index) {
    if (index == 0) {
        printf("[checkmate] Stack is empty.\n");
        return 1;
    }
    int32_t value = stack[index-1];
    printf("%d\n", value);
    fflush(stdout);
    return 0;
}

/*
 * Pop current value from stack helper, removes top-most value
*/
int32_t pop_stack(size_t* index) {
    //If not index:
    if (*index == 0) {
        printf("[checkmate] Stack is empty.\n");
        return 1;
    }

    (*index)--;
    return 0;
}


struct CustomInstruction* user_instructions = NULL;
size_t user_instruction_count = 0;

/*
 * Warn user if their instruction is 0-3
*/
void warn_user_instruction(struct CustomInstruction* instructions, size_t instruction_count, enum Instruction instruct) {
    for (size_t i = 0; i < instruction_count; i++) {
        if (instructions[i].opcode == instruct) {
            printf("[checkmate] Interpreter Warning: User defined instruction is equivalent to instruction %d, which is a built-in.", instruct);
        }
    }
}

/*
 * Register a new custom instruction
*/
void register_custom_instruction(enum Instruction opcode, void (*handler)(int32_t, int32_t*, size_t*, size_t)) {
    warn_user_instruction(user_instructions, user_instruction_count, opcode);
    struct CustomInstruction* tmp = realloc(user_instructions, sizeof(struct CustomInstruction) * (user_instruction_count + 1));
    if (!tmp) {
        printf("[checkmate] Failed to allocate memory for custom instruction.\n");
        return;
    }
    user_instructions = tmp;
    user_instructions[user_instruction_count].opcode = opcode;
    user_instructions[user_instruction_count].handler = handler;
    user_instruction_count++;
}

/*
 * Interpret bytecode
*/
void interpreter(enum Instruction* bytecode, int32_t* stack, size_t* index, size_t stack_size, size_t code_size) {
    for (size_t i = 0; i < code_size; i++) {
        switch (bytecode[i]) {
            case DUP:
                if (*index == 0) {
                    printf("[checkmate] Stack is empty.\n");
                    break;
                }
                int32_t dup_val = peek_stack(*index, stack);
                int32_t retdup = push_stack(dup_val, index, stack, stack_size);
                if (retdup != 0) {
                    printf("[checkmate] Stack overflow in DUP.\n");
                    return;
                }
                break;
            case PUSH:
                if (i + 1 >= code_size) {
                    return;
                }
                int32_t retpush = push_stack(bytecode[i + 1], index, stack, stack_size);
                if (retpush != 0) {
                    return;
                }
                i++;
                break;
            case POP:
                int32_t retpop = pop_stack(index);
                if (retpop != 0) {
                    return;
                }
                break;
            case PRINT:
                int32_t retprint = print_stack(stack, *index);
                if (retprint != 0) {
                    return;
                }
                break;
            default:
                for (size_t j = 0; j < user_instruction_count; j++) {
                    if (bytecode[i] == user_instructions[j].opcode) {
                        if (i + 1 >= code_size) {
                            printf("[checkmate] Missing operand for custom instruction.\n");
                            return;
                        }
                        user_instructions[j].handler(bytecode[i+1], stack, index, stack_size);
                        i++;
                        goto next_instruction;
                    }
                }
                printf("[checkmate] Interpreter Errno 1: Unrecognized bytecode instruction: %d\n.", bytecode[i]);
        }
        next_instruction:;
    }
}

/*
 * Helper for stack add
*/
void add(int32_t value, int32_t* stack, size_t* index, size_t stack_size) {
    if (*index == 0) {
        printf("[checkmate] Stack is empty.\n");
        return;
    }

    //store the current_value
    int32_t current_value = peek_stack(*index, stack);

    if ((value > 0 && current_value > INT_MAX - value) ||
    (value < 0 && current_value < INT_MIN - value)) {
        printf("[checkmate] Integer overflow in add.\n");
        return;
    }

    pop_stack(index);
    int32_t ret = push_stack(current_value+value, index, stack, stack_size);

    if (ret != 0) {
        return;
    }
}

/*
 *Handles specific arguments like help and what
*/
int handle_specific_arguments(int arg_count, char **args, float program_version, char program_debug_msg[]){
	if (arg_count > 1 && (strcmp(args[1], "-v") == 0 || strcmp(args[1], "--version") == 0 || strcmp(args[1], "--ver") == 0)){
		printf("[checkmate] Checkmate Version %f %s\n", program_version, program_debug_msg);
		return 0;
	}
	
    if ((arg_count > 1 && strcmp(args[1], "--what") == 0) || (arg_count > 1 && strcmp(args[1], "-w") == 0)) {
  		printf("[checkmate] Welcome to Checkmate!\n");
	    printf("[checkmate] Checkmate is a modular, stack-based bytecode VM made in C.\n");
	    printf("[checkmate] Modes: test, REPL\n");
	    printf("[checkmate] To access test mode, do checkmate --test\n");
	    printf("[checkmate] To access REPL, run with no arguments\n");
	    return 0;                                                 
    }

    if ((arg_count > 1 && strcmp(args[1], "--help") == 0) || (arg_count > 1 && strcmp(args[1], "-h") == 0)){
	    printf("[checkmate] Arg[1] = --test/-t, --what/-w, --help/-h, --ver/--version/-v.\n");
	    return 0;
    }
    return 1; 
}

/*
 * Main, handles arguments and testing
*/
int main(int argc, char **argv) {
    // parameters, stack, index
    size_t stack_size = 1028;
    int32_t* stack = calloc(stack_size, sizeof(int32_t));
    size_t index = 0;
    float version = 0.1;
    char debug_message[256];
    strcpy(debug_message, "UNRELEASED DEBUG");

    if (argc > 2) {
        printf("[checkmate] Too many arguments!\n");
        return 1;
    }

	//if return 0, main returns 0, else continue
	int func_ret = handle_specific_arguments(argc, argv, version, debug_message);
	if (func_ret == 0){
		return func_ret;
	}
	
    //test
    if ((argc > 1 && strcmp(argv[1], "--test") == 0) || (argc > 1 && strcmp(argv[1], "-t") == 0)) {
        printf("[checkmate] Running test.\n");
        //register instruction
        register_custom_instruction(CUSTOM, &add);
        int size = 5;
        enum Instruction* bytecode = calloc(size, sizeof(enum Instruction));
        bytecode[0] = PUSH;
        bytecode[1] = 1;
        bytecode[2] = CUSTOM;
        bytecode[3] = 1;
        bytecode[4] = PRINT;
        printf("[checkmate] Output: ");
        interpreter(bytecode, stack, &index, stack_size, (const size_t)size);
        free(bytecode);
        free(stack);
        free(user_instructions);
        return 0;
    }

    printf("[checkmate] No arguments found or unrecognized arguments found: running REPL.\n");
    size_t size = 0;
    printf("[checkmate] Size:");
    if (scanf("%zu", &size) != 1) {
        printf("\n[checkmate] Invalid input.\n");
        return 1;
    }

    enum Instruction* bytecode = calloc(size, sizeof(enum Instruction));
    for (size_t i = 0; i < size; i++) {
        int32_t value;
        printf("[checkmate] Value:");
        if (scanf("%d", &value) != 1) {
            printf("[checkmate] Invalid input.\n");
            return 1;
        }
        bytecode[i] = (enum Instruction)value;
    }

    const size_t code_size = size;
    interpreter(bytecode, stack, &index, stack_size, code_size);

    free(bytecode);
    free(stack);
    free(user_instructions);

    return 0;
}
