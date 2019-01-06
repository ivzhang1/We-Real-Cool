#include <stdio.h>
#include <stdlib.h>

struct array_list {
    int *arr;
    int size;
    int max_size;
};

void construct(struct array_list *al);

void print_arr(struct array_list *al);

void expand(struct array_list *al);

int get(struct array_list *al, int index);

int set(struct array_list *al, int index, int new_val);

void add_last(struct array_list *al, int new_val);

void add(struct array_list *al, int index, int new_val);

int delete(struct array_list *al, int index);
