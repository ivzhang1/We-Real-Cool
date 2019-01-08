#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "client.h"
#include "commons.h"

int main(int argc, char * argv[]) {
    check_input(argc, 2, "./client <ip> <port>");
    char *ip = argv[1];
    char *port = argv[2];
    int db_sd = connect_to_db(ip, port);
    while (1) {
        query(db_sd); // do stuff!
        close(db_sd);
        return 0;
    }
}

int connect_to_db(char *ip, char *port) {
    struct addrinfo *results, *p;
    get_results(ip, port, &results);
    int db_sd;
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
        printf("No valid connections found");
        exit(1);
    }
    freeaddrinfo(results);
    printf("[client %d] connected to server\n", getpid());
    return db_sd;
}

void query(int db_sd) {
    char *query_buf = get_query();
    error_check("sending", (int) send(db_sd, query_buf, BUFFER_SIZE, 0));
}

char *get_query() {
    char *query_buf = calloc(BUFFER_SIZE, sizeof(char));
    prompt("Enter query: ", query_buf);
    // check if loading from file
    if (!strncmp(query_buf, "source", 6)) {
        int fd = open(&(query_buf[7]), O_RDONLY);
        read(fd, query_buf, BUFFER_SIZE);
    }
    return query_buf;
}