#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "basetype.h"
#include "load.h"
#include "color.h"
#include "tube.h"

struct _choiced g_choiced;
struct _tube * g_tubes;
int g_tube_cnt = 0;

struct _tube tubes[] = {
#if 0
    /* case 1 */
    {4, 38, 84, 9,  9},
    {4, 9,  9,  84, 38},
    {4, 84,  84,  38, 38},
    {0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
#endif
#if 0
    /* case 2 */
    {4, 38, 9,  9, 9},
    {4, 84, 9,  84, 38},
    {4, 84,  84,  38, 38},
    {0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
#endif
#if 1
    /* case 117 */
    {4, 84, 22, 93,  38},
    {4, 218,   240,   84, 21},
    {4, 38,  21,  38,  21},
    {4, 22, 93,  214, 9},
    {4, 38,  9,    214, 84},
    {4, 22, 214, 226, 9},
    {4, 93,  9,    79,  79},
    {4, 84, 79, 240,    79},
    {4, 226, 94, 21, 226},
    {4, 218, 214, 93, 94},
    {4, 240, 226, 94, 240},
    {4, 218, 22, 94, 218},
    {0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
    {0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
#endif
};

void load_map_from_file(void) // cfg.txt
{

}

static void init_choiced(void)
{
    memset(&g_choiced, 0, sizeof(struct _choiced));
}

static void deinit_choiced(void)
{
    memset(&g_choiced, 0, sizeof(struct _choiced));
}

static void show_choice(void)
{
    for (int i = 0; i < g_choiced.cnt; i++)
	printcolor(g_choiced.colors[i]);
    printf("\n");
}

static int is_not_exist(int color)
{
    for (int i = 0; i < g_choiced.cnt; i++)
	if (color == g_choiced.colors[i])
	    return 0;

    return 1;
}

void format_tube(struct _tube *t)
{
    struct _tube *tmp = dup_tubes(t, 1);
    int j = 0;

    memset(t, 0, sizeof(struct _tube));
    for (int i = 0; i < TUBE_FLOOR; i++)
    {
	if(tmp->colors[i]) {
	    t->colors[j] = tmp->colors[i];
	    j++;
	}
    }

    t->status = j;

    free(tmp);
}

static void load_map_from_stdin(void)
{
    int color;
    char ch;
    char s[1024];

    printf("Please input tube cnt: ");
    fflush(stdout);
    scanf("%d", &g_tube_cnt); 
    g_tubes = (struct _tube*)malloc(g_tube_cnt * sizeof(struct _tube));

    for(int i = 0; i < g_tube_cnt; i++)
    {
retry:
	for (int j = 0; j < TUBE_FLOOR;) 
	{
	    while(1)
	    {
		show_choice();
		printf("choice a color, [-1] show all colors: ");
		fflush(stdout);
		scanf("%d", &color);
		if (color < 0) {
		    print_std256color();
		    continue;
		}

		g_tubes[i].colors[TUBE_FLOOR-j-1] = color;
		j++;
		if (is_not_exist(color))
		{
		    g_choiced.colors[g_choiced.cnt] = color;
		    g_choiced.cnt++ ;
		}
		break;
	    }
	}

	format_tube(&g_tubes[i]);
	printtube(&g_tubes[i]);
	printf("ok? [y/n] ");
	fflush(stdout);
	getchar();
	scanf("%c", &ch);
	printf("ch = %c\n", ch);
	if (ch == 'n')
	    goto retry;
    }

    printresults(g_tubes, g_tube_cnt);
}

void load_map(void)
{
    int type;

    init_choiced();
    printf("------- Select map type -------\n");
    printf("\t1 - from stdin\n");
    printf("\t2 - from file\n");
    printf("\t3 - from inside case\n");
    printf("Please input map type: ");
    fflush(stdout);
    scanf("%d", &type);
    switch(type)
    {
	case 1:
	    load_map_from_stdin();
	    break;
	case 2:
	    load_map_from_file();
	    break;
	case 3:
	    g_tubes = tubes;
	    g_tube_cnt = sizeof(tubes) / sizeof(tubes[0]);
	    printf("tube_cnt = %d\n", g_tube_cnt);
	    break;
	default:
	    printf("Get out !\n");
	    exit(-1);
    }
    deinit_choiced();
}
