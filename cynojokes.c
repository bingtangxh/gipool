#include "cynojokes.h"
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#define NL putchar('\n')

int main(void)
{
    int index1=0,index2=0;
    char current;
    while(1)
    {
        system("cls");
        index1=rand()%7;
        index2=rand()%6;
        if(
            index1>=5&&index2==5
        )continue;
        puts(questions[index1][index2]);
        NL;
        puts("Press any key to see the answer...");
        getch();
        NL;
        puts(answers[index1][index2]);
        NL;
        puts("Press space or ^Z or ^C to exit, or other thing to continue...");
        NL;
        current=getch();
        if(
            current==EOF || current==' '
        ) break;
        else continue;
    }
    return 0;
}