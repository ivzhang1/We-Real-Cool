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
