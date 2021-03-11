#ifndef BASETYPE_H
#define BASETYPE_H

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

/* 管管 */
#define TUBE_FLOOR 4
#define TUBE_FULL TUBE_FLOOR
#define TUBE_EMPTY 0

#define MAX_TUBE_CNT 20
/* 每步最大可能 */
#define MAYBE ( MAX_TUBE_CNT * (MAX_TUBE_CNT-1)) 

/* 一根管管 */
struct _tube {
    int status;
    int colors[TUBE_FLOOR];
};

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
