#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "argTable.h"

#define syserror(s,x) perror(s), exit(x);
#define fatalerror(s,x) fprintf(stderr, "%s\n", s), exit(x);

#define BUFFER_SIZE 256
#define ERR -1

//ceci est un main

int main(void)
{
	pid_t pid;	
	int status;
	
	do
	{

		char buffer[BUFFER_SIZE];
		argTable tab;

		printf(">");
		fflush(stdout);

		scanf("%256[^\n]", buffer);		

		if (buffer == NULL || ! strlen(buffer))
			break;

		tab = cutString(buffer);		

		if ( (pid = fork()) == ERR)
			syserror("fork error", 1);

		if (! pid)
		{
			execvp(*(tab), tab);
			syserror("Exec error", 2);
		}	
		
		wait(&status);

		if (WIFEXITED(status))
			printf("Child process exited with code %d\n", WEXITSTATUS(status));
		else
			printf("Child process killed by signal\n");

		freeArgTable(tab);

	} while (getchar() != EOF);

}
