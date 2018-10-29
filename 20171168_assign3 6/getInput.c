#include "globals.h"
char* readInput()
{
	char* line=NULL;
	size_t size=0;
	if(getline(&line,&size,stdin)==-1)
	{
		printf("getline failed\n");
	}
	return line;
}
