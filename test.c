#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strsep_str(char **str, char *delim){
  char *temp = *str;
  int len = strlen(delim);
  char *found = 0;
  int ctr = 0;
  while (**str){
    if ( !strncmp(*str, delim, len) ){
      found = *str;
      *str += len;
      break;
    }
    (*str) ++;
    ctr ++;
  }
  if (found)
    for (int i = 0; i < len; i ++){
      *found = 0;
      found ++;
    }
  return temp;
}

int main(){

  char *str = malloc(100);
  strcpy(str, "delete oof wher safafa f");

  char *s = strsep_str(&str, "where");
  printf("[%s]\n", s);
  printf("[%s]\n", str);

  return 0;
}
