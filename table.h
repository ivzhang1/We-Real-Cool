#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include "al.h"
#include "qs.h"

#define KEY 52013

#define SEG_SIZE 0xffff
#define BUFFER_SIZE 1024
#define DATA_SIZE 256

#define AVG 0
#define MAX 1
#define MIN 2

struct table {
    char name[DATA_SIZE];
    int *types;
    char **names;
    int num_columns;
    struct array_list al;
};

struct database {
    struct table *tables;
    int num_tables;
};

void print_row(struct table *t, int index);

void print_table(struct table *t);

//removes begining and ending spaces
char *rs(char *str);

int hs(char *str);

void create_table(char *str, struct database *db);

void read_table(char *str, struct database *db);

void insert(char *str, struct database *db);

void delete(char *str, struct database *db);

void drop(char *str, struct database *db);

void sort(char *str, struct database *db);

void execute(char *str, struct database *db);
