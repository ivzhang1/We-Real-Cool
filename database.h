#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>

#include "commons.h"
#include "table.h"

# define KEY 314159265

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int sem_setup();

int server_setup(char *port);

int get_client(int listening_sd);

void fulfill(int client_sd, int sem_id, struct database *db);

char *process(char *query, int sem_id, struct database *db);
