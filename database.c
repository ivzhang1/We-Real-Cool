#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include "table.h"
#include "database.h"


int main(int argc, char * argv[]) {
    check_input(argc, 1, "./database <port>");
    char *port = argv[1];
    int listening_sd = server_setup(port);
    // create semaphore
    int sem_id = sem_setup();
    // create database
    struct table *db = NULL;
    // converse
    while (1) {
        int client_sd = get_client(listening_sd);
        if (!fork()) { // child
            close(listening_sd);
            fulfill(client_sd, sem_id, db); // do stuff!
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

void fulfill(int client_sd, int sem_id, struct table *db) {
    char query_buf[BUFFER_SIZE];
    error_check("receiving", (int) recv(client_sd, query_buf, sizeof(query_buf), 0));
    printf("[subserver %d] received query, processing\n", getpid());
    struct sembuf *sbuf = malloc(sizeof(struct sembuf));
    sbuf->sem_op = -1;
    sbuf->sem_num = 0;
    sbuf->sem_flg = SEM_UNDO;
    semop(sem_id, sbuf, 1);
    char *response_buf = process(query_buf, sem_id, db);
    sbuf->sem_op = 1;
    semop(sem_id, sbuf, 1);
    free(sbuf);
    error_check("responding", (int) send(client_sd, response_buf, BUFFER_SIZE, 0));
    free(response_buf);
}

char *process(char *query, int sem_id, struct table *db) {
    char *response = calloc(BUFFER_SIZE, sizeof(char));
    char *command;
    while ((command = strsep(&query, ";"))) {
        char *action = next_word(&command);
        if (!strcmp(action, "create")) {
            create(command, sem_id, db);
        } else if (!strcmp(action, "insert")) {

        } else if (!strcmp(action, "read")) {

        } else if (!strcmp(action, "update")) {

        } else if (!strcmp(action, "delete")) {

        } else if (!strcmp(action, "drop")) {

        } else {

        }
    }
    return response;
}

void create(char *query, int sem_id, struct table *db) {
    char *name = next_word(&query);
    if (!is_alpha(name)) printf("invalid table name\n"); // todo quit on printf
    struct table *t = malloc(sizeof(struct table));
    strcat(t->name, "table_"); // todo adjust size
    strcat(t->name, name);
    t->columns = NULL;
    init_columns(query, t);
    HASH_ADD_KEYPTR(hh, db, t->name, strlen(t->name), t);
}

void init_columns(char *query, struct table *t) {
    // make sure there was nothing invalid before '{'
    char *prev = strsep(&query, "{");
    if (!empty_string(prev)) printf("invalid syntax before {\n"); // todo quit on printf
    // strip end
    char *after = query;
    query = strsep(&after, "}");
    if (!empty_string(prev)) printf("invalid syntax after }\n"); // todo quit on printf
    // iterate by commas
    char *header;
    for (int i = 0; (header = strsep(&query, ",")); i++) {
        // create column
        struct column *col = malloc(sizeof(struct column));
        // set type
        char *type_str = next_word(&header);
        unsigned char type;
        if (!strcmp(type_str, "int"))type = INT;
        else if (!strcmp(type_str, "double")) type = DBL;
        else if (!strcmp(type_str, "string")) type = STR;
        else printf("invalid type column %d\n", i); // todo quit on printf
        col->type = type;
        // set flags

    }
}