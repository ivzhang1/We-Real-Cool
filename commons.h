#ifndef FINAL_COMMONS_H
#define FINAL_COMMONS_H

#include <netdb.h>

#define BUFFER_SIZE 1024
#define DATA_SIZE 256

void prompt(char *display, char *input);

void check_input(int argc, int num_inputs, char *format);

int error_check(char *msg, int retval);

void get_results(char *ip, char *port, struct addrinfo **results_list);

char *multichar_strsep(char **string, char *delim);

#endif //FINAL_COMMONS_H
