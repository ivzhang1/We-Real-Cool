#include<stdio.h>
#include<string.h>
#include <unistd.h>
#include <conio.h>

int main(){
  char stud_name[256];
  char pass[256];
  
  printf("\nStudent Name: ");
  scanf("%s", stud_name);
  fflush(stdout);

  printf("\nPassword: ");
  c = getch();
  while(c != 13){
    res += c;//add to string
    cout<<'*'; 
    c = getch();
  }

  printf("%s", res);
}
