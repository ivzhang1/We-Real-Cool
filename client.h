#ifndef FINAL_CLIENT_H
#define FINAL_CLIENT_H

int connect_to_db(char *ip, char *port);

char * query(int db_sd);

char * get_query();

#endif //FINAL_CLIENT_H