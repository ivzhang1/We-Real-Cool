#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "general_methods.h"

int main(){
  char* d = "where";
  char* str = "Print this where that";

  printf("%s\n", strsep_string(&str, d));

  
}
