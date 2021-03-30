#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include <stdint.h>

#include "utilities.h"
#include "parser.h"

typedef uint32_t instruction;
/*
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
*/

void instr_mem_from_asm_parse_list(instruction *instr_mem, linked_list *p_list, unsigned int len);
static instruction instr_from_asm_parse(parse *p_line);
static void convert_R_format(instruction *instr, char *instr_meta, parse *p_line);
static void convert_I_format(instruction *instr, char *instr_meta, parse *p_line);
static void convert_S_format(instruction *instr, char *instr_meta, parse *p_line);
static void convert_B_format(instruction *instr, char *instr_meta, parse *p_line);
static void convert_U_format(instruction *instr, char *instr_meta, parse *p_line);
static void convert_J_format(instruction *instr, char *instr_meta, parse *p_line);

instruction* init_instruction_memory(unsigned int len);
void destroy_instruction_memory(instruction *instr_mem, unsigned int len);
void print_assembled_structure(const instruction *instr_mem, unsigned int len);



#endif /* _ASSEMBLER_H_ */