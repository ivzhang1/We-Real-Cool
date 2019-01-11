#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *rs(char *str){
    while (*str && !strncmp(str, " ", 1))
        str ++;
    char *dummy = str + strlen(str) - 1;
    while (!strncmp(dummy, " ", 1))
        dummy --;
    *(dummy + 1) = 0;
    return str;
}

char *rq(char *str){
    if (strncmp(str, "\"", 1))
        return 0;
    str ++;
    char *dummy = str;
    while (*dummy){
        if (!strncmp(dummy, "\"", 1)){
            *dummy = 0;
            return str;
        }
        dummy ++;
    }
    return 0;
}

int main(){

  char str[] = "\" smd\"    ";

  char *s = rq(str);
  printf("[%s]\n", s);

  return 0;
}
