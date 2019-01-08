#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "client.h"
#include "commons.h"

int main(int argc, char * argv[]) {
    char **vars = check_input(argc, argv, 2, "./client <ip> <port>");
    char *ip = vars[0];
    char *port = vars[1];
    int db_sd = connect_to_db(ip, port);
    while (1) {
        query(db_sd); // do stuff!
        return 0;
    }
}

int connect_to_db(char *ip, char *port) {
    struct addrinfo *results;
    int sd = get_results_and_socket(ip, port, &results);
    freeaddrinfo(results);
    int db_sd = error_check("connecting", connect(sd, results->ai_addr, results->ai_addrlen));
    printf("connected\n");
    return db_sd;
}

void query(int db_sd) {

}