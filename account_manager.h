#include <netdb.h>

char* my_getpass (char *line, int n, FILE *stream);

int create_account(int db_sd);

int delete_account(int db_sd);

int connect_to_db(char *ip, char *port);

int create_or_Not(int db_sd);
