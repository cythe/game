#include <stdio.h>
#include <string.h>
#include "water.h"

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

int find_next_layer(struct _tube arr[], int tube_cnt, struct _steps *n)
{
    int cnt = 0;
    int ret;
    struct _steps next; 
    memset(&next, 0, sizeof(struct _steps));

    // for all last layer steps, try to find next layer
    for (int i = 0; i < n->cnt; i++)
    {
	// printf("before pour:\n");
	printresults(arr, tube_cnt);

	ret = pour(&arr[n->flow[i].src], &arr[n->flow[i].dest], 0);
	n->flow[i].cnt = ret;
	n->scence++;

	// return case 1
	if (is_complete(arr, tube_cnt)) {
	    printf("--------------------------------------complete2.\n");
	    printresults(arr, tube_cnt);
	    return 0;
	}

	// after exec scence, try to find next layer 
	cnt = find_all(arr, tube_cnt, &next);
	if (!cnt) {
	    printf("this scence is block, try next scence.\n");
	    npour(&arr[n->flow[i].dest], &arr[n->flow[i].src], n->flow[i].cnt);
	    continue;
	} else {
	    printf("go ahead, find next layer.\n");
	    struct _tube *new_tubes = dup_tubes(arr, tube_cnt);
	    ret = find_next_layer(new_tubes, tube_cnt, &next);
	    if(ret < 0) {
		npour(&arr[n->flow[i].dest], &arr[n->flow[i].src], n->flow[i].cnt);
		printf("this road is block, return to last node.\n");
		// what should do here???
	    } else {
		printf("this road is a results, has been done.\n");
		return 0;
	    }
	}
    }
	    
    printf("emmmm.....\n");
    return -1;
}

#if 0
/* 递归实现 */
int sort(struct _tube arr[], int tube_cnt)
{
    int ret;
    int cnt = 0;
    struct _steps next; 
    memset(&next, 0, sizeof(struct _steps));

    if (is_complete(arr, tube_cnt)) {
	printf("It's a sorted arr, complete.\n");
	return 0;
    }

    cnt = find_all(arr, tube_cnt, &next);
    if (!cnt) {
	printf("It's a unsortable arr.\n");
	return -1;
    }

    /* next is the first layer steps. */
    ret = find_next_layer(arr, tube_cnt, &next);
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
	    printresults(arr, tube_cnt);
	    break;
	}
    }

    /* 打印最终结果 */
    while (s--)
    {
	stack_pop(&stack, &tp);
	printf("exec: %d -> %d, %d\n", tp.src, tp.dest, tp.cnt);
	npour(&arr[tp.dest], &arr[tp.src], tp.cnt);
	printresults(arr, tube_cnt);
    }
    //stack_print(&stack);
}
#endif

void hack_water(void)
{
    sort(g_tubes, g_tube_cnt);
}
