#include <stdio.h>
#include <postavke.h>
#include <string.h>

void printNajmanje(const char *text) 
{
	char p[DULJINA];

	strcpy ( p, text );
	strcat ( p, " --printNajmanje" );
	printf("%s\n", p);
	return;
}