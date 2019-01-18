#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#include "commons.h"
#include "table.h"

int connect_to_db(char *ip, char *port);

char * query(int db_sd);

char * get_query();
