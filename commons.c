#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "commons.h"

void prompt(char *display, char *input) {
    printf("%s", display);
    fgets(input, BUFFER_SIZE, stdin);
    input[strlen(input) - 1] = '\0'; // strip newline
}

int error_check(int retval) {
    if (retval == -1) {
        printf("%s\n", strerror(errno));
        exit(0);
    }
    return retval;
}

int get_results_and_socket(char *ip, char *port, struct addrinfo **results_list) {
    struct addrinfo * hints;
    hints = (struct addrinfo *) calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_UNSPEC;
    hints->ai_socktype = SOCK_STREAM; // TCP socket
    hints->ai_flags = AI_PASSIVE; // only needed on server
    int status = getaddrinfo(ip, port, hints, results_list);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    return error_check(socket((*results_list)->ai_family, (*results_list)->ai_socktype, (*results_list)->ai_protocol));
}