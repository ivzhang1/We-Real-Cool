#include <stdio.h>
#include <string.h>
#include <unistd.h>

void login(){
  fflush(stdin);

  char student_name[256];
  char pass[256];
  
  printf("Student Name: ");
  fgets(student_name, 256, stdin);

  printf("Password: ");
  fgets(pass, 256, stdin);

  *(student_name + strlen(student_name) - 1) = '\0';
  *(pass + strlen(pass) - 1) = '\0';
  
}

void registerr(){
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
  fgets(pass, 256, stdin);
  
  printf("Confirm Password: ");
  fgets(confirm_pass, 256, stdin);
  *(pass + strlen(pass) - 1) = '\0';
  *(confirm_pass + strlen(confirm_pass) - 1) = '\0';

  
  while(strcmp(pass, confirm_pass) != 0 || strlen(pass) <= 4){
    printf("Passwords don't match! Password needs to be more than 4 characters long! Try again!\n");

    printf("Password: ");
    fgets(pass, 256, stdin);
    
    printf("Confirm Password: ");
    fgets(confirm_pass, 256, stdin);
  }
  *(student_name + strlen(student_name) - 1) = '\0';
  *(pass + strlen(pass) - 1) = '\0';
  *(confirm_pass + strlen(confirm_pass) - 1) = '\0';

  //printf("Success: %s, %s, %s\n", student_name, pass, confirm_pass);
  
}

void log_or_reg(){
  char input[256];
  
  printf("Do you have an account (y/n): ");
  fgets(input, 256, stdin);

  
  while(strncmp(input, "y", 1) && strncmp(input, "n", 1)){
    printf("Try again! Enter 'y' or 'n': ");
    fgets(input, 256, stdin);
  }

  
  if (!strncmp(input, "y", 1)){
    login();
  }
  else{
    registerr();
  }

  
}

void flushing(){

  
}


int main(){
  log_or_reg();
  return 0;
}
