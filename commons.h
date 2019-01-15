#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 1024
#define DATA_SIZE 256

void prompt(char *display, char *input);

void check_input(int argc, int num_inputs, char *format);

int error_check(char *msg, int retval);

void get_results(char *ip, char *port, struct addrinfo **results_list);
