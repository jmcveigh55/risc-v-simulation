#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "assembler.h"
#include "registers.h"

void instr_mem_from_asm_parse_list(instruction *instr_mem, linked_list *p_list, unsigned len)
{
	unsigned i;

	p_list = p_list->next; /* skip head */
	for (i = 0; i < len; i++)
	{
		instr_mem[i] = instr_from_asm_parse((parse *)p_list->item);
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
	if (!instrs_fd)
	{
		perror("instr_from_asm_parse: Error opening instruction reference file");
		exit(EXIT_FAILURE);
	}

	test_meta = NULL;
	getline(&test_meta, &len, instrs_fd); /* Skip header */
	while (getline(&test_meta, &len, instrs_fd) != -1)
	{
		test_meta[strlen(test_meta) - 1] = '\0';
		test_operand = strdup(test_meta);
		if (!test_operand)
		{
			perror("instr_from_asm_parse: Failed to strdup test_operand: ");
			exit(EXIT_FAILURE);
		}
		strtok(test_operand, ",");

		if (!strcmp(p_line->operand, test_operand))
			break;
	}

	fclose(instrs_fd);
	free(test_operand);

	switch (p_line->format)
	{
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

	unsigned opcode = (int)strtol(strtok(NULL, ","), NULL, 2);
	unsigned rd = reg_index(p_line->parameters[0]);
	unsigned funct3 = (int)strtol(strtok(NULL, ","), NULL, 2);
	unsigned rs1 = reg_index(p_line->parameters[1]);
	unsigned rs2 = reg_index(p_line->parameters[2]);
	unsigned funct7 = (int)strtol(strtok(NULL, ","), NULL, 2);

	*instr = 0;
	*instr |= opcode;
	*instr |= rd << 7;
	*instr |= funct3 << (7 + 5);
	*instr |= rs1 << (7 + 5 + 3);
	*instr |= rs2 << (7 + 5 + 3 + 5);
	*instr |= funct7 << (7 + 5 + 3 + 5 + 5);
}

static void convert_I_format(instruction *instr, char *instr_meta, parse *p_line)
{
	strtok(instr_meta, ","); /* skip operand */

	unsigned opcode = (int)strtol(strtok(NULL, ","), NULL, 2);
	unsigned rd = reg_index(p_line->parameters[0]);
	unsigned funct3 = (int)strtol(strtok(NULL, ","), NULL, 2);
	unsigned rs1;
	int16_t imm;

	if (strchr(p_line->parameters[1], '('))
	{
		char *temp = strdup(p_line->parameters[1]);
		if (!temp)
		{
			perror("convert_I_format: Failed to strdup temp: ");
			exit(EXIT_FAILURE);
		}

		imm = strtol(strtok(temp, "("), NULL, 10);
		rs1 = reg_index(strtok(NULL, ")"));

		free(temp);
	}
	else
	{
		rs1 = reg_index(p_line->parameters[1]);
		imm = strtol(p_line->parameters[2], NULL, 10);
	}

	*instr = 0;
	*instr |= opcode;
	*instr |= rd << 7;
	*instr |= funct3 << (7 + 5);
	*instr |= rs1 << (7 + 5 + 3);
	*instr |= imm << (7 + 5 + 3 + 5);
}

static void convert_S_format(instruction *instr, char *instr_meta, parse *p_line)
{
	strtok(instr_meta, ","); /* skip operand */

	unsigned opcode = (int)strtol(strtok(NULL, ","), NULL, 2);
	int16_t imm;
	unsigned funct3 = (int)strtol(strtok(NULL, ","), NULL, 2);
	unsigned rs1;
	unsigned rs2 = reg_index(p_line->parameters[0]);

	if (strchr(p_line->parameters[1], '('))
	{
		char *temp = strdup(p_line->parameters[1]);
		if (!temp)
		{
			perror("convert_S_format: Failed to strdup temp: ");
			exit(EXIT_FAILURE);
		}

		imm = strtol(strtok(temp, "("), NULL, 10);
		rs1 = reg_index(strtok(NULL, ")"));

		free(temp);
	}
	else
	{
		rs1 = reg_index(p_line->parameters[1]);
		imm = strtol(p_line->parameters[2], NULL, 10);
	}

	*instr = 0;
	*instr |= opcode;
	*instr |= (imm & 0x1F) << 7; /* imm[0:4] */
	*instr |= funct3 << (7 + 5);
	*instr |= rs1 << (7 + 5 + 3);
	*instr |= rs2 << (7 + 5 + 3 + 5);
	*instr |= (imm & 0xFE0) << 20; /* imm[5:11] ( >> 5 << 7 + 5 + 3 + 5 + 5) */
}

static void convert_B_format(instruction *instr, char *instr_meta, parse *p_line)
{
	strtok(instr_meta, ","); /* skip operand */

	unsigned opcode = (int)strtol(strtok(NULL, ","), NULL, 2);
	int16_t imm = strtol(p_line->parameters[2], NULL, 10);
	unsigned funct3 = (int)strtol(strtok(NULL, ","), NULL, 2);
	unsigned rs1 = reg_index(p_line->parameters[0]);
	unsigned rs2 = reg_index(p_line->parameters[1]);

	*instr = 0;
	*instr |= opcode;
	*instr |= (imm & 0x800) >> 4; /* imm[11]   ( >> 11 << 7 ) */
	*instr |= (imm & 0x1E) << 7;  /* imm[1:4]  ( >> 1 << 7 + 1 ) */
	*instr |= funct3 << (7 + 1 + 4);
	*instr |= rs1 << (7 + 1 + 4 + 3);
	*instr |= rs2 << (7 + 1 + 4 + 3 + 5);
	*instr |= (imm & 0x7E0) >> 20;	/* imm[10:5] ( >> 5 << 7 + 1 + 4 + 3 + 5 + 5 ) */
	*instr |= (imm & 0x1000) << 19; /* imm[12]   ( >> 12 << 7 + 1 + 4 + 3 + 5 + 5 + 6 ) */
}

static void convert_U_format(instruction *instr, char *instr_meta, parse *p_line)
{
	strtok(instr_meta, ","); /* skip operand */

	unsigned opcode = (int)strtol(strtok(NULL, ","), NULL, 2);
	unsigned rd = reg_index(p_line->parameters[0]);
	int16_t imm = strtol(p_line->parameters[2], NULL, 10);

	*instr = 0;
	*instr |= opcode;
	*instr |= rd << 7;
	*instr |= (imm & 0xFFFFF000); /* imm[31:12] ( >> 12 << 7 + 5 ) */
}

static void convert_J_format(instruction *instr, char *instr_meta, parse *p_line)
{
	strtok(instr_meta, ","); /* skip operand */

	unsigned opcode = (int)strtol(strtok(NULL, ","), NULL, 2);
	unsigned rd = reg_index(p_line->parameters[0]);
	int16_t imm = strtol(p_line->parameters[1], NULL, 10);

	*instr = 0;
	*instr |= opcode;
	*instr |= rd << 7;
	*instr |= (imm & 0xFF000);	  /* imm[19:12] ( >> 12 << 7 + 5 ) */
	*instr |= (imm & 0x800) << 9;	  /* imm[11]    ( >> 11 << 7 + 5 + 8) */
	*instr |= (imm & 0x7FE) << 20;	  /* imm[10:1]  ( >> 1  << 7 + 5 + 8 + 1) */
	*instr |= (imm & 0x100000) << 12; /* imm[20]  ( >> 20  << 7 + 5 + 8 + 1 + 11) */
}

instruction *init_instruction_memory(unsigned len)
{
	instruction *head;

	head = (instruction *)malloc(len * sizeof(*head));
	if (!head)
	{
		perror("init_instruction_memory: Failed to malloc head: ");
		exit(EXIT_FAILURE);
	}

	return head;
}

void destroy_instruction_memory(instruction *instr_mem)
{
	free(instr_mem);
}

void print_assembled_structure(const instruction *instr_mem, unsigned len)
{
	unsigned i;

	for (i = 0; i < len; i++)
	{
		printf("instruction_memory[%d]:\t0x%x    \t0b", i, instr_mem[i]);
		print_bin(instr_mem[i]);
		printf("\n");
	}
}
