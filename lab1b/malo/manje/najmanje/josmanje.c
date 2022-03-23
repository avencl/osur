#include <stdio.h>
#include <postavke.h>
#include <string.h>

void josManje(const char *text) 
{
	char p[DULJINA];

	strcpy ( p, text );
	strcat ( p, " --josManje" );
	printf("%s\n", p);
	return;
}