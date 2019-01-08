#include <stdio.h>
#include <stdlib.h>

#include "al.h"

void swap(struct array_list *al, int a, int b);

int partition(struct array_list *al, int lo, int hi, int col, int type);

void quick_sort_h(struct array_list *al, int lo, int hi, int col, int type);

void quick_sort(struct array_list *al, int col, int type);
