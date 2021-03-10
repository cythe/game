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

int stack_push(struct _stack *me, struct _pair d)
{
    if (is_stack_full(me))
	return -1;

    me->data[me->depth].src = d.src; 
    me->data[me->depth].dest = d.dest; 
    me->depth++ ;

    return 0;
}

int stack_pop(struct _stack *me, struct _pair *d)
{
    if (is_stack_empty(me))
	return -1;

    d->src = me->data[me->depth-1].src;
    d->dest = me->data[me->depth-1].dest; 
    me->depth-- ;

    return 0;
}

void stack_print(struct _stack *me)
{
    printf("stack: \n");
    for (int i = 0; i < me->depth; i++)
    {
	printf("[ %d -> %d ]\n",
		me->data[me->depth - i - 1].src,
		me->data[me->depth - i - 1].dest);
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

