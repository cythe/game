#ifndef BASETYPE_H
#define BASETYPE_H

#include "tube.h"

#define MAX_STEP 1024
/* 每步最大可能 */
#define MAYBE ( MAX_TUBE_CNT * (MAX_TUBE_CNT-1)) 

struct _from_to {
    int src;
    int dest;
    int cnt;
};

struct _steps {
    int scence;
    int cnt;
    struct _from_to flow[MAYBE];
};

#endif
