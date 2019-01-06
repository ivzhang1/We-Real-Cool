#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include "client.h"
#include "commons.h"

int main(int argc, char * argv[]) {
    // check input
    if (argc < 3) {
        printf("Format: ./client <ip> <port>\n");
        exit(0);
    }
    char * ip = calloc(1, 100);
    char * port = calloc(1, 10);
    strcpy(ip, argv[1]);
    strcpy(port, argv[2]);
    // connect to db
    struct addrinfo* results;
    int sd = get_results_and_socket(ip, port, &results);
    int c = error_check(connect(sd, results->ai_addr, results->ai_addrlen));
    printf("connected\n");
    while (1) {
        /*
         * Check if sourcing
         * Send to database
         * */
        return 0;
    }
}
