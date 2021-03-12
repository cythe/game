#ifndef LOAD_H
#define LOAD_H

struct _choiced {
    int cnt;
    int colors[1024];
}; 

extern struct _choiced g_choiced;
extern struct _tube * g_tubes;
extern int g_tube_cnt;

void load_map(void);
#endif
