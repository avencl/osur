#include <stdio.h>
#include <string.h>
#include <postavke.h>

void printMalo(const char *prefix)
{
	char p[DULJINA];

	strcpy ( p, prefix );
	strcat (p, " --printMalo");
	printf("%s\n", p);
	printManje(p);
	return;
}