#include<stdio.h>
#include<string.h>
#include <unistd.h>

void login(){
  char student_name[256];
  char pass[256];
  
  printf("Student Name: ");
  scanf("%s", student_name);
  fflush(stdout);
  
  printf("Password: ");
  scanf("%s", pass);
  fflush(stdout);
  
}

int main(){
  char student_name[256];
  char pass[256];
  char confirm_pass[256];
  
  printf("Student Name: ");
  fgets(student_name, 256, stdin);
  //fflush(stdout);

  printf("Password: ");
  fgets(pass, 256, stdin);
  //fflush(stdout);
  
  printf("Confirm Password: ");
  fgets(confirm_pass, 256, stdin);
  //fflush(stdout);

  while(strcmp(pass, confirm_pass) != 0){
    printf("Passwords don't match! Try again!\n");

    printf("Password: ");
    fgets(pass, 256, stdin);
    //fflush(stdout);
    
    printf("Confirm Password: ");
    fgets(confirm_pass, 256, stdin);
    //fflush(stdout);
  }
  printf("%ld\n", strlen(student_name));
  *(student_name + strlen(student_name) - 1) = '\0';
  *(pass + strlen(pass) - 1) = '\0';
  *(confirm_pass + strlen(confirm_pass) - 1) = '\0';

  printf("Success: %s, %s, %s\n", student_name, pass, confirm_pass);
  return 0;
  
}
