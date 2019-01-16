
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include "database.h"
#include "commons.h"

int main(int argc, char * argv[]) {
    check_input(argc, 1, "./database <port>");
    char *port = argv[1];
    int listening_sd = server_setup(port);
    int sem_id = sem_setup();
    while (1) {
        int client_sd = get_client(listening_sd);
        if (!fork()) { // child
            close(listening_sd);
            fulfill(client_sd, sem_id); // do stuff!
            close(client_sd);
            exit(0);
        } else close(client_sd); // parent
    }
}

int sem_setup() {
    int sem_id = error_check("creating semaphore", semget(KEY, 1, IPC_CREAT | 0644));
    union semun ctl = {.val = 1};
    semctl(sem_id, 0, SETVAL, ctl);
    return sem_id;
}

int server_setup(char *port) {
    struct addrinfo *results, *p;
    get_results(NULL, port, &results);
    int listening_sd;
    for (p = results; p != NULL; p = p->ai_next) {
        if ((listening_sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;
        if (bind(listening_sd, p->ai_addr, p->ai_addrlen) == -1) {
            close(listening_sd);
            continue;
        }
        break;
    }
    if (!p) {
        printf("[server %d] failed to bind", getpid());
        exit(1);
    }
    error_check("listening", listen(listening_sd, 10));
    freeaddrinfo(results);
    return listening_sd;
}

int get_client(int listening_sd) {
    struct sockaddr_storage client_address;
    socklen_t sock_size = sizeof(client_address);
    int client_sd = error_check("accepting",
                                accept(listening_sd, (struct sockaddr *) &client_address, &sock_size));
    printf("[server %d] connected to client\n", getpid());
    return client_sd;
}

void fulfill(int client_sd, int sem_id) {
    char query_buf[BUFFER_SIZE];
    error_check("receiving", (int) recv(client_sd, query_buf, sizeof(query_buf), 0));
    printf("[subserver %d] received query, processing\n", getpid());
    struct sembuf *sbuf = malloc(sizeof(struct sembuf));
    sbuf->sem_op = -1;
    sbuf->sem_num = 0;
    sbuf->sem_flg = SEM_UNDO;
    semop(sem_id, sbuf, 1);
    char *response_buf = process(query_buf, sem_id);
    sbuf->sem_op = 1;
    semop(sem_id, sbuf, 1);
    free(sbuf);
    error_check("responding", (int) send(client_sd, response_buf, BUFFER_SIZE, 0));
    free(response_buf);

}

char * process(char *query, int sem_id) {
    char *response = calloc(BUFFER_SIZE, sizeof(char));

    return response;
}