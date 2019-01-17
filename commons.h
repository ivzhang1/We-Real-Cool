#ifndef FINAL_COMMONS_H
#define FINAL_COMMONS_H

#include <netdb.h>

#define BUFFER_SIZE 1024
#define DATA_SIZE 256

#define INT 0
#define DBL 1
#define STR 2

void prompt(char *display, char *input);

void check_input(int argc, int num_inputs, char *format);

int error_check(char *msg, int retval);

void get_results(char *ip, char *port, struct addrinfo **results_list);

int empty_string(char *string);

char *next_word(char **string);

char *multichar_strsep(char **string, char *delim);

int is_alpha(char *str);

#endif //FINAL_COMMONS_H
