#include "assembler_utilities.h"

// Prints binary encoded instruction to specified file
void print_bin(char *filename, uint32_t *bin, uint32_t last_address) {
    FILE *f;
    f = fopen(filename, "w");

    assert(f != NULL);

    fwrite(bin, 4, last_address, f);
    if (ferror(f)) {
        perror("There was an error writing binary instruction to file.");
        exit(EXIT_FAILURE);
    }

    fclose(f);
}

void add_symbol(uint32_t address, char *label, ST *symbol_table) {
    sym *symbol = malloc(sizeof(sym));
    symbol->label = label;
    symbol->address = address;
    symbol->next = symbol_table->last;
    symbol_table->last = symbol;
}

char *read_file(int argc, char **argv, int *num_lines) {
    if (argc == 1) {
        perror("There is no file name.");
        exit(EXIT_FAILURE);
    }

    FILE *f;
    f = fopen(argv[1], "r");

    if (f == NULL) {
        perror("File not found.");
        exit(EXIT_FAILURE);
    }
    int num = 0;
    int num_space = 1;

    while (!feof(f)) {
        if (getc(f) == '\n' && num_space == 0) {
            num++;
            num_space = 1;
        } else {
            num_space = 0;
        }
    }
    rewind(f);

    *num_lines = num;
    char *data = malloc(num * 511 * sizeof(char));

    fread(data, (size_t ) num * 511, 1, f);
    if (ferror(f)) {
        perror("There was an error reading from then file.");
        exit(EXIT_FAILURE);
    }

    return data;
}

// Initialises a 2D array with specified parameters
char **init_2d_array(int rows, int cols) {
    char **res = malloc(sizeof(char*) * rows);

    for (int i = 0; i < rows; ++i) {
        res[i] = malloc(sizeof(char) * cols);
    }

    return res;
}

//converts an number larger than 8 bits to a number less than or equal to 8 bits with a rotation
uint32_t convertOP2(uint32_t op2_32bit) {
    uint32_t rot;
    uint32_t result = op2_32bit;
    int condition = 1;
    int rotate_value = 0;
    while(condition) {
        //check bit0 and bit1
        int bit0 = (op2_32bit & 1);
        int bit1 = ((op2_32bit & 2)>>1);
        //stop the while loop either bit0 or bit1 is 1
        if((bit0) || (bit1)) {
            condition = 0;
        }
        if(condition) {
            //shift value = n -> shift 2n times
            op2_32bit = op2_32bit >> 2;
            rotate_value++;
        }
    }

    if(0 != rotate_value) {
        //count rotate backward (1 round is 16)
        rot = (uint32_t)(16 - rotate_value);
        //rot is the shift value which starts at bit 8
        result = op2_32bit | (rot << 8);
    }
    return result;
}

OPCODE getOpcodeDetails(char * phrase) {
    OPCODE result = AND;
    if(!strcmp(phrase,"and")) {
        result = AND;
    } else if(!strcmp(phrase,"eor")) {
        result = EOR;
    } else if(!strcmp(phrase,"sub")) {
        result = SUB;
    } else if(!strcmp(phrase,"rsb")) {
        result = RSB;
    } else if(!strcmp(phrase,"add")) {
        result = ADD;
    } else if(!strcmp(phrase,"orr")) {
        result = ORR;
    } else if(!strcmp(phrase,"mov")) {
        result = MOV;
    } else if(!strcmp(phrase,"tst")) {
        result = TST;
    } else if(!strcmp(phrase,"teq")) {
        result = TEQ;
    } else if(!strcmp(phrase,"cmp")) {
        result = CMP;

    }
    return result;
}

uint32_t getCond(char *cond) {
    uint32_t res = al;

    if (strncmp("eq", cond, 2) == 0) {
        res = eq;
    } else if (strncmp("ne", cond, 2) == 0) {
        res = ne;
    } else if (strncmp("ge", cond, 2) == 0) {
        res = ge;
    } else if (strncmp("lt", cond, 2) == 0) {
        res = lt;
    } else if (strncmp("gt", cond, 2) == 0) {
        res = gt;
    } else if (strncmp("le", cond, 2) == 0) {
        res = le;
    }

    return res;
}


uint32_t shiftType(char *type) {

    uint32_t shift = 3;

    if (strcmp(type, "lsl") == 0) {
        shift = 0;
    } else if (strcmp(type, "lsr") == 0) {
        shift = 1;
    } else if (strcmp(type, "asr") == 0) {
        shift = 2;
    }

    return shift;

}

void operand_handler(char* operand2, instruction *ins) {

    char *eptr;
    char *save;

    if (operand2[0] == 'r') {
        ins->operand2 = atoi(__strtok_r(operand2, "-r", &save));
        ins -> imm = 0;
    } else if (strstr(operand2, "x") != NULL) {
        ins->operand2 = strtol(operand2, &eptr, 16);
        ins -> imm = 1;
    } else {
        ins->operand2 = strtol(operand2, &eptr, 10);
        ins -> imm = 1;
    }
    if (abs(ins -> operand2) != ins -> operand2) {
        ins -> u = 0;
    }


}

uint32_t register_handler(char *token) {

    uint32_t res;

    res = token[1] - '0';

    if (token[2] != '\0') {
        res = res * 10 + (token[2] - '0');
    }

    return res;

}

void address_handler(instruction *ins, char *token) {

    char *str[4], *save;
    str[0] = __strtok_r(token, "]", &save);
    str[1] = __strtok_r(NULL, "], #", &save);
    str[2] = __strtok_r(NULL, "], #", &save);
    str[3] = __strtok_r(NULL, "], #", &save);
    if (str[1] == NULL) {
        ins -> p = 1;
        str[0] = __strtok_r(str[0], ", #", &save);
        str[1] = __strtok_r(NULL, ", #", &save);
        str[2] = __strtok_r(NULL, "], #", &save);
        str[3] = __strtok_r(NULL, "], #", &save);
    } else {
        ins -> p = 0;
    }
    ins -> imm = 1;
    ins -> rn = register_handler(str[0]);
    if (str[1] != NULL) {
        operand_handler(str[1], ins);
        if (str[2] != NULL) {
            ins -> rm = ins -> operand2;
            if (str[1][0] == '-') {
                ins -> u = 0;
            }
            operand_handler(str[3], ins);
            ins -> imm = 0;
            ins -> operand2 = (ins -> operand2 << 7) | (shiftType(str[2]) << 5) | (ins -> rm);
        }
    }

}


uint32_t get_Address(ST *symbolTable, char *label) {

    sym *sym = symbolTable -> last;

    while (sym != NULL) {
        if (strcmp(sym -> label, label) == 0) {
            return sym -> address;
        }
        sym = sym -> next;
    }

    perror("label not found.");
    exit(EXIT_FAILURE);

}
