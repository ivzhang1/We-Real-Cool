#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include "al.h"
#include "qs.h"

#define SEG_SIZE 0xffff
#define BUFFER_SIZE 1024
#define DATA_SIZE 256

#define AVG 0
#define MAX 1
#define MIN 2

#define OR 0
#define AND 1

#define PRIMARY_KEY 1
#define AUTO_INC 2
#define DEFAULT 3

#define GREATER 1
#define EQUAL 0
#define LESS -1

struct table {
    char name[DATA_SIZE];
    int *tags;
    union value *defaults;
    int *types;
    char **col_names;
    int num_columns;
    struct array_list *al;
};

struct database {
    struct table *tables;
    int num_tables;
};

char *str_row(struct table *t, int index);

char *str_table(struct table *t, int *read);

//removes begining and ending spaces
char *rs(char *str);

//removes quotation marks
char *rq(char *str);


void set_val(struct table *t, int row, int col, int type, int tag, char *new_val);

struct table *get_table(char *tname, struct database *db);

void initialize(struct table *t, int i);

char *set_tag(struct table *t, int j, char *tag);

char *create_table(char *str, struct database *db);

int index_of(struct table *t, char *col);

int *bool_and(struct table *t, char **piece, int c);

int *bool_or(struct table *t, char **piece, int c);

char *read_spec(struct table *t, char *expr);

char *insert(char *str, struct database *db);

char *delete(char *str, struct database *db);

char *drop(char *str, struct database *db);

char *sort(char *str, struct database *db);

char *update(char *str, struct database *db);

char *execute(char *str, struct database *db);
