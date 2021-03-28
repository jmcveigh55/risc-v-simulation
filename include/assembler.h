#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include "utilities.h"
#include "parser.h"

typedef struct instruction_s instruction;
struct instruction_s {
    char type;
    int original[32];
    int opcode[7];
    int imm[20];
    int rs1[5];
    int rs2[5];
    int rd[5];
    int funct3[3];
    int funct7[7];
};


void instr_mem_from_asm_parse_list(linked_list **instr_mem, linked_list *p_list);
static instruction* instr_from_asm_parse(parse *p_line);
static void convert_R_format(instruction *instr_line, parse *p_line);
static void convert_I_format(instruction *instr_line, parse *p_line);
static void convert_S_format(instruction *instr_line, parse *p_line);
static void convert_B_format(instruction *instr_line, parse *p_line);
static void convert_J_format(instruction *instr_line, parse *p_line);

linked_list* init_instruction_memory();
void destroy_instruction_memory(linked_list **instr_mem);
void print_instruction_structure(const linked_list *instr_mem);


#endif /* _ASSEMBLER_H_ */