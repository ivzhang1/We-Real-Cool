#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "table.h"
#include "client.h"

int main(int argc, char * argv[]) {
    check_input(argc, 2, "./client <ip> <port>");
    char *ip = argv[1];
    char *port = argv[2];
    int db_sd = connect_to_db(ip, port);
    while (1) {
        char *response_buf = query(db_sd);
        printf("%s\n", response_buf);
        free(response_buf);
    }
}

int connect_to_db(char *ip, char *port) {
    struct addrinfo *results, *p;
    get_results(ip, port, &results);
    int db_sd = -1;
    for (p = results; p != NULL; p = p->ai_next) {
        if ((db_sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;
        if (connect(db_sd, p->ai_addr, p->ai_addrlen) == -1) {
            close(db_sd);
            continue;
        }
        break;
    }
    if (!p) {
        printf("No valid connections found\n");
        exit(1);
    }
    freeaddrinfo(results);
    printf("[client %d] connected to server\n", getpid());
    return db_sd;
}

char *query(int db_sd) {
    char *query_buf = get_query();
    error_check("sending", (int) send(db_sd, query_buf, BUFFER_SIZE, 0));
    free(query_buf);
    char *response_buf = calloc(BUFFER_SIZE, sizeof(char));
    recv(db_sd, response_buf, BUFFER_SIZE, 0);
    return response_buf;
}

char *get_query() {
    char *query_buf = calloc(BUFFER_SIZE, sizeof(char));
    prompt("Enter query: ", query_buf);
    // check if loading from file
    char *temp = query_buf;
    for (; *temp == ' '; temp++); // dont use next word to modify string
    if (!strncmp(temp, "source", 6)) {
        char *file = next_word(&temp);
        int fd = error_check("loading file", open(file, O_RDONLY));
        query_buf[read(fd, query_buf, BUFFER_SIZE)] = '\0';
    }
    return query_buf;
}
