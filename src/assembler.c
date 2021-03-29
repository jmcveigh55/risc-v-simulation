#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "assembler.h"


void instr_mem_from_asm_parse_list(instruction **instr_mem, linked_list *p_list, unsigned int len)
{
    int i;

    p_list = p_list->next; /* skip head */
    for (i=0; i< len; i++) {
        instr_mem[i] = instr_from_asm_parse( (parse*)p_list->item );
        p_list = p_list->next;
    }
}

static instruction* instr_from_asm_parse(parse *p_line)
{
    FILE *instrs_fd;
    size_t len;
    char *buff, *test_operand;
    instruction *instr_item = (instruction*)malloc(sizeof(*instr_item));

    instrs_fd = fopen("src/data/RISC-V_instruction_set.csv", "r");
    if (!instrs_fd) {
        perror("Error opening instruction reference file");
        exit(EXIT_FAILURE);
    }

    buff = NULL;
    getline( &buff, &len, instrs_fd ); /* Skip header */
    while( getline( &buff, &len, instrs_fd ) != -1 ) {
        buff[ strlen(buff)-1 ] = '\0';
        test_operand = strdup( strtok(buff, ",") );

        if (!strcmp(p_line->operand, test_operand))
            break;
    }
    free(test_operand);
    fclose(instrs_fd);

    printf("%s %s\n", p_line->operand, test_operand);

    switch(p_line->format) {
        case 'R':
            convert_R_format(instr_item, buff, p_line);
            break;
        case 'I':
            convert_I_format(instr_item, buff, p_line);
            break;
        case 'S':
            convert_S_format(instr_item, buff, p_line);
            break;
        case 'B':
            convert_B_format(instr_item, buff, p_line);
            break;
        case 'U':
            convert_U_format(instr_item, buff, p_line);
        case 'J':
            convert_J_format(instr_item, buff, p_line);
    }

    free(buff);

    return instr_item;
}

static void convert_R_format(instruction *instr_line, char *instr_meta, parse *p_line)
{
    return;
}

static void convert_I_format(instruction *instr_line, char *instr_meta, parse *p_line)
{
    return;
}

static void convert_S_format(instruction *instr_line, char *instr_meta, parse *p_line)
{
    return;
}

static void convert_B_format(instruction *instr_line, char *instr_meta, parse *p_line)
{
    return;
}

static void convert_U_format(instruction *instr_line, char *instr_meta, parse *p_line)
{
    return;
}

static void convert_J_format(instruction *instr_line, char *instr_meta, parse *p_line)
{
    return;
}

instruction** init_instruction_memory(unsigned int len)
{
    instruction **head;

    head = (instruction**)malloc(len * sizeof(*head));
    if (!head) {
        perror("Failed to malloc head: ");
        exit(EXIT_FAILURE);
    }

    return head;
}

void destroy_instruction_memory(instruction **instr_mem, unsigned int len)
{
    int i;

    for (i=0; i < len; i++)
        free(instr_mem[i]);
    free(instr_mem);
}

void print_assembled_structure(const instruction *instr_mem, unsigned int len)
{

}


