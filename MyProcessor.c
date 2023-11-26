#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    REG, IMM
} AddrMode;

#define WORD_LENGTH 17
#define NUM_BLOCKS 16

char memo[NUM_BLOCKS][WORD_LENGTH] = {{"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"},
                                      {"0000000000000000"}};

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
    int i = R0;
    char addr[17], data[17];
    addr[16] = '\0';
    data[16] = '\0';
    while (!feof(dataFile)) {
        fscanf(dataFile, "%s %s", addr, data);
        strcpy(memo[binToDec(addr)], data);
        reg[i++] = binToDec(data);
    }
    fclose(dataFile);
}

void loadProg(char fileName[]) {
    FILE *progFile = fopen(fileName, "r");
    if (progFile == NULL) {
        perror("Error opening program file");
        exit(EXIT_FAILURE);
    }
    char instruct[WORD_LENGTH];
    instruct[WORD_LENGTH - 1] = '\0';
    int i = 0;
    while (!feof(progFile)) {
        fscanf(progFile, "%s", instruct);
        strcpy(memo[i], instruct);
        i++;
    }
    fclose(progFile);
}


void instructionDecode(Instruction *instruction) {
    char instruct[WORD_LENGTH];
    instruct[WORD_LENGTH - 1] = '\0';
    strcpy(instruct, memo[pc]);
    char type[2], opcode[5], addrMode[2], operand1[4], operand2[5], operand3[4];
    strncpy(type, instruct + 0, 1);
    type[1] = '\0';
    strncpy(opcode, instruct + 1, 4);
    opcode[4] = '\0';
    strncpy(addrMode, instruct + 5, 1);
    addrMode[1] = '\0';
    strncpy(operand1, instruct + 6, 3);
    operand1[3] = '\0';
    strncpy(operand2, instruct + 9, 4);
    operand2[4] = '\0';
    strncpy(operand3, instruct + 13, 3);
    operand3[3] = '\0';
    (*instruction).type = binToDec(type);
    (*instruction).opcode = binToDec(opcode);
    (*instruction).addrMode = binToDec(addrMode);
    (*instruction).operand1 = binToDec(operand1);
    (*instruction).operand2 = binToDec(operand2);
    (*instruction).operand3 = binToDec(operand3);
}

Instruction instructionFetch() {
    Instruction instruct;
    instruct.type = instruct.opcode = instruct.addrMode = instruct.operand1 = instruct.operand2 = instruct.operand3 = 0;
    if (pc < NUM_BLOCKS) {
        instructionDecode(&instruct);
        pc++;
    } else {
        instruct.type = -1;
    }
    return instruct;
}

void performALUOps(Instruction instruction) {
    switch (instruction.addrMode) {
        case REG:
            switch (instruction.opcode) {
                case ADD:
                    reg[instruction.operand1] = reg[instruction.operand2] + reg[instruction.operand3];
                    break;
                case SUB:
                    reg[instruction.operand1] = reg[instruction.operand2] - reg[instruction.operand3];
                    break;
                case AND:
                    reg[instruction.operand1] = reg[instruction.operand2] & reg[instruction.operand3];
                    break;
                case OR:
                    reg[instruction.operand1] = reg[instruction.operand2] | reg[instruction.operand3];
                    break;
                case XOR:
                    reg[instruction.operand2] = reg[instruction.operand2] ^ reg[instruction.operand3];
                    break;
                default:
                    return;
            }
            break;
        case IMM:
            switch (instruction.opcode) {
                case ADD:
                    reg[instruction.operand1] = instruction.operand2 + reg[instruction.operand3];
                    break;
                case SUB:
                    reg[instruction.operand1] = instruction.operand2 - reg[instruction.operand3];
                    break;
                case AND:
                    reg[instruction.operand1] = instruction.operand2 & reg[instruction.operand3];
                    break;
                case OR:
                    reg[instruction.operand1] = instruction.operand2 | reg[instruction.operand3];
                    break;
                case XOR:
                    reg[instruction.operand2] = instruction.operand2 ^ reg[instruction.operand3];
                    break;
                default:
                    return;
            }
            break;
        default:
            return;
    }
}

void performMemoOps(Instruction instruction) {
    switch (instruction.addrMode) {
        case REG:
            switch (instruction.opcode) {
                case LOAD:
                case STORE:
                    reg[instruction.operand1] = reg[instruction.operand2];
                    break;
                default:
                    return;
            }
            break;
        case IMM:
            switch (instruction.opcode) {
                case LOAD:
                case STORE:
                    reg[instruction.operand1] = instruction.operand2;
                    break;
                default:
                    return;
            }
            break;
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
    printf("\nRegister values:\n");
    for (int i = R0; i <= R7; i++) {
        printf("R%d: %d\n", i, reg[i]);
    }
}

void displayMemory() {
    printf("\nMemory: ");
    for (int i = 0; i < 16; i++) {
        printf("\n%s ", memo[i]);
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
    FILE *filePtr = fopen(progFile, "r");
    while (!feof(filePtr)) {
        char scanNext[WORD_LENGTH];
        fscanf(filePtr, "%s", scanNext);
        Instruction instruction = instructionFetch();
        if (instruction.type == -1) {
            break;
        }
        printf("Instruction: %d %d %d %d %d %d\n", instruction.type, instruction.opcode, instruction.addrMode,
               instruction.operand1, instruction.operand2, instruction.operand3);
        instructionExecute(instruction);
    }
    displayAllRegisterValues();
    displayMemory();
    return 0;
}
