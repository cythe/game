#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "load.h"
#include "water.h"

int get_puzzle_type(char *s)
{
    if(strcmp(s, "water"))
	return 0;
    if(strcmp(s, "stars"))
	return 1;

    return -1;
}

int main(int argc, char* argv[])
{
    int type;

    type = get_puzzle_type(argv[0]);

    printf("type = %d\n", type);
    
    load_map(type);

    switch (type) {
	case 0:
	    hack_water();
	    break;
	case 1:
	    //hack_stars();
	    break;
	default:
	    printf("WTF?\n");
	    break;
    }

    return 0;
}
