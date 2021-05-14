#include <stdio.h>
#include "basetype.h"

int is_color_tolerant(int a, int b)
{
    if (a > b)
	return (a - b) < g_color_tolerance;
    else
	return (b - a) < g_color_tolerance;

}

void printrgb(int r, int g, int b)
{
    if (g_need_color_value) // print R,G,B 
        printf("[\033[1;48;2;%d;%d;%dm%3d,%3d,%3d\033[0m]", r,g,b, r, g, b); 
    else
        printf("[\033[1;48;2;%d;%d;%dm  \033[0m]", r,g,b); 
}

void printcolor(int c)
{
    if(g_use_rgb == 1)
    {
	printrgb((c>>16)&0xff, (c>>8)&0xff, c&0xff);
	return;
    }

    if (g_need_color_value) // print R,G,B 
	printf("[\033[1;48;5;%dm%3d\033[0m]", c, c);
    else
	printf("[\033[1;48;5;%dm  \033[0m]", c);
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
