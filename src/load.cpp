#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

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

#if 1
void printdata(cv::Mat &mat)
{

    int i=0, h=0, w=0, b=0,r=0,g=0;
    for (auto it = mat.begin<cv::Vec3b>(); it != mat.end<cv::Vec3b>(); ++it)
    {
	h=i/g_resize_width;
	w=i%g_resize_width;
	if (w == 0)
	    printf("\n %3d ", h);
	//std::cout << int((*it)[0]) << " " << int((*it)[1]) << " " << int((*it)[2]) << std::endl;
	b=int((*it)[0]);
	g=int((*it)[1]);
	r=int((*it)[2]);

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
	printrgb(r,g,b);
	i++;
    }
}
#endif

int get_a_color(cv::Vec3b *p, int cols, int *color)
{
    int dupcnt = 0;
    int r=0,g=0,b=0;
    int sumr=0, sumb=0, sumg=0;
    int bs_flag = 0;
    int sum_flag = 0;
    int i;

    for (i = 0; i < cols; i++)
    {
	cv::Vec3b &pix = *p++;
	b = pix[0];
	g = pix[1];
	r = pix[2];

	if (is_color_tolerant(r, 180) && is_color_tolerant(g, 180) && is_color_tolerant(b, 180)) {
	    if(sum_flag == 0 && bs_flag == 0) {
		bs_flag = 1;
		continue;
	    }

	    if (sum_flag == 1 && bs_flag == 1)
		goto out;
	} else {
	    if (1 == bs_flag) {
		sum_flag = 1;
		sumr += r;
		sumg += g;
		sumb += b;
		dupcnt++;
	    }
	}
    }

out:
    if(dupcnt)
    {
	r = sumr/dupcnt;
	g = sumg/dupcnt;
	b = sumb/dupcnt;
    }

    *color = (r<<16|g<<8|b);

    return i;
}

