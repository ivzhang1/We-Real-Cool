#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "commons.h"

void prompt(char *display, char *input) {
    printf("%s\n", display);
    fgets(input, BUFFER_SIZE, stdin);
    input[strlen(input) - 1] = '\0'; // strip newline
}

void check_input(int argc, int num_inputs, char *format) {
    if (argc - 1 != num_inputs) {
        printf("Format: [%s]\n", format);
        exit(1);
    }
}

int error_check(char *msg, int retval) {
    if (retval == -1) {
        printf("%s error: [%s]\n", msg, strerror(errno));
        exit(1);
    }
    return retval;
}

void get_results(char *ip, char *port, struct addrinfo **results_list) {
    struct addrinfo *hints;
    hints = (struct addrinfo *) calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_UNSPEC;
    hints->ai_socktype = SOCK_STREAM; // TCP socket
    if (!ip) hints->ai_flags = AI_PASSIVE; // only needed on server
    int status = getaddrinfo(ip, port, hints, results_list);
    freeaddrinfo(hints);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: [%s]\n", gai_strerror(status));
        exit(1);
    }
}

int empty_string(char *string) {
    for (; *string == ' '; string++);
    return *string == 0;
}

char *next_word(char **string) {
    for (; **string != ' '; (*string)++);
    return strsep(string, " ");
}

int is_alpha(char *str) {
    for (; *str && ((('a' <= *str) && (*str <= 'z')) || (('A' <= *str) && (*str <= 'Z'))); str++);
    return *str == 0;
}

char *multichar_strsep(char **string, char *delim) {
    size_t len = strlen(delim);
    char *p = *string;
    while (strncmp(p++, delim, len)) p++;
    for (int i = 0; i < len; ++i) *(p++) = 0;
    char *temp = *string;
    *string = p;
    return temp;
}