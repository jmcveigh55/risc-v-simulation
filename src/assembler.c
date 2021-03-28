#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "assembler.h"


void instr_mem_from_asm_parse_list(linked_list **instr_mem, linked_list *p_list)
{
    p_list = p_list->next;

    while (p_list != NULL) {
        linked_list_add_head( instr_mem, (void*)instr_from_asm_parse( (parse*)p_list->item ) );
        p_list = p_list->next;
    }

    linked_list_reverse(instr_mem);
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

linked_list* init_instruction_memory()
{
    linked_list *head;

    head = (linked_list*)malloc(sizeof(*head));
    if (!head) {
        perror("Failed to malloc head: ");
        exit(EXIT_FAILURE);
    }

    head->item = NULL;
    head->next = NULL;

    return head;
}

void destroy_instruction_memory(linked_list **instr_mem)
{
    linked_list *c, *n;

    c = (*instr_mem)->next;
    while (c != NULL) {
        n = c->next;
        c = n;
    }
    free(*instr_mem);
    *instr_mem = NULL;
}

void print_assembled_structure(const linked_list *instr_mem)
{

}


