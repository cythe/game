#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum color {
    COL_NULL=0,
    COL_BLUE,
    COL_GREEN,
    COL_RED,
    COL_MAX,
};

char* color[COL_MAX] = {
    "COL_NULL",
    "COL_BLUE",
    "COL_GREEN",
    "COL_RED",
};

#define TUBE_FLOOR 4
#define TUBE_FULL TUBE_FLOOR
#define TUBE_EMPTY 0
#define EMPTY_TUBE {TUBE_EMPTY, COL_NULL}

struct _tube {
    int status;
    int colors[TUBE_FLOOR];
};

struct _steps {
    int cnt;
    struct _pair{
	int src;
	int dest;
    } pair[256];
} steps;

void add_step(int src, int dest)
{
    steps.pair[steps.cnt].src = src;
    steps.pair[steps.cnt].dest = dest;
    steps.cnt++;
}

void drop_step(int src, int dest)
{
    steps.cnt--;
}

void printsteps(void)
{
    printf("steps: %d\n", steps.cnt);
    for (int i = 0; i < steps.cnt; i++)
    {
	printf("\t%d -> %d\n", steps.pair[i].src, steps.pair[i].dest);
    }
}

void init_steps(void)
{
    memset(&steps, 0, sizeof(struct _steps));
    printsteps();
}

void printresults(struct _tube tubes[], int tube_cnt)
{
    for (int i = 0; i < tube_cnt; i++)
    {
	printf(" %d  ", i);
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
	    printf("|%d| ", tubes[k].colors[TUBE_FLOOR-j-1]);
	}
	printf("\n");
    }

    for (int i = 0; i < TUBE_FLOOR; i++)
    {
	printf(" ^  ");
    }
    printf("\n");
//    ^~^*_-=+|\;'/.,'
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

int sorted(struct _tube tube)
{
    int color_tmp = tube.colors[0];
    if (tube.status == TUBE_FULL) {
	for (int i = 0; i < tube.status; i++)
	{
	    if (color_tmp == tube.colors[i])
		continue;
	    else 
		return 0;
	}
	return 1;
    } else {
	return 0;
    } 
}

int iscomplete(struct _tube arr[], int tube_cnt)
{
    int i;
    for (i = 0; i < tube_cnt; i++)
    {
	if (sorted(arr[i]))
	    continue;
	else
	    return 0;
    }

    return 1;
}

int swap_tube(struct _tube *src, struct _tube *dest)
{
    struct _tube *t = malloc(sizeof(struct _tube));
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

    // top color eque
    if (dest->colors[dest->status-1] == src->colors[src->status-1])
	return 1; 
    else
	return 0;

    // other scence
    printf("Emmmm.....\n");
    printtube(src);
    printtube(dest);
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

void sort(struct _tube arr[], int tube_cnt)
{
    /* Emmmmmmm ..... */

}

struct _tube *dup_tubes(struct _tube tubes[], int size)
{
    struct _tube *p = malloc(size * sizeof(struct _tube));
    memcpy(p, tubes, size*sizeof(struct _tube));

    return p;
}

int main(int argc, char* argv[])
{
    struct _tube tubes[] = {
	{4, COL_BLUE, COL_GREEN, COL_RED,  COL_RED},
	{4, COL_RED,  COL_RED,  COL_GREEN, COL_BLUE},
	{4, COL_GREEN,  COL_GREEN,  COL_BLUE, COL_BLUE},
	{0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
    };

    int tube_cnt = sizeof(tubes) / sizeof(tubes[0]);
    printf("tube_cnt = %d\n", tube_cnt);

    struct _tube *d = dup_tubes(tubes, tube_cnt);

    init_steps();

    sort(d, tube_cnt);

    printf("---------- before ----------\n");
    printresults(tubes, tube_cnt);

    printf("---------- after ----------\n");
    printresults(d, tube_cnt);

    printsteps();

    free(d);
    return 0;
}
