#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "assembler.h"


static unsigned int verbose = 0;

void print_usage();
int parse_input(int argc, char **argv, char** input_file);


int main(int argc, char **argv)
{
    unsigned int num_instrs;
    char *asm_file_name;
    linked_list *parsed_program;
    instruction *instruction_memory;

    if (parse_input(argc, argv, &asm_file_name)) {
        print_usage();
        exit(EXIT_FAILURE);
    }

    parsed_program = init_parse_list();
    parse_instructions(&parsed_program, asm_file_name, &num_instrs);

    if (verbose)
        print_parsed_structure(parsed_program);

    instruction_memory = init_instruction_memory(num_instrs);
    instr_mem_from_asm_parse_list(instruction_memory, parsed_program, num_instrs);
    destroy_parse_list(&parsed_program);

    if (verbose)
        print_assembled_structure(instruction_memory, num_instrs);

    destroy_instruction_memory(instruction_memory, num_instrs);

    return 0;
}


void print_usage()
{
    printf("./risc-v_emulator [-h | -v] input_file\n");
    printf("\tinput_file\t: The RISC-V assembly file.\n");
    printf("optional arguments:\n");
    printf("\t-h\t\t: Print the usage statement.\n");
    printf("\t-v\t\t: Verbose mode. Print the parsed and assembled instructions.\n");
    
}

int parse_input(int argc, char **argv, char** input_file)
{
    if ((argc != 2) && (argc != 3)) {
        return 1;
    }
    else if (!strcmp( argv[1], "-h" ))
        return 1;
    else if (!strcmp( argv[1], "-v" ))
        verbose = 1;

    *input_file = (char*)malloc( strlen(argv[argc - 1]) );
    strcpy(*input_file, argv[argc - 1]);

    return 0;
}
