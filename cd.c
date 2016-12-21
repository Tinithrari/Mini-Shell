#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cd.h"

//Recuperer le repertoire courant : getcwd(buffer, tailletab)
int cd (char * arbo)
{
	int b;

	if(arbo == NULL)
		return 1;
	
	b = chdir(arbo);
		
	if(b == -1)
	{
		perror("cd error");
		return 1;
	}
	
	else
		return 0;	
}
