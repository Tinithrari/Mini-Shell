#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define syserror(s,x) perror(s), exit(x);
#define fatalerror(s,x) fprintf(stderr, "%s\n", s), exit(x);

#define BUFFER_SIZE 256
#define ERR -1

/**
 *@fn static void freeTab(char **tab)
 *@brief Libère un tableau alloué avec cutStr
 *@param un tableau de chaine de caractères alloué avec cutStr
 */
static void freeTab(char **tab)
{
	char **ptr = tab;
	int i;

	if (tab == NULL)
		return;

	for (i = 0 ;*(ptr + i); i++)
		free( *(ptr + i) );

	free(ptr);
}

/**
 *@fn static char** cutStr(char * str)
 *@brief Décompose une chaine de caractère en un tableau se terminant par NULL
 *@param str Chaine de caractère non nul à découper
 *@return un tableau contenant la chaine de caractère découpée et se terminant par NULL
 */
static char** cutStr(char * str)
{
	// TODO: Ajouter un délimiteur
	char *token, **tab;
	int cpt = 0, i;

	if ( ! str || ! strlen(str) )
		return NULL;

	token = strtok(str, " ");

	if (token == NULL)
	{
		cpt++;
	}
	else
	{
		for (;! token;)
		{
			cpt++;
			token = strtok(NULL," ");
		}
	}
	tab = (char**) malloc( sizeof(char*) * (cpt + 1) );

	if (! tab)
		fatalerror("malloc error 1", 2);

	*( tab + cpt ) = NULL;
	
	if (token)
	{
		for (i = 0, token = strtok(str, " "); i < cpt; i++)
		{
			*(tab + i) = (char*) malloc( sizeof(char) * ( strlen(token) + 1 ) );
		
			if (*(tab + i) == NULL)
			{
				freeTab(tab);
				fatalerror("malloc error 2", 2);
			}
		
			strcpy( *(tab + i), token );
			token = strtok(NULL, " ");
		}
	}
	else
	{
		*tab = (char*) malloc( sizeof(char) * ( strlen(str) + 1 ) );

		if (! (*tab))
		{
			freeTab(tab);
			fatalerror("malloc error 3", 2);
		}

		strcpy(*tab, str);
	}
	
	return tab;
}

int main(void)
{
	pid_t pid;	

	do
	{

		char buffer[BUFFER_SIZE];
		char** tab;

		printf(">");
		scanf("%s", buffer);

		if (*buffer == '\n')
		{
			printf("lolilol\n");
			getchar();
			continue;
		}

		tab = cutStr(buffer);
		free(tab);
		/*
		if ( (pid = fork()) == ERR)
			syserror("Fork error", 1);

		if (! pid)
		{
			
		}
		*/
	}while (getchar() != EOF);


}
