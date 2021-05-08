#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

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

static int is_not_exist(int color)
{
    for (int i = 0; i < g_choiced.cnt; i++)
	if (color == g_choiced.colors[i])
	    return 0;

    return 1;
}

static void add_color(int color)
{
    if (is_not_exist(color))
    {
	g_choiced.colors[g_choiced.cnt] = color;
	g_choiced.cnt ++;
    }
}

static void show_choice(void)
{
    for (int i = 0; i < g_choiced.cnt; i++)
	printcolor(g_choiced.colors[i]);
    printf("\n");
}

static void init_choiced(void)
{
    int color;
    int ch;
    memset(&g_choiced, 0, sizeof(struct _choiced));

    do{
	show_choice();
	printf("==== init color panton ====\n"
		"In this section, you can select colors you may use.\n"
		"You also can pick colors in next step with a little tedious.\n"
		"[ -1]: show all colors\n"
		"[999]: End\n"
		"Choice a color: ");
	fflush(stdout);

	scanf("%d", &color);
	if (color < 0) {
	    print_std256color();
	    continue;
	}
	if (color > 256) {
	    break;
	}

	if (is_not_exist(color))
	{
	    g_choiced.colors[g_choiced.cnt] = color;
	    g_choiced.cnt++ ;
	}
    } while(1);
}

static void deinit_choiced(void)
{
    memset(&g_choiced, 0, sizeof(struct _choiced));
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

    free_tubes(tmp);
}

static void cal_tube_cnt(void)
{
    printf("Please input tube cnt (include empty tube): ");
    fflush(stdout);
    scanf("%d", &g_tube_cnt); 
    printf("tube_cnt = %d\n", g_tube_cnt);
    g_tube_cnt = 14;
}

static void load_map_from_picture(void)
{
    g_use_rgb=1;
    char pic[256]={0}; //"webwxgetmsgimg.jpg";
    int i=0, j=0,k=0,c,l, r,g,b;
    int color_cnt;
    int color;
    char ch;
	FILE* fp = NULL;

    printf("Please input file path: ");
    fflush(stdout);
    ch = getchar();
    fgets(pic, sizeof(pic), stdin);
    pic[strlen(pic)-1]='\0';
    printf("We will read map from [%s]\n", pic);
    
    printf("Calculate tube_cnt...\n");
    cal_tube_cnt();

    g_tubes = (struct _tube*)malloc(g_tube_cnt * sizeof(struct _tube));

    cv::Mat mat1 = cv::imread(pic);
    cv::Mat mat2(64,64,CV_8UC3);

    cv::resize(mat1, mat2, mat2.size(), 0,0);

    for (auto it = mat2.begin<cv::Vec3b>(); it != mat2.end<cv::Vec3b>(); ++it)
    {
	j=i/64;
	k=i%64;
	//std::cout << int((*it)[0]) << " " << int((*it)[1]) << " " << int((*it)[2]) << std::endl;
	b=int((*it)[0]);
	g=int((*it)[1]);
	r=int((*it)[2]);
	if (j==21 || j==24|| j==27|| j==30|| j==40|| j==43|| j==46|| j==49)	
	{
	    if (k==3 || k==12 || k==21 || k==30 || k==39 || k==48 || k==57)
	    {
		if(r<60&& g<60&& b<60) {
		    r=0;g=0;b=0;
		}
#if 1
		r=r&~0xf;
		g=g&~0xf;
		b=b&~0xf;
#else
		r=r&~0x3;
		g=g&~0x3;
		b=b&~0x3;
#endif
		//printrgb(r,g,b,0);
		add_color((r<<16)|(g<<8)|b);
		c = (k-3)/9;
		if(j>=40)
		    c+=7;
		l = j<40 ? (j-21)/3 : (j-40)/3;
		g_tubes[c].colors[TUBE_FLOOR-l-1] = (r<<16)|(g<<8)|b;
		printf("c=%d, l=%d\n", c,l);
	    }
	    if (k == 0)
		printf("\n %d ", j);
	}
	i++;
    }

    for(i=0;i<14;i++)
    {
	format_tube(&g_tubes[i]);
	printtube(&g_tubes[i]);
    }

    printresults(g_tubes, g_tube_cnt);
}

static void load_map_from_stdin(void)
{
    int color_cnt;
    int color;
    char ch;
	int j = 0;

    printf("Please input tube cnt (include empty tube): ");
    fflush(stdout);
    scanf("%d", &g_tube_cnt); 
    g_tubes = (struct _tube*)malloc(g_tube_cnt * sizeof(struct _tube));

    for(int i = 0; i < g_tube_cnt; i++)
    {
repick:
	j = 0;
	show_choice();
	printf("==== choice color for tube[%d] ====\n"
		"format: [color1 color2 color3 color4]\n"
		"         top ---------------> bottom\n"
		"[ -1]: show all colors\n"
		"[ 0 ]: for NULL empty tube is [0 0 0 0] \n"
		"choice color: ", i);
	fflush(stdout);

	while(j < TUBE_FLOOR)
	{
	    scanf("%d", &color);
	    if (color < 0) {
		print_std256color();
		goto repick;
	//	continue;
	    }

	    g_tubes[i].colors[TUBE_FLOOR-j-1] = color;
	    j++;
	    if (is_not_exist(color))
	    {
		printf("This color is not exist at list!!!\n");
		color_cnt = g_choiced.cnt;
		g_choiced.colors[g_choiced.cnt] = color;
		g_choiced.cnt++ ;
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
	{
	    color_cnt = g_choiced.cnt;
	    goto repick;
	}
    }

    printresults(g_tubes, g_tube_cnt);
}

void load_map(void)
{
    int type;

    init_choiced();
    printf("------- Select map type -------\n");
    printf("\t1 - from stdin\n");
    printf("\t2 - from picture\n");
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
	    load_map_from_picture();
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
