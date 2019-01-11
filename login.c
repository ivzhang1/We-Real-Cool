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
  scanf("%s", student_name);
  fflush(stdout);

  printf("Password: ");
  scanf("%s", pass);
  fflush(stdout);
  
  printf("Confirm Password: ");
  scanf("%s", confirm_pass);
  fflush(stdout);

  while(strcmp(pass, confirm_pass) != 0){
    printf("Passwords don't match! Try again!\n");

    printf("Password: ");
    scanf("%s", pass);
    fflush(stdout);
    
    printf("Confirm Password: ");
    scanf("%s", confirm_pass);
    fflush(stdout);
  }

  return 0;
  
}
