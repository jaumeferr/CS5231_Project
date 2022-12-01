#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>

// gcc Game.c -o Game

int secret(char* pass,int *result,int a,int b)
{
    char buffer[10];
    strcpy(buffer,pass);
    char* sec;
    printf("Player 2 enter the word to be guessed!\n");
    scanf("%s",&sec);
   //secret stored in the stack
   //call a different function that prints the buffer when there is an integer overflow.
    if (a > INT_MAX - b)
        leak(buffer);
    else {
        printf("Welcome!!\n");
        printf("Player 1 get ready to find the secret word!!\n");
        printf("Player 2 is allowed to mention if the letter guessed is available in the secret entered by Player 2\n");
        printf("Let the games begin!");
        return 0;
    
}
}
int leak(char * buf)
{
    printf("Secret leaked!\n");
    strcat(buf,"%p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p");
    printf(buf);
    return -1;
}
int main(int argc, char** argv)
{
    int *res=(int*)malloc(sizeof(int));
    char nameA[10];
    char nameB[10];
    int x,y;
    //exploit the code so that the value of x is changed in such a way that it causes a buffer overflow even if the value is not a higher value.
   printf("Enter the ages of the players\n");
   scanf("%d\n%d",&x,&y);
    printf("Enter player names(Max length 10)\n");
    scanf("%s",&nameA);
    scanf("%s",&nameB);
   secret(nameA,res, x, y);
    
    return 0;
}