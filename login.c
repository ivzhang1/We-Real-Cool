#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include "table.h"
#include "login.h"
#include "commons.h"


char* my_getpass (char *line, int n, FILE *stream)
{
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

int login(int db_sd){
  fflush(stdin);

  char student_name[256];
  char pass[256];

  printf("Student Name: ");
  fgets(student_name, 256, stdin);
  *(student_name + strlen(student_name) - 1) = '\0';

  printf("Password: ");
  my_getpass(pass, 256, stdin);
  *(pass + strlen(pass) - 1) = '\0';
  //while(!strcmp(accounts[i].password, pass)){
  //  printf("Wrong password! Try again!\n");

  //printf("Password: ");
    //  my_getpass(pass, 256, stdin);
    //*(pass + strlen(pass) - 1) = '\0';

  //}

  char user_pass[1000];
  sprintf(user_pass, "read usernames * where username = %s & password = %s", student_name, pass);
  //printf("%s", execute(user_pass, db));

  error_check("sending", (int) send(db_sd, user_pass, 1000, 0));
  char *response_buf = calloc(1000, sizeof(char));
  recv(db_sd, response_buf, 1000, 0);




  return 0;
}

int registerr(){
  char student_name[256];
  char pass[256];
  char confirm_pass[256];

  printf("Student Name: ");
  fgets(student_name, 256, stdin);
  *(student_name + strlen(student_name) - 1) = '\0';

  while(strlen(student_name) <= 4){
    printf("Username must be longer than 4 characters\n");
    printf("Student Name: ");
    fgets(student_name, 256, stdin);
  }



  printf("Password: ");
  my_getpass(pass, 256, stdin);

  printf("Confirm Password: ");
  my_getpass(confirm_pass, 256, stdin);

  *(pass + strlen(pass) - 1) = '\0';
  *(confirm_pass + strlen(confirm_pass) - 1) = '\0';


  while(strcmp(pass, confirm_pass) != 0 || strlen(pass) <= 4){
    printf("Passwords don't match! Password needs to be more than 4 characters long! Try again!\n");

    printf("Password: ");
    my_getpass(pass, 256, stdin);

    printf("Confirm Password: ");
    my_getpass(confirm_pass, 256, stdin);

  }
  *(student_name + strlen(student_name) - 1) = '\0';
  *(pass + strlen(pass) - 1) = '\0';
  *(confirm_pass + strlen(confirm_pass) - 1) = '\0';

  printf("Success: %s, %s, %s\n", student_name, pass, confirm_pass);

  return 0;
}
