#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "al.h"

#define BUFFER_SIZE 1024
#define DATA_SIZE 256

// #define C "create"
// #define R "read"
// #define U "update"
// #define DE "delete"
// #define DR "drop"
// #define S "source"

#define AVG 0
#define MAX 1
#define MIN 2

#define INT 0
#define DOUBLE 1
#define STRING 2

struct table {
    char name[DATA_SIZE];
    int *types;
    char **names;
    int num_columns;
    struct array_list al;
};

void print_table(struct table *t);

void prompt(char display[], char input[]);

void create_table(char *str);

void read_table(char *str);

void insert(char *str);

void delete(char *str);

void drop(char *str);

void execute(char *str);
