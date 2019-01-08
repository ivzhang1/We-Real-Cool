#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include "al.h"
#include "qs.h"

#define KEY 52013

#define SEG_SIZE 0xffff
#define BUFFER_SIZE 1024
#define DATA_SIZE 256

void prompt(char display[], char input[]);
