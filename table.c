#include <string.h>
#include "table.h"

// djb2 string hash function developed by dan bernstein
unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) hash = ((hash << 5) + hash) + c;
    return hash;
}

void *lookup(struct hash_table *ht, char *str) {
    unsigned long h = hash((unsigned char *) str);
    struct bucket b = ht->buckets[h];
    int i = 0;
    for (; strcmp((char *) b.values[i], str); i++);
    return b.values[i];
}

