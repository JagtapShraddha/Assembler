#include<stdio.h>
#include<stdlib.h>

struct Opcode {
    char opcode[4];
    char instruction[4];
    int num_params;
    char* parameters[2];  // Array of two strings for 1st and 2nd parameters
    int size;
    int is_mod_rm;        // 1 for yes, 0 for no
    int plus_rd;          // 1 for yes, 0 for no
    int num_bytes;
};

// Initialize an array of structures
struct Opcode opcodes[] = {
    {"01", "ADD", 2, {"reg", "reg"}, 2, 1, 0, 2},
    {"83", "ADD", 2, {"reg", "imm8"}, 3, 0, 0, 3},
    {"81", "ADD", 2, {"reg", "imm32"}, 6, 0, 0, 6},
    {"03", "ADD", 2, {"reg", "mem"}, 6, 1, 0, 6},
    {"83", "ADD", 2, {"mem", "imm8"}, 7, 0, 0, 7},
    {"81", "ADD", 2, {"mem", "imm32"}, 10, 0, 0, 10},
    {"01", "ADD", 2, {"mem", "reg"}, 6, 1, 0, 6},
    {"05", "ADD", 2, {"EAX", "imm32"}, 5, 0, 0, 5},
    {"29", "SUB", 2, {"reg", "reg"}, 2, 1, 0, 2},
    {"83", "SUB", 2, {"reg", "imm8"}, 3, 0, 0, 3},
    {"81", "SUB", 2, {"reg", "imm32"}, 6, 0, 0, 6},
    {"2B", "SUB", 2, {"reg", "mem"}, 6, 1, 0, 6},
    {"83", "SUB", 2, {"mem", "imm8"}, 7, 0, 0, 7},
    {"81", "SUB", 2, {"mem", "imm32"}, 10, 0, 0, 10},
    {"2B", "SUB", 2, {"mem", "reg"}, 6, 1, 0, 6},
    {"29", "SUB", 2, {"EAX", "imm32"}, 5, 0, 0, 5},
    {"39", "CMP", 2, {"reg", "reg"}, 2, 1, 0, 2},
    {"83", "CMP", 2, {"reg", "imm8"}, 3, 0, 0, 3},
    {"81", "CMP", 2, {"reg", "imm32"}, 6, 0, 0, 6},
    {"3B", "CMP", 2, {"reg", "mem"}, 6, 1, 0, 6},
    {"83", "CMP", 2, {"mem", "imm8"}, 7, 0, 0, 7},
    {"81", "CMP", 2, {"mem", "imm32"}, 10, 0, 0, 10},
    {"39", "CMP", 2, {"mem", "reg"}, 6, 1, 0, 6},
    {"3D", "CMP", 2, {"EAX", "imm32"}, 5, 0, 0, 5},
    {"EB", "JMP", 1, {"rel8"}, 2, 0, 0, 2},
    {"E9", "JMP", 1, {"rel16"}, 5, 0, 0, 5},
    {"E9", "JMP", 1, {"rel32"}, 5, 0, 0, 5},
    {"FF", "JMP", 1, {"reg"}, 2, 1, 0, 2},
    {"FF", "JMP", 1, {"mem"}, 6, 1, 0, 6},
    {"89", "MOV", 2, {"reg", "reg"}, 2, 1, 0, 2},
    {"B8", "MOV", 2, {"reg", "imm"}, 5, 0, 1, 5},
    {"8B", "MOV", 2, {"reg", "mem"}, 6, 1, 0, 6},
    {"C7", "MOV", 2, {"mem", "imm"}, 10, 0, 0, 10},
    {"89", "MOV", 2, {"mem", "reg"}, 6, 1, 0, 6},
    {"A3", "MOV", 2, {"mem", "EAX"}, 5, 0, 0, 5},
    {"40", "INC", 1, {"reg"}, 1, 0, 1, 1},
    {"FF", "INC", 1, {"mem"}, 6, 0, 0, 6},
    {"48", "DEC", 1, {"reg"}, 1, 0, 1, 1},
    {"FF", "DEC", 1, {"mem"}, 6, 0, 0, 6},
    {"31", "XOR", 2, {"reg", "reg"}, 2, 1, 0, 2},
    {"83", "XOR", 2, {"reg", "imm8"}, 3, 0, 0, 3},
    {"81", "XOR", 2, {"reg", "imm32"}, 6, 0, 0, 6},
    {"33", "XOR", 2, {"reg", "mem"}, 6, 1, 0, 6},
    {"83", "XOR", 2, {"mem", "imm8"}, 7, 0, 0, 7},
    {"81", "XOR", 2, {"mem", "imm32"}, 10, 0, 0, 10},
    {"31", "XOR", 2, {"mem", "reg"}, 6, 1, 0, 6},
    {"35", "XOR", 2, {"EAX", "imm32"}, 5, 0, 0, 5},
    {"F7", "MUL", 1, {"reg"}, 2, 0, 0, 2},
    {"F7", "MUL", 1, {"mem"}, 6, 0, 0, 6},
    {"F7", "DIV", 1, {"reg"}, 2, 0, 0, 2},
    {"F7", "DIV", 1, {"mem"}, 6, 0, 0, 6},
    {"74", "JZ", 1, {"rel16"}, 2, 0, 0, 2},
    {"0F84", "JZ", 1, {"rel32"}, 6, 0, 0, 6},
    {"75", "JNZ", 1, {"rel8"}, 2, 0, 0, 2},
    {"0F85", "JNZ", 1, {"rel16"}, 6, 0, 0, 6},
    {"0F85", "JNZ", 1, {"rel32"}, 6, 0, 0, 6}
};

// Function to print all opcodes
void print_opcodes() {
    int n = sizeof(opcodes) / sizeof(opcodes[0]);
    for (int i = 0; i < n; i++) {
        printf("%s | %s | %d | %s | %s | %d | %d | %d | %d\n",
            opcodes[i].opcode,
            opcodes[i].instruction,
            opcodes[i].num_params,
            opcodes[i].parameters[0],
            opcodes[i].parameters[1],
            opcodes[i].size,
            opcodes[i].is_mod_rm,
            opcodes[i].plus_rd,
            opcodes[i].num_bytes
        );
    }
}

int main() {
    print_opcodes();
    return 0;
}
