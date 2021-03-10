#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

enum color {
    COL_NULL=0,
    BLUE1,
    BLUE2,
    PUPOR,
    GREEN1,
    GREEN2,
    GREEN3,
    RED,
    PINK,
    GRAY,
    YELLOW1,
    YELLOW2,
    BROWN,
    MAX,
};

#define TUBE_FLOOR 4
#define TUBE_FULL TUBE_FLOOR
#define TUBE_EMPTY 0

#define TUBE_CNT 14
#define MAYBE ( TUBE_CNT * (TUBE_CNT-1)) 

struct _tube {
    int status;
    int colors[TUBE_FLOOR];
};

struct _step {
    int cnt;
    struct _pair pair[MAYBE];
};

void printsteps(struct _step *s)
{
    printf("steps: %d\n", s->cnt);
    for (int i = 0; i < s->cnt; i++)
	printf("\t%d -> %d\n", s->pair[i].src, s->pair[i].dest);
}

void add_step(struct _step *s, int src, int dest)
{
    s->pair[s->cnt].src = src;
    s->pair[s->cnt].dest = dest;
    s->cnt++;
}

void printresults(struct _tube tubes[], int tube_cnt)
{
    for (int i = 0; i < tube_cnt; i++)
    {
	printf(" %2d  ", i);
    }
    printf("\n");
#if 0
    for (int i = 0; i < tube_cnt; i++)
    {
	printf("|-| ");
    }
    printf("\n");
#endif
    for (int j = 0; j < TUBE_FLOOR; j++)
    {
	for (int k = 0; k < tube_cnt; k++)
	{
	    printf("|%2d| ", tubes[k].colors[TUBE_FLOOR-j-1]);
	}
	printf("\n");
    }

    for (int i = 0; i < tube_cnt; i++)
    {
	printf("  ^  ");
    }
    printf("\n");
}

void printtube(struct _tube *tube)
{
    printf("status: %d ", tube->status);
    printf("colors: ");
    for (int i = 0; i < tube->status; i++)
    {
	printf("%d ", tube->colors[i]);
    }
    printf("\n");
}

void _pour(struct _tube *src, struct _tube *dest)
{
    do {
	dest->colors[dest->status] = src->colors[src->status-1];
	dest->status++;
	src->colors[src->status-1] = COL_NULL;
	src->status--;
	if (dest->status == TUBE_FULL || src->status == TUBE_EMPTY)
	    break;
    } while (src->colors[src->status-1] == dest->colors[dest->status-1]);
}

void pour(struct _tube *tube1, struct _tube *tube2, int reverse)
{
    if (reverse)
	_pour(tube2, tube1);
    else
	_pour(tube1, tube2);
}

int is_sorted(struct _tube *tube)
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
	if (is_sorted(&arr[i]))
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

int can_pour(struct _tube *src, struct _tube *dest)
{
    // dest full or src empty
    if (dest->status == TUBE_FULL || src->status == TUBE_EMPTY)
	return 0;

    // dest empty
    if (dest->status == TUBE_EMPTY)
	return 1;

    // top color equel
    if (dest->colors[dest->status-1] == src->colors[src->status-1])
	return 1; 
    else
	return 0;

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

int find_all(struct _tube arr[], int tube_cnt, struct _step *s)
{
    int cnt = 0;
    for (int i = 0; i < tube_cnt; i++)
    {
	for (int j = 0; j < tube_cnt; j++)
	{
	    if (j==i)
		continue;
	    if (can_pour(&arr[i], &arr[j])) {
		/* add this step to list */
		add_step(s, i, j);
		cnt ++ ;
	    }
	}
    }

    return cnt;
}

int g_cnt=0;
struct _tube *dup_tubes(struct _tube tubes[], int size)
{
    struct _tube *p = malloc(size * sizeof(struct _tube));
    if (NULL == p)
    {
	printf(" %d@%s: malloc error.\n", __LINE__,__func__);
	getchar();
    }
    memcpy(p, tubes, size*sizeof(struct _tube));
    g_cnt ++ ;
    printf("+++++++++ tubes =  %d+++++++++++++++++++++++++++++ \n", g_cnt);

    return p;
}

int find_next_road(struct _tube arr[], int tube_cnt)
{
    int cnt = 0;
    struct _step next; 
    memset(&next, 0, sizeof(struct _step));

    if (is_complete(arr, tube_cnt)) {
	printf("-------------------------------complete1.\n");
	return 0;
    }

    cnt = find_all(arr, tube_cnt, &next);
    if (!cnt) {
	printf("this road is block, return to last node.\n");
	return -1;
    }

    //printsteps(&next);

    for (int i = 0; i < cnt; i++)
    {
	printf("before pour:\n");
	printresults(arr, tube_cnt);
	/* create a duplicate to try for next step */
	struct _tube *n = dup_tubes(arr, tube_cnt);
	pour(&n[next.pair[i].src], &n[next.pair[i].dest], 0);
	printf("after pour:\n");
	printresults(n, tube_cnt);
	if (is_complete(n, tube_cnt)) {
	    free(n);
	    printf("--------------------------------------complete2.\n");
	    return 0;
	}
	// try to find next road, if there is nothing, back to last and try another.
	int ret = find_next_road(n, tube_cnt);
	if (ret == 0)
	{
	    if (is_complete(arr, tube_cnt)) {
		printf("-------------------------------------complete3.\n");
		return 0;
	    }
	    else 
	    {
		free(n);
		continue;
	    }
	}
    }

    printf("last road is err, return to up.\n");
    return -1;
}

void sort(struct _tube arr[], int tube_cnt)
{
    int ret;
    ret = find_next_road(arr, tube_cnt);
    if(ret)
	printf("complete.\n");
    else
	printf("failed.");
}

int main(int argc, char* argv[])
{
    /* next: load map from file */
    struct _tube tubes[] = {
	{4, GREEN1, GREEN3, PUPOR,  BLUE1},
	{4, PINK,   GRAY,   GREEN1, BLUE2},
	{4, BLUE1,  BLUE2,  BLUE1,  BLUE2},
	{4, GREEN3, PUPOR,  YELLOW2, RED},
	{4, BLUE1,  RED,    YELLOW2, GREEN1},
	{4, GREEN3, YELLOW2, YELLOW1, RED},
	{4, PUPOR,  RED,    GREEN2,  GREEN2},
	{4, GREEN1, GREEN2, GRAY,    GREEN2},
	{4, YELLOW1, BROWN, BLUE2, YELLOW1},
	{4, PINK, YELLOW2, PUPOR, BROWN},
	{4, GRAY, YELLOW1, BROWN, GRAY},
	{4, PINK, GREEN3, BROWN, PINK},
	{0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
	{0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
    };

    int tube_cnt = sizeof(tubes) / sizeof(tubes[0]);
    printf("tube_cnt = %d\n", tube_cnt);

    sort(tubes, tube_cnt);

    return 0;
}
