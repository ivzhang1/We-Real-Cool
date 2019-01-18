#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include "commons.h"

char* my_getpass (char *line, int n, FILE *stream);

int create_account();

int delete_account();

