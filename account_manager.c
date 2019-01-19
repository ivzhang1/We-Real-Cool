#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include "commons.h"
#include "account_manager.h"

int main(int argc, char * argv[]) {
    check_input(argc, 2, "./account_manager <ip> <port>");
    char *ip = argv[1];
    char *port = argv[2];
    int db_sd = connect_to_db(ip, port);

    while (1) {
      create_or_Not(db_sd);

    }
    return 0;
}


int connect_to_db(char *ip, char *port) {
    struct addrinfo *results, *p;
    get_results(ip, port, &results);
    int db_sd;
    for (p = results; p != NULL; p = p->ai_next) {
        if ((db_sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;
        if (connect(db_sd, p->ai_addr, p->ai_addrlen) == -1) {
            close(db_sd);
            continue;
        }
        break;
    }
    if (!p) {
        printf("No valid connections found\n");
        exit(1);
    }
    freeaddrinfo(results);
    printf("[client %d] connected to server\n", getpid());
    return db_sd;
}


char* my_getpass (char *line, int n, FILE *stream){
    struct termios old, new;
    char* nread = (char *)calloc(256, sizeof(char));

    /* Turn echoing off and fail if we can't. */
    if (tcgetattr (fileno (stream), &old) != 0)
        return NULL;
    new = old;
    new.c_lflag &= ~ECHO;
    if (tcsetattr (fileno (stream), TCSAFLUSH, &new) != 0)
        return NULL;

    /* Read the password. */
    nread = fgets(line, n, stream);

    /* Restore terminal. */
    (void) tcsetattr (fileno (stream), TCSAFLUSH, &old);
    printf("\n");
    return nread;
}


int create_account(int db_sd){
  char student_name[256];
  char pass[256];

  printf("Student Name: ");
  fgets(student_name, 256, stdin);
  *(student_name + strlen(student_name)) = '\0';


  printf("Password: ");
  my_getpass(pass, 256, stdin);

  *(student_name + strlen(student_name)-1) = '\0';
  *(pass + strlen(pass)-1) = '\0';

  char user_pass[BUFFER_SIZE];
  sprintf(user_pass, "insert usernames  {  (username:\"%s\", password:\"%s\")  } ", student_name, pass);
  //printf("%s", execute(user_pass, db));

  error_check("sending", (int) send(db_sd, user_pass, BUFFER_SIZE, 0));
  char *response_buf = calloc(BUFFER_SIZE, sizeof(char));
  recv(db_sd, response_buf, BUFFER_SIZE, 0);

  printf("%s\n", response_buf);
  free(response_buf);

  printf("Account Successfully Created: %s, %s\n", student_name, pass);
  return 0;
}

int delete_account(int db_sd){
  char student_name[256];

  printf("Student Name: ");
  fgets(student_name, 256, stdin);
  *(student_name + strlen(student_name)-1) = '\0';

  char user_pass[BUFFER_SIZE];
  sprintf(user_pass, "delete usernames where username = \"%s\"", student_name);
  //printf("%s", execute(user_pass, db));

  error_check("sending", (int) send(db_sd, user_pass, BUFFER_SIZE, 0));
  char *response_buf = calloc(BUFFER_SIZE, sizeof(char));
  recv(db_sd, response_buf, BUFFER_SIZE, 0);

  printf("%s\n", response_buf);
  free(response_buf);



  printf("Account Successfully Deleted: %s\n", student_name);

  return 0;
}

int create_or_Not(int db_sd){
  char input[256];

  printf("y ==> Create an Account\nn ==> Delete an Account\nEnter y/n: ");
  fgets(input, 256, stdin);


  while(strncmp(input, "y", 1) && strncmp(input, "n", 1)){
    printf("Try again! Enter 'y' or 'n': ");
    fgets(input, 256, stdin);
  }


  if (!strncmp(input, "y", 1)){
    return create_account(db_sd);
  }
  else{
    return delete_account(db_sd);
  }


}