void get_color(cv::Mat &mat)
{
    int rows = mat.rows;
    int cols = mat.cols;
    int oldr=0, oldb=0, oldg=0;
    int row_data = g_resize_height / 4;
    int r,g,b;
    std::vector<int> valid_rows;
    std::vector<int> samplehline;
    int color;

    // filter valid_rows
    for (int i = row_data; i < rows; i++)
    {
	int maxmatch=0;
	int match = 0;

	cv::Vec3b *p = mat.ptr<cv::Vec3b>(i,0);
	for (int j = 0; j < cols; j++)
	{
	    cv::Vec3b &pix = *p++;
	    b = pix[0];
	    g = pix[1];
	    r = pix[2];

	    if(r<60&& g<60&& b<60) {
		pix[0]=pix[1]=pix[2]=0;
		if(match) {
		    //printf("match(%d) broken, i=%d\n", match,i);
		    match = 0;
		}
		continue;
	    }

	    if(oldr==0 && oldb==0 && oldg==0) {
		oldr=r;
		oldb=b;
		oldg=g;
	    }

	    if (is_color_tolerant(oldr, r) && is_color_tolerant(oldg, g) && is_color_tolerant(oldb, b)) {
		//printcolor((r<<16)|(g<<8)|b);
		match += 1;
		if(maxmatch < match)
		    maxmatch = match;
		continue;
	    }

	    if (match) {
		//printf("match(%d) broken, i=%d\n", match,i);
		match = 0;
	    }

	    oldr=r;
	    oldb=b;
	    oldg=g;
	}

	//printf("maxmatch(%d) = %d\n", maxmatch, i);
	if(maxmatch < 7)
	    continue;
	if(maxmatch > g_resize_width/20)
	{
	    valid_rows.push_back(i);
	}
    }

    // print valid rows.
    int i;
    for(i = 0; i < valid_rows.size(); i++)
    {
	printf("%d: ", valid_rows[i]);
	for(int j=0;j<cols;j++)
	{
	    cv::Vec3b *p = mat.ptr<cv::Vec3b>(valid_rows[i],j);
	    cv::Vec3b &pix = *p;
	    b = pix[0];
	    g = pix[1];
	    r = pix[2];
	    printcolor((r<<16)|(g<<8)|b);
	}
	printf("\n");
    }
    printf("There are %d rows valid data.\n", i);

#if 0
    // 指定横向扫描线
    std::vector<int> start;
    int scan_step;
    int step = valid_rows.size() / TUBE_FLOOR / 2;
    int inst=0;
    for(i = 1; i < valid_rows.size(); i++)
    {
	inst = valid_rows[i]-valid_rows[i-1];
	if (inst > g_resize_height/60)
	{
	    start.push_back(valid_rows[i-1]);
	    start.push_back(valid_rows[i]);

	}
    }
    sort( start.begin(), start.end() );
    start.erase( unique( start.begin(), start.end() ), start.end() );
    int testrow;
    for(i = 1; i < valid_rows.size(); i++)
    {
	testrow = (start[i]+start[i-1])/2;
	for
    }

    for(i=0;i<start.size();i++)
    {
	printf("start[%d] = %d\n", i,start[i]);
    }
#endif
    std::vector<int> line;

    line.push_back(98);
    line.push_back(98+22*1);
    line.push_back(98+22*2);
    line.push_back(98+22*3);
    line.push_back(177);
    line.push_back(177+22*1);
    line.push_back(177+22*2);
    line.push_back(177+22*3);
    int startp = valid_rows.size() / TUBE_FLOOR / 4;
    int k=0;
    for(i=0;i<line.size();i++)
    {
	printf("%d: ", valid_rows[line[i]]);
	for(int j=0;j<cols;j++)
	{
	    cv::Vec3b *p = mat.ptr<cv::Vec3b>(valid_rows[line[i]],j);
	    cv::Vec3b &pix = *p;
	    b = pix[0];
	    g = pix[1];
	    r = pix[2];
	    printcolor((r<<16)|(g<<8)|b);
	}
	printf("\n");
	k++;
    }


    k=0;
    int bs_flag=0;
    int be_flag=0;
    int match=0;
    int pos=0;
    std::vector<int> colors;
    int cnt[3*TUBE_FLOOR] = {0};
    for(i=0;i<line.size();i++)
    {
	cv::Vec3b *p = mat.ptr<cv::Vec3b>(valid_rows[line[i]],0);
	pos=0;
	bs_flag = 0;
	do {
	    match = get_a_color(p, cols-pos, &color);
	    cnt[k]+=1;
	    r = color >> 16 & 0xff;
	    g = color >> 8 & 0xff;
	    b = color & 0xff;

	    printcolor(color);
	    printf("this is get color match = %d\n", match);
	    colors.push_back(color);
	    p += match;
	    pos += match;
	} while(cols - pos > 0);
	printf("\n");
	k++;
    }
    printf("colors = %d\n", colors.size());

    printf("k0=%d, k1=%d, k2=%d\n", cnt[0], cnt[1],cnt[2]);
    std::vector<int>::iterator it;
    int last;
    int init=0;
    for(it=colors.begin();it!=colors.end();)
    {
	printcolor(*it);
	if (0==init) {
	    init =1;
	    it++;
	} else {
	    init = 0;
	    colors.erase(it);
	}
    }

    for (i=0;i<colors.size();i++)
    {
	if (i%7 == 0)
	    printf("\n");
	printcolor(colors[i]);
    }

    g_tube_cnt = colors.size()/TUBE_FLOOR;
    printf("g_tube_cnt = %d cnt = %d\n", g_tube_cnt, cnt[0]+cnt[1]+cnt[2]);
    g_tubes = (struct _tube*)malloc(g_tube_cnt * sizeof(struct _tube));
    int c,l;
    int index = 0;
    int bnt = 0;
    int offset = 0;
    for (k = 0; k < 12; k+=4)
    {
	if (cnt[k] > 0)
	{
	    b = cnt[k]/2; //7
	    for(int i = 0; i < b*4;i++)
	    {
		c = (index-offset)%b + offset/TUBE_FLOOR;
		l = (index-offset)/b;
		printf("index = %d, c= %d , l=%d\n", index, c, l);
		g_tubes[c].colors[TUBE_FLOOR-l-1] = colors[index];
		index++;
	    }
	    offset+=4*b;
	}
    }
}

static void load_map_from_picture(void)
{
    g_use_rgb=1;
    char pic[256]={0}; //"webwxgetmsgimg.jpg";
    int i=0, h=0,w=0,c,l, r,g,b;
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

    cv::Mat mat1 = cv::imread(pic);
    g_resize_height = mat1.rows;
    g_resize_width = mat1.cols;
    printf("height = %d, width = %d\n", g_resize_height, g_resize_width);

    for(i=16;i>0;i--)
    {
	if (g_resize_width & (0x1 << i))
	    break;
    }
    printf("i=%d\n", i);
    g_resize_width >>= (i-7);
    g_resize_height = g_resize_width*(double)(mat1.rows/mat1.cols);

    printf("height = %d, width = %d\n", g_resize_height, g_resize_width);

    cv::Mat mat2(g_resize_height, g_resize_width, CV_8UC3);

    cv::resize(mat1, mat2, mat2.size(), 0,0, cv::INTER_LINEAR);

    //printdata(mat2);

    get_color(mat2);

    for(i=0;i<14;i++)
    {
	format_tube(&g_tubes[i]);
	printtube(&g_tubes[i]);
    }

    printresults(g_tubes, g_tube_cnt);
    show_choice();
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

void load_map_water_puzzle(void)
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

void load_map(int type)
{
    switch(type) {
	case 0:
	    load_map_water_puzzle();
	    break;
	case 1:
	    break;
	case 2:
	    break;
	case 3:
	    break;
    }
}
