#include <stdio.h>
#include <stdlib.h>

void print_arr(int* arr, int size){
  for (int i = 0; i < size; i ++)
    printf("%d\t", arr[i]);
  printf("\n");
}

void swap(int* arr, int a, int b){
  int temp = arr[a];
  arr[a] = arr[b];
  arr[b] = temp;
}

int partition(int* arr, int lo, int hi){
  int v = arr[lo];

  swap(arr, lo, hi);
  int s = lo;

  for (int i = lo; i < hi; i ++)
    if (arr[i] <= v){
      swap(arr, i, s);
      s ++;
    }
  swap(arr, s, hi);

  return s;
}

void quick_sort_h(int* arr, int lo, int hi){
  if (lo < hi){
    int pivot = partition(arr, lo, hi);
    quick_sort_h(arr, lo, pivot - 1);
    quick_sort_h(arr, pivot + 1, hi);
  }
}

void quick_sort(int* arr, int size){
  quick_sort_h(arr, 0, size - 1);
}

int main(){
  int arr[] = {7,1,5,12,3,6,8,9,2,10};
  // printf("%d\n", partition(arr, 0, 9));
  // printf("%d\n", partition(arr, 1, 4));
  // printf("%d\n", partition(arr, 6, 9));
  quick_sort(arr, sizeof(arr) / sizeof(arr[0]));
  print_arr(arr, sizeof(arr) / sizeof(arr[0]));
  return 0;
}
