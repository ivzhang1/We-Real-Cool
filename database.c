#include "database.h"

int main(int argc, char * argv[]) {
    check_input(argc, 1, "./database <port>");
    char *port = argv[1];
    int listening_sd = server_setup(port);
    int sem_id = error_check("creating semaphore", semget(KEY, 1, IPC_CREAT | 0644));

    struct database *db = malloc( sizeof(struct database) );
    db->tables = calloc(10, sizeof(struct table));
    db->num_tables = 0;

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

void fulfill(int client_sd, int sem_id, struct database *db) {
    char *query_buf = malloc(BUFFER_SIZE);
    error_check("receiving", (int) recv(client_sd, query_buf, sizeof(query_buf), 0));
    char *piece, *response_buf;
    printf("[subserver %d] received query, processing\n", getpid());
    while (piece = strsep(&query_buf, ";")){
        response_buf = process(piece, sem_id, db);
        error_check("responding", (int) send(client_sd, response_buf, BUFFER_SIZE, 0));
        free(response_buf);
    }
    free(query_buf);
}

char *process(char *query, int sem_id, struct database *db) {
    struct sembuf *buf;
    buf->sem_op = -1;
    buf->sem_num = 0;
    buf->sem_flg = SEM_UNDO;
    semop(sem_id, buf, 1);
    char *response = execute(query, db);
    buf->sem_op = 1;
    semop(sem_id, buf, 1);
    return response;
}
