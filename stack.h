#ifndef STACK_H
#define STACK_H

#include "basetype.h"

#define STACK_COUNT 1
#define STACK_DEPTH 5

struct _stack {
    struct _pair data[STACK_DEPTH];
    int depth; 
};

void stack_init(struct _stack *me);
void stack_deinit(struct _stack *me);
int stack_push(struct _stack *me, struct _pair d);
int stack_pop(struct _stack *me, struct _pair *d);
void stack_print(struct _stack *me);

#endif // STACK_H
