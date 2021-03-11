#ifndef STACK_H
#define STACK_H

#include "basetype.h"

#define STACK_COUNT 1
#define STACK_DEPTH 1024

struct _stack {
    struct _from_to data[STACK_DEPTH];
    int depth; 
    int max_depth;
};

void stack_init(struct _stack *me);
void stack_deinit(struct _stack *me);
int stack_push(struct _stack *me, struct _from_to d);
int stack_pop(struct _stack *me, struct _from_to *d);
void stack_print(struct _stack *me);

#endif // STACK_H
