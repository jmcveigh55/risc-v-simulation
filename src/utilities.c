#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utilities.h"


void linked_list_add_head(linked_list **head, void *item)
{
    linked_list *new;
    new = (linked_list*)malloc(sizeof(*new));
    new->item = item;

    new->next = (*head)->next; /* Line causing seg fault */
    (*head)->next = new;
}

void linked_list_reverse(linked_list **head)
{
    linked_list* prev;
    linked_list* current = (*head)->next;
    linked_list* next;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    (*head)->next = prev;
}

unsigned int is_in(const char *item, const char *array[])
{
    int i;
    for(i=0; array[i]; i++)
        if (!strcmp(item, array[i]))
        return 1;
    return 0;
}

char* to_lower(char *input_string)
{
    char *p;

    /* Raise each by 0x60, restrict to ASCII */
    for(p = input_string; *p; ++p)
        *p = *p > 0x40 && *p < 0x5b ? *p | 0x60 : *p;

    return input_string;
}

void print_bin(unsigned int number)
{
    unsigned int i;

    for (i = 1 << 31; i > 0; i = i / 2)
        (number & i) ? printf("1") : printf("0");
}
