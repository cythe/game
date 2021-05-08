#include <stdio.h>
#include <string.h>
#include "color.h"

#define ROW 10
#define COL 10

int remain[10] = {2000,1980,1920,1820,1680,1500,1280,1020,720,380};
int score = 0;

//e.g:
int eg[10][10] = {
    {220,220,46,92,20,92,92,92,46,46},
    {160,92,160,220,220,92,92,160,20,220},
    {20,92,220,20,220,92,92,46,20,220},
    {20,220,160,92,160,220,20,46,220,20},
    {20,20,20,160,160,92,160,220,20,92},
    {220,20,46,92,46,220,220,46,160,220},
    {92,20,46,46,46,20,20,20,220,220},
    {220,220,46,220,160,20,220,20,20,220},
    {92,160,46,46,160,160,92,220,20,46},
    {92,20,46,20,92,160,20,20,20,46}
};

struct _star {
    int color;
    int paired;
    int search;
} stars[ROW][COL] = {{0}};

struct _point {
    int x;
    int y;
};

struct _step {
    int cnt;
    struct _point points[ROW*COL];
};

int get_point(int cnt)
{
    return 5 * cnt * cnt;
}

int trip()
{

}

int is_valid_point(int i, int j)
{
    return i>=0 && i<ROW && j>=0 && j<COL;
}

int print_step(struct _step *st)
{
    int star[ROW][COL] = {0};
    int i, x, y;
    for(i = 0; i < st->cnt; i++)
    {
	x=st->points[i].x;
	y=st->points[i].y;
	star[x][y] = 1;
    }

    for(x=0;x<ROW;x++)
    {
	for(y=0;y<ROW;y++)
	{
	    printf("%d ", star[x][y]);
	}
	printf("\n");
    }
    printf("\n");
}
#if 1
int ten_search(int i, int j, struct _step *steps, int s)
{
    int p = 0;
    if (is_valid_point(i+1, j) && stars[i+1][j].color==stars[i][j].color) {
	    p = steps[s].cnt;
	    steps[s].points[p].x = i+1;
	    steps[s].points[p].y = j;
	    steps[s].cnt++;
	    printf("%d add %d %d to steps\n", __LINE__, i+1, j);
	    ten_search(i+1,j, steps, s);
    }

    if (is_valid_point(i-1, j) && stars[i-1][j].color==stars[i][j].color) {
	    p = steps[s].cnt;
	    steps[s].points[p].x = i-1;
	    steps[s].points[p].y = j;
	    steps[s].cnt++;
	    printf("%d add %d %d to steps\n", __LINE__, i-1, j);
	    ten_search(i-1,j, steps, s);
    }

    if (is_valid_point(i, j+1) && stars[i][j+1].color==stars[i][j].color) {
	    p = steps[s].cnt;
	    steps[s].points[p].x = i;
	    steps[s].points[p].y = j+1;
	    steps[s].cnt++;
	    printf("%d add %d %d to steps\n", __LINE__, i, j+1);
	    ten_search(i,j+1, steps, s);
    }

    if (is_valid_point(i, j-1) && stars[i][j-1].color==stars[i][j].color) {
	    p = steps[s].cnt;
	    steps[s].points[p].x = i;
	    steps[s].points[p].y = j-1;
	    steps[s].cnt++;
	    printf("%d add %d %d to steps\n", __LINE__, i, j-1);
	    ten_search(i,j+1, steps, s);
    }

}
#endif

int find_all()
{
    int i,j,s=0,p=0;
    struct _step steps[ROW*COL];
    memset(steps, 0, sizeof(steps));
#if 0
    for(i = 0; i < ROW; i++)
    {
	for(j = 0; j < COL; j++)
	{
	    if (stars[i][j].paired=1)
		continue;
	    // 找出这个块
	    ten_search(8,3,&steps);
	}
    }
#endif
    ten_search(8,2,&steps, 0);
    print_step(&steps[0]);
}

void init_color(void)
{
    for(int i = 0; i < 10; i++)
    {
	for (int j = 0; j < 10; j++)
	{
	    stars[i][j].color=eg[i][j];
	}
    }
}

int main(int argc, char *argv[])
{
    init_color();

    for(int i = 0; i < 10; i++)
    {
	for (int j = 0; j < 10; j++)
	{
	    printcolor(stars[i][j].color);
	}
	printf("\n");
    }

    find_all();
    return 0;
}
