#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "cd.h"

//Recuperer le repertoire courant : getcwd(buffer, tailletab)
int cd (char * arbo)
{
	int b;
	char path[1024];
	int modified = 0;
	
	if(arbo == NULL)
		return 1;

	if (strstr(arbo, "~") != NULL)
	{
		sprintf(path, "/home/%s", getenv("LOGNAME"));
		strcat(path, arbo + 1);
		modified = 1;
	}
	
	b = chdir(modified ? path : arbo);
		
	if(b == -1)
	{
		perror("cd error");
		return 1;
	}
	
	else
		return 0;	
}
