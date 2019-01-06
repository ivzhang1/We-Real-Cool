#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define DATA_SIZE 256

#define C "create"
#define R "read"
#define U "update"
#define DE "delete"
#define DR "drop"
#define S "source"

#define AVG 0
#define MAX 1
#define MIN 2

#define INT 0
#define DOUBLE 1
#define STRING 2

struct create_column {
    int type;
    char *column_name;
    int tags[2];
};

struct create {
    char *name;
    int num_columns;
    struct create_column* columns;
};

union value {
    int type;
    int integer;
    double decimal;
    char *string;
};

struct insert {
    char *table_name;
    int num_columns;
    union value* values;
};

struct delete {
    char *table_name;
    char *column_name;
    union value val;
};

void prompt(char display[], char input[]);
