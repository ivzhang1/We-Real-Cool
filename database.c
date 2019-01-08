
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include "database.h"
#include "commons.h"

int main(int argc, char * argv[]) {
    char *port = check_input(argc, argv, 1, "./database <port>")[0];
    int listening_sd = server_setup(port);
    int sem_id = error_check("creating semaphore", semget(KEY, 1, IPC_CREAT | 0644));
    while (1) {
        int client_sd = get_client(listening_sd);
        if (!fork()) { // child
            close(listening_sd);
            fulfill(client_sd, sem_id); // do stuff!
            exit(0);
        } else close(client_sd); // parent
        return 0;
    }
}

int server_setup(char *port) {
    struct addrinfo *results;
    int listening_sd = get_results_and_socket(NULL, port, &results);
    error_check("binding", bind(listening_sd, results->ai_addr, results->ai_addrlen));
    freeaddrinfo(results);
    error_check("listening", listen(listening_sd, 100));
    return listening_sd;
}

int get_client(int listening_sd) {
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    int client_sd = error_check("accepting", accept(listening_sd, (struct sockaddr *) &client_address, &sock_size));
    printf("[server %d] connected to client\n", getpid());
    return client_sd;
}

void fulfill(int client_sd, int sem_id) {

}