#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "tube.h"


int g_cnt = 0;
struct _tube *dup_tubes(struct _tube tubes[], int size)
{
    struct _tube *p = malloc(size * sizeof(struct _tube));
    if (NULL == p)
    {
	printf(" %d@%s: malloc error.\n", __LINE__,__func__);
	getchar();
    }
    memcpy(p, tubes, size*sizeof(struct _tube));
    g_cnt++ ;
    printf("+++++++++ tubes =  %d+++++++++++++++++++++++++++++ \n", g_cnt);

    return p;
}

void free_tubes(struct _tube *tube)
{
	free(tube);
	g_cnt--;
}

/* 当前管管们的状态 */
void printresults(struct _tube tubes[], int tube_cnt)
{
    for (int i = 0; i < tube_cnt; i++)
    {
	printf(" %2d  ", i);
    }
    printf("\n");

    for (int j = 0; j < TUBE_FLOOR; j++)
    {
	for (int k = 0; k < tube_cnt; k++)
	{
	    printcolor(tubes[k].colors[TUBE_FLOOR-j-1]);
	}
	printf("\n");
    }

    for (int i = 0; i < tube_cnt; i++)
    {
	printf(" ^^^ ");
    }
    printf("\n");
}

void printtube(struct _tube *tube)
{
    printf("status: %d \n", tube->status);
    for (int i = 0; i < TUBE_FLOOR; i++)
    {
	printcolor(tube->colors[TUBE_FLOOR-i-1]); 
    }
    printf(" ^^^ \n");
}

static int _pour(struct _tube *src, struct _tube *dest)
{
    int cnt = 0;
    do {
	dest->colors[dest->status] = src->colors[src->status-1];
	dest->status++;
	src->colors[src->status-1] = COL_NULL;
	src->status--;
	cnt++;
	if (dest->status == TUBE_FULL || src->status == TUBE_EMPTY)
	    break;
    } while (src->colors[src->status-1] == dest->colors[dest->status-1]);
    return cnt;
}

void npour(struct _tube *src, struct _tube *dest, int n)
{
    for (int i = 0; i < n; i++)
    {
	dest->colors[dest->status] = src->colors[src->status-1];
	dest->status++;
	src->colors[src->status-1] = COL_NULL;
	src->status--;
    }
}

int pour(struct _tube *tube1, struct _tube *tube2, int reverse)
{
    if (reverse)
	return _pour(tube2, tube1);
    else
	return _pour(tube1, tube2);
}

/* 有序的 */
static int is_sorted(struct _tube *tube)
{
    int color_tmp = tube->colors[0];
    if (tube->status == TUBE_EMPTY) {
	return 1;
    }

    for (int i = 0; i < tube->status; i++)
    {
	if (color_tmp == tube->colors[i])
	    continue;
	else 
	    return 0;
    }
    return 1;
}

/* 排完的 */
static int is_sorted_full(struct _tube *tube)
{
    int color_tmp = tube->colors[0];
    if (tube->status == TUBE_EMPTY) {
	return 1;
    }
    if (tube->status == TUBE_FULL) {
	for (int i = 0; i < tube->status; i++)
	{
	    if (color_tmp == tube->colors[i])
		continue;
	    else 
		return 0;
	}
	return 1;
    } else {
	return 0;
    } 
}

int is_complete(struct _tube arr[], int tube_cnt)
{
    int i;
    for (i = 0; i < tube_cnt; i++)
    {
	if (is_sorted_full(&arr[i]))
	    continue;
	else
	    return 0;
    }

    return 1;
}

int swap_tube(struct _tube *src, struct _tube *dest)
{
    struct _tube *t = malloc(sizeof(struct _tube));
    if (NULL == t)
    {
	printf(" %d@%s: malloc error.\n", __LINE__,__func__);
	getchar();
    }
    memcpy(t, dest, sizeof(struct _tube));
    memcpy(dest, src, sizeof(struct _tube));
    memcpy(src, t, sizeof(struct _tube));
    free(t);
}

int cal_place(struct _tube *t)
{
    return TUBE_FLOOR - t->status;
}

int cal_color(struct _tube *t)
{
    int cnt=0;
    int top = t->colors[t->status-1];
    for (int i = 0; i < t->status; i++)
    {
	if(t->colors[t->status-1-i] == top)
	    cnt++;
    }
    return cnt;

}

int can_pour(struct _tube *src, struct _tube *dest)
{
    // dest full or src empty
    if (dest->status == TUBE_FULL || src->status == TUBE_EMPTY)
	return 0;

    // dest empty
    if (dest->status == TUBE_EMPTY && !is_sorted(src))
	return 1;

    // top color equel
    if (dest->colors[dest->status-1] == src->colors[src->status-1])
    {
	if (cal_place(dest) < cal_color(src))
	    return 0;
	return 1; 
    } else {
	return 0;
    }

    // there maybe other scence ???
    printf("Emmmm.....\n");
    printtube(src);
    printtube(dest);
    getchar();

    return 0;
}

int get_next_tube(int i, int tubecnt)
{
    return (i+1) % tubecnt;
}

int find_empty(struct _tube arr[], int tube_cnt)
{
    int i;
    for (i = 0; i < tube_cnt; i++)
    {
	if (arr[i].status == TUBE_EMPTY)
	    return i;
    }
    return -1;
}

int get_top(struct _tube* t)
{
    return t->colors[t->status-1];
}

