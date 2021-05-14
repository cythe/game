#ifndef COLOR_H
#define COLOR_H

enum E_COLOR {
    COL_NULL=0,
};

int is_color_tolerant(int a, int b);
void printrgb(int r, int g, int b);
void printcolor(int c);
void print_std256color(void);

#endif
