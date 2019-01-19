#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <netdb.h>

#include "commons.h"
#include "table.h"

#define SEM_KEY 314159265
#define SHM_KEY 290895069
#define SEG_SIZE 0x1000000

//union semun {
//    int val;
//    struct semid_ds *buf;
//    unsigned short *array;
//    struct seminfo *__buf;
//};

int sem_setup();

int server_setup(char *port);

int get_client(int listening_sd);

int fulfill(int client_sd, struct database *db);
