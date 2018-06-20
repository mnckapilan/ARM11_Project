//
// Created by Kapilan M on 04/06/2018.
//

#ifndef ARM11_09_ASSEMBLER_DEFINITIONS_H
#define ARM11_09_ASSEMBLER_DEFINITIONS_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct symbol {
    uint32_t address;
    char *label;
    struct symbol *next;
} sym;

typedef struct symbol_table {
    sym *last;
} ST;

typedef struct instructions {

    char *phrase;
    uint32_t operand2;
    uint32_t rd;
    uint32_t rn;
    uint32_t rs;
    uint32_t rm;
    uint32_t imm;
    uint32_t expression;
    uint32_t lastAdd;
    uint32_t p;
    uint32_t u;
    uint32_t s;
    uint32_t w;
    uint32_t regList;

} instruction;


//enums for OPCODEs
typedef enum {
    AND = 0,
    EOR = 1,
    SUB = 2,
    RSB = 3,
    ADD = 4,
    TST = 8,
    TEQ = 9,
    CMP = 10,
    ORR = 12,
    MOV = 13
} OPCODE;

typedef enum {
    eq = 0,
    ne = 1,
    ge = 10,
    lt = 11,
    gt = 12,
    le = 13,
    al = 14,
} COMP;

void print_bin(char *filename, uint32_t *bin, uint32_t last_address);

void add_symbol(uint32_t address, char *label, ST *symbol_table);

char *read_file(int argc, char **argv, int *num_lines);

char **init_2d_array(int rows, int cols);

uint32_t convertOP2(uint32_t op2_32bit);

OPCODE getOpcodeDetails(char * phrase);

uint32_t getCond(char *cond);

uint32_t shiftType(char *type);

void operand_handler(char* operand2, instruction *ins);

uint32_t register_handler(char *token);

void address_handler(instruction *ins, char *token);

uint32_t get_Address(ST *symbolTable, char *label);

int contains(char *string, char find);

#endif //ARM11_09_ASSEMBLER_DEFINITIONS_H
