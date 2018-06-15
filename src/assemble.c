#include "assembler_utilities.h"
#include "assembler_branch.h"
#include "assembler_data_processing.h"
#include "assembler_multiply.h"
#include "assembler_special.h"
#include "assembler_single_data_transfer.h"
#define NO_EXPECTED_ARGS 3

void set_instruction(instruction *ins, char line[511], uint32_t *res,
                     uint32_t current_address, ST *symbol_table) {

    char *token;
    char *save;

    token = __strtok_r(line, " ,#\n", &save);

    ins->phrase = strcpy(malloc(strlen(token) + 1), token);

    switch (token[0]) {
        case 'b': token = __strtok_r(NULL, " ,#\n", &save);
            ins->expression = get_Address(symbol_table, token);
            res[current_address] = branch(ins, current_address * 4);
            break;
        case 'c':
        case 't': token = __strtok_r(NULL, " ,#\n", &save);
            ins->rn = register_handler(token);;
            token = __strtok_r(NULL, " ,#\n", &save);
            operand_handler(token, ins);
            token = __strtok_r(NULL, " ,#\n", &save);
            if (token != NULL) {
                ins->rm = ins->operand2;
                ins->rs = shiftType(token);
                token = __strtok_r(NULL, " ,#\n", &save);
                operand_handler(token, ins);
                if (ins->imm == 0) {
                    printf("rm : %d, type: %d, ins->operand: %d\n", ins->rm, ins->rs, ins->operand2);
                    ins->operand2 = (ins->operand2 << 8) | (ins->rs << 5) | (1 << 4) | (ins->rm);
                } else {
                    ins->operand2 = (ins->operand2 << 7) | (ins->rs << 5) | (ins->rm);
                }
                ins->imm = 0;
            }
            res[current_address] = assembler_dataProcessing(ins);
            break;
        case 'm': if (token[1] == 'u' || token[1] == 'l') {
                token = __strtok_r(NULL, " ,#\n", &save);
                ins->rd = register_handler(token);
                token = __strtok_r(NULL, " ,#\n", &save);
                ins->rm = register_handler(token);
                token = __strtok_r(NULL, " ,#\n", &save);
                ins->rs = register_handler(token);
                ins->imm = 0;
                if ((ins->phrase)[1] == 'l') {
                    token = __strtok_r(NULL, " ,#\n", &save);
                    ins->rn = register_handler(token);
                }
                res[current_address] = assembler_multiply(ins);
            } else if (token[1] == 'o') {
                token = __strtok_r(NULL, " ,#\n", &save);
                ins->rd = register_handler(token);;
                token = __strtok_r(NULL, " ,#\n", &save);
                operand_handler(token, ins);
                token = __strtok_r(NULL, " ,#\n", &save);
                if (token != NULL) {
                    ins->rm = ins->operand2;
                    ins->rs = shiftType(token);
                    token = __strtok_r(NULL, " ,#\n", &save);
                    operand_handler(token, ins);
                    if (ins->imm == 0) {
                        printf("rm : %d, type: %d, ins->operand: %d\n", ins->rm, ins->rs, ins->operand2);
                        ins->operand2 = (ins->operand2 << 8) | (ins->rs << 5) | (1 << 4) | (ins->rm);
                    } else {
                        ins->operand2 = (ins->operand2 << 7) | (ins->rs << 5) | (ins->rm);
                    }
                    ins->imm = 0;
                }
                res[current_address] = assembler_dataProcessing(ins);
            }
            break;
        case 'o':
        case 'e':
        case 'a': if (token[4] == 'q') {
                res[current_address] = assembler_special(ins);
                break;
            }
        case 'r':
        case 's': if (token[1] != 't') {
                token = __strtok_r(NULL, " ,#\n", &save);
                ins->rd = register_handler(token);
                token = __strtok_r(NULL, " ,#\n", &save);
                ins->rn = register_handler(token);
                token = __strtok_r(NULL, " ,#\n", &save);
                operand_handler(token, ins);
                token = __strtok_r(NULL, " ,#\n", &save);
                if (token != NULL) {
                    ins->rm = ins->operand2;
                    ins->rs = shiftType(token);
                    token = __strtok_r(NULL, " ,#\n", &save);
                    operand_handler(token, ins);
                    if (ins->imm == 0) {
                        printf("rm : %d, type: %d, ins->operand: %d\n", ins->rm, ins->rs, ins->operand2);
                        ins->operand2 = (ins->operand2 << 8) | (ins->rs << 5) | (1 << 4) | (ins->rm);
                    } else {
                        ins->operand2 = (ins->operand2 << 7) | (ins->rs << 5) | (ins->rm);
                    }
                    ins->imm = 0;
                }
                res[current_address] = assembler_dataProcessing(ins);
                break;
            }
        case 'l': if (token[1] != 's') {
                ins->rn = 0;
                ins->u = 1;
                ins->operand2 = 0;
                token = __strtok_r(NULL, " ,#\n", &save);
                ins->rd = register_handler(token);
                token = __strtok_r(NULL, "\n", &save);
                int i, shift = 1;
                if (token[0] == ' ')  {
                    shift = 2;
                }
                for (i = 0; i < strlen(token); i++) {
                    token[i] = token[i + shift];
                }
                if (token[0] != 'r') {
                    operand_handler(token, ins);
                    if ((ins->operand2) > 0xff) {
                        ins->rn = 15;
                        ins->lastAdd = ins->lastAdd + 1;
                        ins->p = 1;
                        res[ins->lastAdd - 1] = ins->operand2;
                        ins->operand2 = (ins->lastAdd - 1) * 4 - current_address * 4
                                        - 8;
                    } else {
                        ins->phrase = "mov";
                        res[current_address] = assembler_dataProcessing(ins);
                        break;
                    }
                } else {
                    address_handler(ins, token);
                }
                res[current_address] = single_data_transfer(ins);

            } else {
                token = __strtok_r(NULL, " ,#\n", &save);
                ins->rd = register_handler(token);
                token = __strtok_r(NULL, " ,#\n", &save);
                operand_handler(token, ins);
                res[current_address] = assembler_special(ins);
            }
            break;
    }


}

void run_assembler(int argc, char **argv) {
    instruction *ins = malloc(sizeof(instruction));
    ST *symbol_table = malloc(sizeof(ST));

    int a = 0, *num = &a, num_lines;
    uint32_t current_address = 0;
    char *data, *save, *label;

    data = read_file(argc, argv, num);

    if (data != NULL) {
        free(data);
    }

    label = malloc(511 * sizeof(char));

    char **array = init_2d_array(*num, 511);
    num_lines = *num;
    array[0] = __strtok_r(data, "\n", &save);

    for (int i = 1; i < num_lines; ++i) {
        array[i] = __strtok_r(NULL, "\n", &save);
    }

    for (int i = 0; i < num_lines; ++i) {
        if (contains(array[i], ':')) {
            label = strcpy(malloc(strlen(array[i]) + 1),array[i]);
            label[strlen(label) - 1] = '\0';
            add_symbol(current_address * 4, label, symbol_table);
        } else {
            current_address++;
        }
    }

    ins->lastAdd = current_address;
    uint32_t *res = malloc(current_address * 2 * sizeof(uint32_t));
    current_address = 0;

    for (int i = 0; i < num_lines; ++i) {
        if (strstr(array[i], ":") == NULL) {
            set_instruction(ins, array[i], res, current_address, symbol_table);
            current_address++;
        }
    }

    print_bin(argv[2], res, ins->lastAdd);
}
int main(int argc, char **argv) {

    run_assembler(argc, argv);

    exit(EXIT_SUCCESS);
}


