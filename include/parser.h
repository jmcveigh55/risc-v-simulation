#ifndef _PARSER_H_
#define _PARSER_H_

#include "utilities.h"


typedef struct parse_s parse;
struct parse_s {
    char *operand;
    char format;
    char **parameters;
    int param_count;
};


void parse_instructions(linked_list **p_instrs, char *asm_file, unsigned int *line_count);
static parse* parse_line(char *asm_line);

linked_list* init_parse_list();
void destroy_parse_list(linked_list **p_list);
static void destroy_parse(parse **p_line);
static char get_format(const char *operand);
void print_parsed_structure(const linked_list *p_list);


#endif /* _PARSER_H_ */