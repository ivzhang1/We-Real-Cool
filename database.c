
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
    int client_sd;
    int sem_id = sem_setup();
    struct sembuf *sbuf = malloc(sizeof(struct sembuf));
    sbuf->sem_num = 0;
    sbuf->sem_flg = SEM_UNDO;
    // int from_subserver, to_server;

    struct database *db = db_setup();

    char user_pass[] = "create  usernames  {  string username  ,  string   password  } ";
    printf("%s", execute(user_pass, db));

    while (1) {
        client_sd = get_client(listening_sd);
        // mkfifo("wkp", 0644);
        // from_subserver = open("wkp", O_RDONLY);
        // if (!fork()) { // child
            // close(listening_sd);
        sbuf->sem_op = -1;
        semop(sem_id, sbuf, 1);

        while(fulfill(client_sd, db)); // do stuff!
        close(client_sd);

        sbuf->sem_op = 1;
        semop(sem_id, sbuf, 1);
            // exit(0);
        // } else
            // close(client_sd); // parent
    }
}

int sem_setup() {
    int sem_id = error_check("creating semaphore", semget(SEM_KEY, 1, IPC_CREAT | 0644));

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
    error_check("listening", listen(listening_sd, 0));
    freeaddrinfo(results);
    return listening_sd;
}

int get_client(int listening_sd) {
    struct sockaddr_storage client_address;
    socklen_t sock_size = sizeof(client_address);
    int client_sd = error_check("accepting", accept(listening_sd, (struct sockaddr *) &client_address, &sock_size));
    printf("[server %d] connected to client\n", getpid());
    return client_sd;
}

int fulfill(int client_sd, struct database *db) {
    char *query_buf = malloc(BUFFER_SIZE);
    // error_check("receiving", (int) recv(client_sd, query_buf, BUFFER_SIZE, 0));
    if ( !recv(client_sd, query_buf, BUFFER_SIZE, 0) ){
        // printf("##");
        return 0;

    }

    char *piece;
    char *response_buf = malloc(BUFFER_SIZE);
    response_buf[0] = 0;

    // query_buf[ strlen(query_buf) - 1 ] = 0;
    while (piece = strsep(&query_buf, ";\n")){
        // printf("[%s]\n", piece);
        strcat(response_buf, execute(piece, db));
    }
    // error_check("responding", (int) send(client_sd, response_buf, BUFFER_SIZE, 0));
    if ( !send(client_sd, response_buf, BUFFER_SIZE, 0) ){
        // printf("#");
        return 0;
    }

    free(response_buf);
    return 1;
}
