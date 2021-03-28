#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "assembler.h"

#define DEBUG 0

void print_usage();
int parse_input(int argc, char **argv, char** input_file);


int main(int argc, char **argv)
{
    char *asm_file_name;
    linked_list *parsed_program, *instruction_memory;

    if (parse_input(argc, argv, &asm_file_name)) {
        print_usage();
        exit(EXIT_FAILURE);
    }

    parsed_program = init_parse_list();
    parse_instructions(&parsed_program, asm_file_name);

#if DEBUG
    print_parsed_structure(parsed_program);
#endif

    instr_mem_from_asm_parse_list(&instruction_memory, parsed_program);
    destroy_parse_list(&parsed_program);

#if DEBUG
    //print_assembled_structure(parsed_program);
#endif

    destroy_instruction_memory(&instruction_memory);

    return 0;
}


void print_usage()
{
    printf("./risc-v_emulator [ -h  | input_file ]\n");
    printf("\t-h\t\t: Print the usage statement.\n");
    printf("\tinput_file\t: The RISC-V assembly file.\n");
}

int parse_input(int argc, char **argv, char** input_file)
{
    if (argc != 2)
        return 1;
    else if (!strcmp( argv[1], "-h" ))
        return 1;

    *input_file = (char*)malloc( strlen(argv[1]) );
    strcpy(*input_file, argv[1]);

    return 0;
}
