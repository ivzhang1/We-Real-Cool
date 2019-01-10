#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "general_methods.h"

char *custom_strsep(char ** string, char * delim) {
    size_t len = strlen(delim);
    char *p = *string;
    while (strncmp(p++, delim, len));
    p--;
    for (int i = 0; i < len; ++i) *(p++) = 0;
    char *temp = *string;
    *string = p;
    return temp;
}
