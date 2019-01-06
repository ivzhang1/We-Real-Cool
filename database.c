#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include "database.h"
#include "commons.h"

int main(int argc, char * argv[]) {
    // check input
    if (argc == 1) {
        printf("Format: ./database <port>\n");
        exit(0);
    }
    char * port = calloc(1, 10);
    strcpy(port, argv[1]);
    // setup socket on port
    struct addrinfo * results;
    int sd = get_results_and_socket(NULL, port, &results);
    error_check(bind(sd, results->ai_addr, results->ai_addrlen));
    error_check(listen(sd, 100));
    // setup semaphore
    int sem_id = error_check(semget(KEY, 1, IPC_CREAT | 0644));
    // server time
    while (1) {
        socklen_t sock_size;
        struct sockaddr_storage client_address;
        printf("waiting...\n");
        int client_socket = accept(sd, (struct sockaddr *)&client_address, &sock_size);
        printf("connected\n");
        /*
         * Actual stuff here
         * */
        return 0;
    }
}
