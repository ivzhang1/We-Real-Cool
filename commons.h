#include <netdb.h>

#define BUFFER_SIZE 1024
#define DATA_SIZE 256

void prompt(char *display, char *input);

int error_check(int retval);

int get_results_and_socket(char *ip, char *port, struct addrinfo **results_list);