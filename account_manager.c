#include "account_manager.h"

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


int create_account(){
  char student_name[256];
  char pass[256];
  char confirm_pass[256];
  
  printf("Student Name: ");
  fgets(student_name, 256, stdin);
  *(student_name + strlen(student_name)) = '\0';

  
  printf("Password: ");
  my_getpass(pass, 256, stdin);
  
  printf("Confirm Password: ");
  my_getpass(confirm_pass, 256, stdin);

  *(pass + strlen(pass)) = '\0';
  *(confirm_pass + strlen(confirm_pass)) = '\0';

  
  while(strcmp(pass, confirm_pass) != 0){
    printf("Passwords don't match! Try again!\n");

    printf("Password: ");
    my_getpass(pass, 256, stdin);
    
    printf("Confirm Password: ");
    my_getpass(confirm_pass, 256, stdin);

  }
  *(student_name + strlen(student_name)-1) = '\0';
  *(pass + strlen(pass)-1) = '\0';
  *(confirm_pass + strlen(confirm_pass)-1) = '\0';
  
  printf("Account Successfully Created: %s, %s, %s\n", student_name, pass, confirm_pass);

  return 0;
}

int delete_account(){
  char student_name[256];
  
  printf("Student Name: ");
  fgets(student_name, 256, stdin);
  *(student_name + strlen(student_name)-1) = '\0';
  
  printf("Account Successfully Deleted: %s\n", student_name);

  return 0;
}

int main(){
  create_account();
  return 0;
}

