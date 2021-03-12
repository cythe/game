#include <stdio.h>

void printcolor(int c)
{
    if(c <= 0)
	printf("|   |");
    else
	printf("|\033[1;48;5;%dm%3d\033[0m|", c, c);
}

void print_std256color(void)
{
    int c = 0;
    printf("\n==================== stadard 256 colors ====================\n");
    printf("-------------------- stadard 8色 --------------------\n");
    for (int i = 0; i < 8; i++)
    {
	printcolor(i);
	//printf("\033[1;48;5;%dm[%3d]\033[0m", i, i);
    }
    printf("\n");

    printf("-------------------- enhance 8 colors --------------------\n");
    for (int i = 8; i < 16; i++)
    {
	printcolor(i);
	//printf("\033[1;48;5;%dm[%3d]\033[0m", i, i);
    }
    printf("\n");

    printf("-------------------- 216 colors --------------------");
    for (int i = 16; i < 232; i++)
    {
	if (c%36 == 0)
	    printf("\n");
	printcolor(i);
	//printf("\033[1;48;5;%dm[%3d]\033[0m", i, i);
	c++;
    }
    printf("\n");

    printf("-------------------- gray --------------------\n");
    for (int i = 232; i < 256; i++)
    {
	printcolor(i);
	//printf("\033[1;48;5;%dm[%3d]\033[0m", i, i);
    }
    printf("\n");
}
