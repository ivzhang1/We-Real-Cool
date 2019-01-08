#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef AL_H
#define AL_H

#define INT 0
#define DOUBLE 1
#define STRING 2

union value {
    int integer;
    double decimal;
    char *string;
};

struct row {
    union value *values;
};

struct array_list {
    struct row *rows;
    int num_rows;
    int max_size;
};

void construct(struct array_list *al);

void expand(struct array_list *al);

struct row get(struct array_list *al, int index);

struct row set(struct array_list *al, int index, struct row new_row);

void add_last(struct array_list *al, struct row new_row);

void add(struct array_list *al, int index, struct row new_row);

struct row remov(struct array_list *al, int index);

#endif
