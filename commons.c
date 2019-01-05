#include <stdio.h>
#include <string.h>
#include "commons.h"

void prompt(char display[], char input[]) {
    printf("%s", display);
    fgets(input, BUFFER_SIZE, stdin);
    input[strlen(input) - 1] = '\0'; // strip newline
}