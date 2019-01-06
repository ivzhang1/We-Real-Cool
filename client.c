#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "commons.h"

int main(int argc, char * argv[]) {
    /*
     * Prompt ip:port (if not given)
     * Connect to socket
     * always:
     *      Prompt for input
     *      Check if loading from file
     *      send to database
     * */
     char input[STR_LEN];
     char* piece;
     char* dummy;
     int s;

     // signal(SIGINT, sighandler);
     while (1){

         getcwd(cwd, STR_LEN);
         printf("%s> ", cwd);
         scanf(" %[^\n]s", input);


         dummy = input;
         while (piece = strsep(&dummy, ";")){

             args = parse_args(piece);
             s = execute(args);
             if (s > 0)
                 printf("%s\n", strerror(s));
             if (s == -2)
                 break;
             if (s == -3)
                 return 0;
         }
     }

     return 0;
}
