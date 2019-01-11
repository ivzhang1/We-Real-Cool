#ifndef FINAL_TABLE_H
#define FINAL_TABLE_H

#include "commons.h"

#define INT 0
#define DOUBLE 1
#define STRING 2
#define COLUMN 3

struct column {
    char name[DATA_SIZE];
    unsigned char tags;
    unsigned char type;
    void **data;
};

struct table {
    char name[DATA_SIZE];
    struct column *columns;
};

struct bucket {
    unsigned long hash;
    void **values;
    unsigned char type;
};

struct hash_table {
    struct bucket *buckets;
    unsigned long num_buckets;
};

unsigned long hash(unsigned char *str);

void *lookup(struct hash_table *ht, char *str);


#endif //FINAL_TABLE_H
