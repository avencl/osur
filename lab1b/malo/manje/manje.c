#include <stdio.h>
#include <postavke.h>
#include <string.h>

void printManje(const char *text)
{
	char p[DULJINA];

	strcpy ( p, text );
	strcat ( p, " --printManje" );
	printf("%s\n", p);
	// printf("ovo je var varijabla %d"" \n", VAR1);
	printNajmanje(p);
	josManje(p);
	return;
}