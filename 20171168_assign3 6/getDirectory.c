#include "globals.h"
char * getHomeDirectory()
{
	size_t size=40000;
	char * tempdir=(char*)malloc(size);
	tempdir=getcwd(tempdir,size);
	return tempdir;
}
char* giveDirectory(char* home)
{
	size_t size=40000;
	char * tempdir=(char*)malloc(size);
	tempdir=getcwd(tempdir,size);
	int i=0;
	char* currdir=(char*)malloc(size);
	while(i <= strlen(tempdir)&&i <= strlen(home)&&tempdir[i]==home[i]&&home[i]!='\0'&&tempdir[i]!='\0')
		++i;
	if (i==strlen(home))
	{
		currdir[0]='~';
		for (int j = i; j <strlen(tempdir) ; ++j)
		{
			currdir[j-i+1]=tempdir[j];
		}
	}
	else
	{
		currdir=tempdir;
	}
	return currdir;
}
