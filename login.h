#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include "table.h"
char* my_getpass (char *line, int n, FILE *stream);

int login();

int registerr();

int log_or_reg();
