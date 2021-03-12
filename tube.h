#ifndef TUBE_H
#define TUBE_H

/* 管管属性 */
#define TUBE_FLOOR 4
#define TUBE_FULL TUBE_FLOOR
#define TUBE_EMPTY 0

#define MAX_TUBE_CNT 20

/* 一根管管 */
struct _tube {
    int status;
    int colors[TUBE_FLOOR];
};

struct _tube *dup_tubes(struct _tube tubes[], int size);
void free_tubes(struct _tube *tubes);
void printresults(struct _tube tubes[], int tube_cnt);
void printtube(struct _tube *tube);
void npour(struct _tube *src, struct _tube *dest, int n);
int pour(struct _tube *tube1, struct _tube *tube2, int reverse);
int is_complete(struct _tube arr[], int tube_cnt);
int swap_tube(struct _tube *src, struct _tube *dest);
int cal_place(struct _tube *t);
int cal_color(struct _tube *t);
int can_pour(struct _tube *src, struct _tube *dest);
int get_next_tube(int i, int tubecnt);
int find_empty(struct _tube arr[], int tube_cnt);
int get_top(struct _tube* t);

#endif
