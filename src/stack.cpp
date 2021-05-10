#include <stdio.h>
#include <string.h>
#include "stack.h"

static int is_stack_empty(struct _stack *me)
{
    return me->depth ? 0 : 1;
}

static int is_stack_full(struct _stack *me)
{
    return me->depth < STACK_DEPTH ? 0 : 1;
}

int stack_push(struct _stack *me, struct _from_to d)
{
    if (is_stack_full(me))
    {
	printf("stack is full\n");
	return -1;
    }

    me->data[me->depth].src = d.src; 
    me->data[me->depth].dest = d.dest; 
    me->data[me->depth].cnt = d.cnt; 
    me->depth++ ;

    if (me->max_depth < me->depth)
	me->max_depth = me->depth;

    return 0;
}

int stack_pop(struct _stack *me, struct _from_to *d)
{
    if (is_stack_empty(me))
    {
	printf("stack is full\n");
	return -1;
    }

    d->src = me->data[me->depth-1].src;
    d->dest = me->data[me->depth-1].dest; 
    d->cnt = me->data[me->depth-1].cnt;
    me->data[me->depth-1].src = 0;
    me->data[me->depth-1].dest = 0;
    me->data[me->depth-1].cnt = 0;
    me->depth-- ;
    return 0;
}

void stack_print(struct _stack *me)
{
    printf("stack: depth=%d, max_depth=%d, STACK_DEPTH=%d\n", me->depth, me->max_depth, STACK_DEPTH);
    for (int i = 0; i < me->depth; i++)
    {
	printf("[%2d -> %2d %2d]\n",
		me->data[me->depth - i - 1].src,
		me->data[me->depth - i - 1].dest,
		me->data[me->depth - i - 1].cnt);
    }
}
#if 0
static void stack_init(void) __attribute__((constructor));
static void stack_deinit(void) __attribute__((destructor));
#endif

void stack_deinit(struct _stack *me)
{
    printf("%d@%s\n", __LINE__,__func__);
    memset(me, 0, sizeof(struct _stack));
}

void stack_init(struct _stack *me)
{
    printf("%d@%s\n", __LINE__,__func__);
    memset(me, 0, sizeof(struct _stack));
}

