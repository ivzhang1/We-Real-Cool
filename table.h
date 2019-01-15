#ifndef FINAL_TABLE_H
#define FINAL_TABLE_H

#include "uthash.h"
#include "commons.h"

struct column {
    char name[DATA_SIZE];
    unsigned char tags;
    unsigned char type;
    void *data;
    UT_hash_handle hh;
};

struct table {
    char name[DATA_SIZE];
    struct column *columns;
    UT_hash_handle hh;
};


#endif //FINAL_TABLE_H
