#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tube.h"
#include "stack.h"
#include "color.h"
#include "load.h"

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

int find_all(struct _tube arr[], int tube_cnt, struct _steps *s)
{
    int cnt = 0;
    memset(s, 0, sizeof(struct _steps));
    for (int i = 0; i < tube_cnt; i++)
    {
	for (int j = 0; j < tube_cnt; j++)
	{
	    if (j == i)
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
	free_tubes(arr);
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
	    free_tubes(n);
	    continue;
	} else {
	    free_tubes(n);
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
void sort(struct _tube arr[], int tube_cnt)
{
    struct _steps curr[MAX_STEP];
    memset(curr, 0, sizeof(MAX_STEP * sizeof(struct _steps)));
    struct _from_to tp;
    int ret;
    int p_curr;
    int s;

    struct _stack stack;

    stack_init(&stack);

    for (s = 0; s < MAX_STEP; )
    {
	//printf("--------------------- step %d -------------------\n", s);
	ret = find_all(arr, tube_cnt, &curr[s]); // 找到这一步所有情况
	// printsteps(&curr[s]);
	if (ret == 0) {
	    // 如果没有下一步, 需要回溯
revert:
	    if (s > 0) {
		s--;
		//printf("--------return --------- step %d -------------------\n", s);
	    } else {
		//printf("Road block, return last step. \n");
		return;
	    }

	    stack_pop(&stack, &tp);
	    //printf("exec: %d -> %d, %d\n", tp.dest, tp.src, tp.cnt);
	    npour(&arr[tp.dest], &arr[tp.src], tp.cnt);
	    //printresults(arr, tube_cnt);
	}

	p_curr = curr[s].scence;
	if (p_curr >= curr[s].cnt) {
	    goto revert;
	}

	ret = pour(&arr[curr[s].flow[p_curr].src], &arr[curr[s].flow[p_curr].dest], 0);
	curr[s].scence++;
	curr[s].flow[p_curr].cnt = ret;
	stack_push(&stack, curr[s].flow[p_curr]);
	//printf("exec: %d -> %d, %d\n", curr[s].flow[p_curr].src, curr[s].flow[p_curr].dest,curr[s].flow[p_curr].cnt);
	//printresults(arr, tube_cnt);
	s++;
	if (is_complete(arr, tube_cnt)) {
	    printf("complete!\n");
	    break;
	}
    }

    while (s--)
    {
	stack_pop(&stack, &tp);
	printf("exec: %d -> %d, %d\n", tp.dest, tp.src, tp.cnt);
	npour(&arr[tp.dest], &arr[tp.src], tp.cnt);
	printresults(arr, tube_cnt);
    }
    /* 打印最终结果 */
    //stack_print(&stack);
}
#endif

int main(int argc, char* argv[])
{
    load_map();

    sort(g_tubes, g_tube_cnt);

    return 0;
}
