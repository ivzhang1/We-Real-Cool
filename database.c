#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "database.h"
#include "commons.h"

int main(int argc, char * argv[]) {
    /*
     * Setup semaphore
     * Prompt port (if not given)
     * always:
     *      wait for connection
     *      fork on connection
     *      subserver:
     *          parse
     *          use semaphore as necessary
     *
     * */

}
