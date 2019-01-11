#ifndef FINAL_TABLE_H
#define FINAL_TABLE_H

#include "commons.h"

#define INT 0
#define DOUBLE 1
#define STRING 2

struct column {
    char name[DATA_SIZE];
    unsigned char tags;
    unsigned char type;
    unsigned char *data;
};

struct table {
    char name[DATA_SIZE];
    struct column *columns;
};

#endif //FINAL_TABLE_H
