#include "al.h"

void construct(struct array_list *al){
    al->arr = calloc(10, sizeof(int));
    al->size = 0;
    al->max_size = 10;
}

void print_arr(struct array_list *al){
    for (int i = 0; i < al->size; i ++)
        printf("%d\n", al->arr[i]);
}

void expand(struct array_list *al){
    al->arr = realloc(al->arr, al->max_size * 2 * sizeof(int));
    al->max_size *= 2;
}

int get(struct array_list *al, int index){
    return al->arr[index];
}

int set(struct array_list *al, int index, int new_val){
    int temp = al->arr[index];
    al->arr[index] = new_val;
    return temp;
}

void add_last(struct array_list *al, int new_val){
    if (al->size >= al->max_size)
        expand(al);

    al->arr[ al->size ] = new_val;
    al->size ++;
}

void add(struct array_list *al, int index, int new_val){
    if (al->size >= al->max_size)
        expand(al);

    for (int i = al->size; i > index; i --)
        al->arr[i] = al->arr[i - 1];

    al->arr[index] = new_val;
    al->size ++;
}

int delete(struct array_list *al, int index){
    int temp = al->arr[index];
    for(int i = index; i < al->size - 1; i ++)
        al->arr[i] = al->arr[i + 1];
    al->size --;
    return temp;
}
