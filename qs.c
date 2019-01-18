#include "qs.h"

void swap(struct array_list *al, int a, int b){
    set(al, a, set(al, b, get(al, a)));
}

int partition(struct array_list *al, int lo, int hi, int col, int type){
    union value v = get(al, lo)->values[col];
    swap(al, lo, hi);
    int s = lo;

    if (type == INT){
        for (int i = lo; i < hi; i ++)
            if (get(al, i)->values[col].integer < v.integer){
                swap(al, i, s);
                s ++;
            }
    }
    else if (type == DOUBLE){
        for (int i = lo; i < hi; i ++)
            if (get(al, i)->values[col].decimal < v.decimal){
                swap(al, i, s);
                s ++;
            }
    }
    else if (type == STRING){
        for (int i = lo; i < hi; i ++)
            if ( strcmp(get(al, i)->values[col].string, v.string) < 0){
                swap(al, i, s);
                s ++;
            }
    }

    swap(al, s, hi);
    return s;
}

void quick_sort_h(struct array_list *al, int lo, int hi, int col, int type){
    if (lo < hi){
        int pivot = partition(al, lo, hi, col, type);
        quick_sort_h(al, lo, pivot - 1, col, type);
        quick_sort_h(al, pivot + 1, hi, col, type);
    }
}

void quick_sort(struct array_list *al, int col, int type){
    quick_sort_h(al, 0, al->num_rows - 1, col, type);
}
