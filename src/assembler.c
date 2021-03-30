#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "assembler.h"
#include "registers.h"


void instr_mem_from_asm_parse_list(instruction *instr_mem, linked_list *p_list, unsigned int len)
{
    int i;

    p_list = p_list->next; /* skip head */
    for (i=0; i< len; i++) {
        instr_mem[i] = instr_from_asm_parse( (parse*)p_list->item );
        p_list = p_list->next;
    }
}

static instruction instr_from_asm_parse(parse *p_line)
{
    FILE *instrs_fd;
    size_t len;
    char *test_meta, *test_operand;
    instruction instr;

    instrs_fd = fopen("src/data/RISC-V_instruction_set.csv", "r");
    if (!instrs_fd) {
        perror("Error opening instruction reference file");
        exit(EXIT_FAILURE);
    }

    test_meta = NULL;
    getline( &test_meta, &len, instrs_fd ); /* Skip header */
    while( getline( &test_meta, &len, instrs_fd ) != -1 ) {
        test_meta[ strlen(test_meta)-1 ] = '\0';
        test_operand = strdup( test_meta );
        strtok(test_operand, ",");

        if (!strcmp(p_line->operand, test_operand))
            break;
    }

    fclose(instrs_fd);
    free(test_operand);

    switch(p_line->format) {
        case 'R':
            convert_R_format(&instr, test_meta, p_line);
            break;
        case 'I':
            convert_I_format(&instr, test_meta, p_line);
            break;
        case 'S':
            convert_S_format(&instr, test_meta, p_line);
            break;
        case 'B':
            convert_B_format(&instr, test_meta, p_line);
            break;
        case 'U':
            convert_U_format(&instr, test_meta, p_line);
        case 'J':
            convert_J_format(&instr, test_meta, p_line);
    }

    free(test_meta);

    return instr;
}

static void convert_R_format(instruction *instr, char *instr_meta, parse *p_line)
{
    strtok(instr_meta, ","); /* skip operand */

    unsigned int opcode = (int)strtol( strtok(NULL, ","), NULL, 2);
    unsigned int rd = reg_index( p_line->parameters[0] );
    unsigned int funct3 = (int)strtol( strtok(NULL, ","), NULL, 2);
    unsigned int rs1 = reg_index( p_line->parameters[1] );
    unsigned int rs2 = reg_index( p_line->parameters[2] );
    unsigned int funct7 = (int)strtol( strtok(NULL, ","), NULL, 2);

    *instr = 0;
    *instr |= opcode;
    *instr |= rd     << 7;
    *instr |= funct3 << (7 + 5);
    *instr |= rs1    << (7 + 5 + 3);
    *instr |= rs2    << (7 + 5 + 3 + 5);
    *instr |= funct7 << (7 + 5 + 3 + 5 + 5);
}

static void convert_I_format(instruction *instr, char *instr_meta, parse *p_line)
{
    strtok(instr_meta, ","); /* skip operand */

    unsigned int opcode = (int)strtol( strtok(NULL, ","), NULL, 2);
    unsigned int rd = reg_index( p_line->parameters[0] );
    unsigned int funct3 = (int)strtol( strtok(NULL, ","), NULL, 2);
    unsigned int rs1;
    int16_t imm;

    if(!strcmp(p_line->operand, "ld") ||
       !strcmp(p_line->operand, "jalr")) {
        char *temp = strdup(p_line->parameters[1]);

        imm = strtol(strtok(temp, "("), NULL, 10);
        rs1 = reg_index( strtok(NULL, ")") );

        free(temp);
    }
    else {
        rs1 = reg_index(p_line->parameters[1]);
        imm = strtol(p_line->parameters[2], NULL, 10);
    }

    *instr = 0;
    *instr |= opcode;
    *instr |= rd      << 7;
    *instr |= funct3  << (7 + 5);
    *instr |= rs1     << (7 + 5 + 3);
    *instr |= imm     << (7 + 5 + 3 + 5);
}

static void convert_S_format(instruction *instr, char *instr_meta, parse *p_line)
{
    printf("instr_meta: %s\n", instr_meta);
}

static void convert_B_format(instruction *instr, char *instr_meta, parse *p_line)
{
    printf("instr_meta: %s\n", instr_meta);
}

static void convert_U_format(instruction *instr, char *instr_meta, parse *p_line)
{
    printf("instr_meta: %s\n", instr_meta);
}

static void convert_J_format(instruction *instr, char *instr_meta, parse *p_line)
{
    printf("instr_meta: %s\n", instr_meta);
}

instruction* init_instruction_memory(unsigned int len)
{
    instruction *head;

    head = (instruction*)malloc(len * sizeof(*head));
    if (!head) {
        perror("Failed to malloc head: ");
        exit(EXIT_FAILURE);
    }

    return head;
}

void destroy_instruction_memory(instruction *instr_mem, unsigned int len)
{
    int i;

    free(instr_mem);
}

void print_assembled_structure(const instruction *instr_mem, unsigned int len)
{
    int i;

    for (i=0; i < len; i++) {
        printf("instruction_memory[%d]: 0x%x    \t0b", i,  instr_mem[i]);
        print_bin(instr_mem[i]);
        printf("\n");
    }
}
