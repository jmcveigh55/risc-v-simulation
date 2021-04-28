#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include <stdint.h>

#include "utilities.h"
#include "parser.h"

typedef uint32_t instruction;

void instr_mem_from_asm_parse_list(instruction *instr_mem, linked_list *p_list, unsigned len);
static instruction instr_from_asm_parse(parse *p_line);
static void convert_R_format(instruction *instr, char *instr_meta, parse *p_line);
static void convert_I_format(instruction *instr, char *instr_meta, parse *p_line);
static void convert_S_format(instruction *instr, char *instr_meta, parse *p_line);
static void convert_B_format(instruction *instr, char *instr_meta, parse *p_line);
static void convert_U_format(instruction *instr, char *instr_meta, parse *p_line);
static void convert_J_format(instruction *instr, char *instr_meta, parse *p_line);

instruction* init_instruction_memory(unsigned len);
void destroy_instruction_memory(instruction *instr_mem);
void print_assembled_structure(const instruction *instr_mem, unsigned len);



#endif /* _ASSEMBLER_H_ */