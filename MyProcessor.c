#include <stdio.h>
#include <stdlib.h>

typedef enum Opcode {
    ADD, SUB, AND, OR, XOR, LOAD, STORE
} Opcode;

typedef enum Register {
    R0, R1, R2, R3, R4, R5, R6, R7
} Register;

typedef struct Instruction {
    int type, addrMode, operand1, operand2, operand3;
    Opcode opcode;
} Instruction;

typedef enum InstructionType {
    ARITHMETIC_LOGICAL, DATA_TRANSFER
} InstructionType;

typedef enum AddrMode {
    IMM, REG
} AddrMode;

#define MEMO_SIZE 1024
int memo[MEMO_SIZE] = {0};

int pc = 0;

int reg[8] = {0};

int binToDec(const char *binStr) {
    return strtol(binStr, NULL, 2);
}

void loadData(char fileName[]) {
    FILE *dataFile = fopen(fileName, "r");
    if (dataFile == NULL) {
        perror("Error opening data file");
        exit(EXIT_FAILURE);
    }
    while (!feof(dataFile)) {
        char addr[16], data[16];
        fscanf(dataFile, "%s %s", addr, data);
        memo[binToDec(addr)] = binToDec(data);
    }
    fclose(dataFile);
}

void loadProg(char fileName[]) {
    FILE *progFile = fopen(fileName, "r");
    if (progFile == NULL) {
        perror("Error opening program file");
        exit(EXIT_FAILURE);
    }
    char instruct[16];
    while (!feof(progFile)) {
        fscanf(progFile, "%s", instruct);
        memo[pc++] = binToDec(instruct);
    }
    fclose(progFile);
}


void instructionDecode(Instruction instruction) {
    instruction.type = memo[pc];
    instruction.opcode = memo[pc + 1];
    instruction.addrMode = memo[pc + 2];
    instruction.operand1 = memo[pc + 3];
    instruction.operand1 = memo[pc + 4];
    instruction.operand1 = memo[pc + 5];
}

Instruction instructionFetch() {
    Instruction instruction;
    instruction.type = instruction.opcode = instruction.addrMode = instruction.operand1 = instruction.operand2 = instruction.operand3 = 0;
    instructionDecode(instruction);
    switch (memo[pc]) {
        case ARITHMETIC_LOGICAL:
            pc += 6;
            break;
        case DATA_TRANSFER:
            pc += 5;
            break;
    }
    return instruction;
}

void performALUOps(Instruction instruction) {
    switch (instruction.addrMode) {
        case IMM:
            switch (instruction.opcode) {
                case ADD:
                    instruction.operand1 = instruction.operand2 + reg[instruction.operand3];
                    break;
                case SUB:
                    instruction.operand1 = instruction.operand2 - reg[instruction.operand3];
                    break;
                case AND:
                    instruction.operand1 = instruction.operand2 & reg[instruction.operand3];
                    break;
                case OR:
                    instruction.operand1 = instruction.operand2 | reg[instruction.operand3];
                    break;
                case XOR:
                    instruction.operand2 = instruction.operand2 ^ reg[instruction.operand3];
                    break;
                default:
                    return;
            }
        case REG:
            switch (instruction.opcode) {
                case ADD:
                    instruction.operand1 = reg[instruction.operand2] + reg[instruction.operand3];
                    break;
                case SUB:
                    instruction.operand1 = reg[instruction.operand2] - reg[instruction.operand3];
                    break;
                case AND:
                    instruction.operand1 = reg[instruction.operand2] & reg[instruction.operand3];
                    break;
                case OR:
                    instruction.operand1 = reg[instruction.operand2] | reg[instruction.operand3];
                    break;
                case XOR:
                    instruction.operand2 = reg[instruction.operand2] ^ reg[instruction.operand3];
                    break;
                default:
                    return;
            }
        default:
            return;
    }

}

void performMemoOps(Instruction instruction) {
    switch (instruction.addrMode) {
        case IMM:
            switch (instruction.opcode) {
                case LOAD:
                case STORE:
                    reg[instruction.operand1] = instruction.operand2;
                default:
                    return;
            }
        case REG:
            switch (instruction.opcode) {
                case LOAD:
                case STORE:
                    reg[instruction.operand1] = reg[instruction.operand2];
                default:
                    return;
            }
        default:
            return;
    }
}

void instructionExecute(Instruction instruction) {
    switch (instruction.type) {
        case ARITHMETIC_LOGICAL:
            performALUOps(instruction);
            break;
        case DATA_TRANSFER:
            performMemoOps(instruction);
            break;
        default:
            printf("\nLafda hai bhai!");
    }
}

void displayAllRegisterValues() {
    printf("Register values:\n");
    for (int i = R0; i <= R7; i++) {
        printf("R%d: %d\n", i, reg[i]);
    }
}

int main() {
    char progFile[50], dataFile[32];
    printf("\nEnter data file name: ");
    scanf("%s", dataFile);
    loadData(dataFile);
    printf("\nEnter program file name: ");
    scanf("%s", progFile);
    loadProg(progFile);
    for (int i = 0; i <= 1; i++) {
        Instruction instruction = instructionFetch();
//        if (instruction == 0xFFFFFFFF) {
//            break;
//        }
        instructionDecode(instruction);
        instructionExecute(instruction);
    }
    displayAllRegisterValues();
    return 0;
}