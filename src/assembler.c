#include <stdlib.h>
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
    instruction *instr_item = (instruction*)malloc(sizeof(*instr_item));

    switch(p_line->parse_format) {
        case 'R':
            convert_R_format(instr_item, p_line);
            break;
        case 'I':
            convert_I_format(instr_item, p_line);
            break;
        case 'S':
            convert_S_format(instr_item, p_line);
            break;
        case 'B':
            convert_B_format(instr_item, p_line);
            break;
        case 'J':
            convert_J_format(instr_item, p_line);
    }

    return instr_item;
}

static void convert_R_format(instruction *instr_line, parse *p_line)
{
    return;
}

static void convert_I_format(instruction *instr_line, parse *p_line)
{
    return;
}

static void convert_S_format(instruction *instr_line, parse *p_line)
{
    return;
}

static void convert_B_format(instruction *instr_line, parse *p_line)
{
    return;
}

static void convert_J_format(instruction *instr_line, parse *p_line)
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