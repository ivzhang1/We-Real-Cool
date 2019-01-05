#include <stdio.h>
#include <stdlib.h>

struct array_list {
  int *arr;
  int size;
  int max_size;
};

void construct(struct array_list *al){
  al->arr = calloc(10, sizeof(int));
  al->size = 0;
  al->max_size = 10;
}

void print_arr(struct array_list *al){
  for (int i = 0; i < al->size; i ++)
    printf("%d\t", al->arr[i]);
  printf("\n");
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

int remov(struct array_list *al, int index){
  int temp = al->arr[index];
  for(int i = index; i < al->size - 1; i ++)
    al->arr[i] = al->arr[i + 1];
  al->size --;
  return temp;
}

int main(){
  struct array_list *al = malloc(sizeof(struct array_list));
  construct(al);
  add_last(al, 7);
  add_last(al, 1);
  add_last(al, 5);
  add_last(al, 12);
  add_last(al, 3);
  print_arr(al);

  add(al, 0, 0);
  add(al, 2, 6);
  add(al, 7, 10);
  print_arr(al);

  set(al, 1, 8);
  set(al, 6, 2);
  print_arr(al);

  printf("%d\n", get(al, 5));
  printf("%d\n", remov(al, 5));
  print_arr(al);
  return 0;
}
