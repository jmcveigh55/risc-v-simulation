#ifndef _UTILITIES_H_
#define _UTILITIES_H_


typedef struct linked_list_s linked_list;
struct linked_list_s {
    void *item;
    linked_list *next;
};

void linked_list_add_head(linked_list **L, void *item);
void linked_list_reverse(linked_list **L);
unsigned is_in(const char *item, const char *array[]);
char* to_lower(char *input_string);
void print_bin(unsigned number);


#endif /* _UTILITIES_H_ */