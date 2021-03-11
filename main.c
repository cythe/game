#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

void printsteps(struct _steps *s)
{
    printf("steps: %d\n", s->cnt);
    for (int i = 0; i < s->cnt; i++)
	printf("\t%d -> %d\n", s->flow[i].src, s->flow[i].dest);
}

void add_step(struct _steps *s, int src, int dest)
{
    s->flow[s->cnt].src = src;
    s->flow[s->cnt].dest = dest;
    s->cnt++;
}

/* 当前管管们的状态 */
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

int _pour(struct _tube *src, struct _tube *dest)
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
int is_sorted(struct _tube *tube)
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
int is_sorted_full(struct _tube *tube)
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

int find_all(struct _tube arr[], int tube_cnt, struct _steps *s)
{
    int cnt = 0;
    memset(s, 0, sizeof(struct _steps));
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
    g_cnt++ ;
    printf("+++++++++ tubes =  %d+++++++++++++++++++++++++++++ \n", g_cnt);

    return p;
}

int find_next_road(struct _tube arr[], int tube_cnt)
{
    int cnt = 0;
    struct _steps next; 
    memset(&next, 0, sizeof(struct _steps));

    if (is_complete(arr, tube_cnt)) {
	printf("--------------------------------------complete2.\n");
	return 0;
    }

    cnt = find_all(arr, tube_cnt, &next);
    if (!cnt) {
	printf("this road is block, return to last node.\n");
	free(arr);
	g_cnt--;
	return -1;
    }

    printsteps(&next);

    for (int i = 0; i < next.cnt; i++)
    {
	printf("before pour:\n");
	printresults(arr, tube_cnt);
	/* create a duplicate to try for next step */
	struct _tube *n = dup_tubes(arr, tube_cnt);
	pour(&n[next.flow[i].src], &n[next.flow[i].dest], 0);
	printf("after pour:\n");
	printresults(n, tube_cnt);
	// try to find next road, if there is nothing, back to last and try another.
	int ret = find_next_road(n, tube_cnt);
	if (ret) {
	    free(n);
	    g_cnt--;
	    continue;
	} else {
	    free(n);
	    g_cnt--;
	    return ret;
	}
    }

    printf("last road is err, return to up.\n");
    return -1;
}

#if 0
/* 递归实现, 目测还有问题, 复杂case会挂 */
void sort(struct _tube arr[], int tube_cnt)
{
    int ret;
    ret = find_next_road(arr, tube_cnt);
    if(!ret)
	printf("complete.\n");
    else
	printf("failed.\n");
}
#else
/* 通过栈和回溯实现 */
// 假设需要MAX_STEP 步
#define MAX_STEP 1024
void sort(struct _tube arr[], int tube_cnt)
{
    struct _steps curr[MAX_STEP];
    memset(curr, 0, sizeof(MAX_STEP * sizeof(struct _steps)));
    struct _from_to tp;
    int ret;
    int p_curr;

    struct _stack stack;

    stack_init(&stack);

    for (int s = 0; s < MAX_STEP; )
    {
	printf("--------------------- step %d -------------------\n", s);
	ret = find_all(arr, tube_cnt, &curr[s]); // 找到这一步所有情况
	// printsteps(&curr[s]);
	if (ret == 0) {
	    // 如果没有下一步, 需要回溯
revert:
	    if (s > 0) {
		s--;
		printf("--------return --------- step %d -------------------\n", s);
	    } else {
		printf("Road block, return last step. \n");
		return;
	    }

	    stack_pop(&stack, &tp);
	    printf("exec: %d -> %d, %d\n", tp.dest, tp.src, tp.cnt);
	    npour(&arr[tp.dest], &arr[tp.src], tp.cnt);
	    printresults(arr, tube_cnt);
	}

	p_curr = curr[s].scence;
	if (p_curr >= curr[s].cnt)
	{
	    goto revert;
	}

	ret = pour(&arr[curr[s].flow[p_curr].src], &arr[curr[s].flow[p_curr].dest], 0);
	curr[s].scence++;
	curr[s].flow[p_curr].cnt = ret;
	stack_push(&stack, curr[s].flow[p_curr]);
	printf("exec: %d -> %d, %d\n", curr[s].flow[p_curr].src, curr[s].flow[p_curr].dest,curr[s].flow[p_curr].cnt);
	printresults(arr, tube_cnt);
	s++;
	if (is_complete(arr, tube_cnt)) {
	    printf("complete!\n");
	    break;
	}
    }

    /* 打印最终结果 */
    stack_print(&stack);
}
#endif

int main(int argc, char* argv[])
{
    /* next: load map from file */
    struct _tube tubes[] = {
#if 0
	/* case 1 */
	{4, BLUE1, GREEN1, RED,  RED},
	{4, RED,  RED,  GREEN1, BLUE1},
	{4, GREEN1,  GREEN1,  BLUE1, BLUE1},
	{0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
#else
#if 0
	/* case 2 */
	{4, BLUE1, RED,  RED, RED},
	{4, GREEN1, RED,  GREEN1, BLUE1},
	{4, GREEN1,  GREEN1,  BLUE1, BLUE1},
	{0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
#else
	/* case 117 */
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
#endif
#endif
    };

    int tube_cnt = sizeof(tubes) / sizeof(tubes[0]);
    printf("tube_cnt = %d\n", tube_cnt);

    sort(tubes, tube_cnt);

    return 0;
}
