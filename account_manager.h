#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

struct account{
  char username[256];
  char password[256];
};

char* my_getpass (char *line, int n, FILE *stream);

int create_account();

void delete_account();

