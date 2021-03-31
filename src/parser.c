#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parser.h"


static const char *ROperands[] = {"slli", "srli", "srai", "add", "sub", "sll", "slt", "sltu", "xor",
                                  "srl", "sra", "or", "and", "slliw", "srliw", "sraiw", "addw", "subw",
                                  "sllw", "srlw", "sraw", "mul", "mulh", "mulhsu", "mulhu", "div", "divu",
                                  "rem", "remu", "mulw", "divw", "divuw", "remw", "remuw", NULL};
static const char *IOperands[] = {"jalr", "lb", "lh", "lw", "lbu", "lhu", "addi", "slti", "sltiu", "xori",
                                  "ori", "andi", "lwu", "ld", "addiw", NULL};
static const char *SOperands[] = {"sb", "sh", "sw", "sd", NULL};
static const char *BOperands[] = {"beq", "bne", "blt", "bge", "bltu", "bgeu", NULL};
static const char *UOperands[] = {"lui", "auipc", NULL};
static const char *JOperands[] = {"jal", NULL};


void parse_instructions(linked_list **p_instrs, char *asm_file, unsigned *line_count)
{
    FILE* asm_fd;
    size_t len;
    char* buff;

    asm_fd = fopen(asm_file, "r");
    if (!asm_fd) {
        perror("Error opening assembly file");
        exit(EXIT_FAILURE);
    }

    /* Stack each item in a linked list */
    buff = NULL;
    *line_count = 0;
    while( getline( &buff, &len, asm_fd ) != -1 )
    {
        buff[ strlen(buff)-1 ] = '\0';
        linked_list_add_head( p_instrs, (void*)parse_line(buff) );
        (*line_count)++;
    }

    /* Reverse the linked list to get the proper commands order
     * (Process is faster than adding to tail originally) */
    linked_list_reverse(p_instrs);

    fclose(asm_fd);
    free(buff);
}

static parse* parse_line(char *asm_line)
{
    char *s, *temp;
    unsigned ac, i;
    parse *p_line;

    p_line = (parse*)malloc(sizeof(*p_line));

    for (ac=0, s=asm_line; s[ac]; s[ac]==' ' ? ac++ : *s++); /* ac: param count */

    p_line->operand = strdup( to_lower( strtok(asm_line, " ") ) );
    p_line->format = get_format(p_line->operand);

    p_line->param_count = ac;
    p_line->parameters = (char**)malloc( ac * sizeof(*p_line->parameters) );
    if (!p_line->parameters) {
        perror("Failed to malloc p_line->parameters: ");
        exit(EXIT_FAILURE);
    }

    for(i=0; i < ac; i++)
        p_line->parameters[i] = strdup( strtok(NULL, ", ") );

    return p_line;
}

linked_list* init_parse_list()
{
    linked_list* head;

    head = (linked_list*)malloc(sizeof(*head));
    if (!head) {
        perror("Failed to malloc head: ");
        exit(EXIT_FAILURE);
    }

    head->item = NULL;
    head->next = NULL;

    return head;
}

void destroy_parse_list(linked_list **p_list)
{
    linked_list *c, *n;

    c = (*p_list)->next;
    while (c != NULL) {
        n = c->next;
        destroy_parse( (parse**)&c->item );
        c = n;
    }
    free(*p_list);
    *p_list = NULL;
}

static void destroy_parse(parse **p_line)
{
    unsigned i;

    free( (*p_line)->operand );

    for (i=0; i < (*p_line)->param_count; i++)
        free( (*p_line)->parameters[i] );
    free( (*p_line)->parameters );
    
    free(*p_line);
}

static char get_format(const char *operand)
{
    if (is_in(operand, ROperands))
        return 'R';
    else if (is_in(operand, IOperands))
        return 'I';
    else if (is_in(operand, SOperands))
        return 'S';
    else if (is_in(operand, BOperands))
        return 'B';
    else if (is_in(operand, JOperands))
        return 'J';

    return '\0';
}

void print_parsed_structure(const linked_list* p_list)
{
    unsigned i, j;

    i = 0;
    while (p_list != NULL) {
        if (i) {
            printf("item: %d\n", i-1);
            printf("\toperand: %s\n", ( (parse*)p_list->item )->operand);
            printf("\ttype: %c\n", ( (parse*)p_list->item )->format);

            for (j=0; j < ( (parse*)p_list->item )->param_count; j++)
                printf("\tparam %d: %s\n", j, ( (parse*)p_list->item )->parameters[j]);
        }

        i++;
        p_list = p_list->next;
    }
}
