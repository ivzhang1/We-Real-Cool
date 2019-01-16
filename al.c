#include "al.h"

void construct(struct array_list *al){
    al->rows = calloc(10, sizeof(struct row *));
    al->num_rows = 0;
    al->max_size = 10;
}

void expand(struct array_list *al){
    al->rows = realloc(al->rows, al->max_size * 2 * sizeof(struct row *));
    al->max_size *= 2;
}

struct row *get(struct array_list *al, int index){
    if (index < 0 || index >= al->num_rows){
        printf("Index Out of Bound\n");
        return 0;
    }
    return al->rows[index];
}

struct row *set(struct array_list *al, int index, struct row *new_row){
    struct row *temp = al->rows[index];
    al->rows[index] = new_row;
    // free(new_row);
    return temp;
}

void add(struct array_list *al, struct row *new_row){
    if (al->num_rows >= al->max_size)
        expand(al);

    al->rows[ al->num_rows ] = new_row;
    // free(new_row);
    al->num_rows ++;
}

struct row *remov(struct array_list *al, int index){
    struct row *temp = al->rows[index];
    for(int i = index; i < al->num_rows - 1; i ++)
        al->rows[i] = al->rows[i + 1];
    al->num_rows --;
    return temp;
}
