#include "argTable.h"

#include <stdlib.h>
#include <string.h>
#define EXTRA_NB 1

argTable cutString(char* string)
{
	argTable tab;
	char *token;
	int cpt = 0, length;


	// if the pointer or the string is empty, return NULL
	if (! string || ! strlen(string))
		return NULL;
	
	length = strlen(string);

	// Try to extract the first argument from the string
	token = strtok(string, " ");

	// If there are no token, there is just one element
	if (! token)
		cpt++;
	else
	{
		int i;
		// Counting number of elements in the string
		for (;token;)
		{
			cpt++;
			token = strtok(NULL, " ");
		}

		// Reinitialize string
		for (i = 0; i < length; i++)
			if ( *(string + i) == '\0')
				*(string + i) = ' ';

		token = strtok(string, " ");
	}
	
	// Allocate a new array with the number of element, adding some extra space for extra elements
	tab = (argTable) malloc( sizeof(char*) * (cpt + EXTRA_NB) );

	// if allocation fail, return NULL
	if (! tab) 
	{
		return NULL;
	}

	// Add NULL a the end of the array
	(*(tab + cpt)) = NULL;

	// if there are no token, just copy the string in the table.
	if (! token)
	{
		// allocation of the size of the string and an extra slot for the \0
		*tab = (char*) malloc( sizeof(char) * (strlen(string) + 1));
		
		// if allocation fail, free the table and return NULL
		if (! *(tab) )
		{
			freeArgTable(tab);
			return NULL;
		}

		// Copy the string into the table
		strcpy(*tab, string);
	}
	else
	{
		int i;
		// Adding each token into the table
		for (i = 0; i < cpt; i++)
		{
			// Allocate the size of token
			(*(tab + i)) = (char*) malloc( sizeof(char) * ( strlen(token) + 1) );

			// if allocation fail, free the table and return NULL
			if ( ! (*(tab + i) ) )
			{
				freeArgTable(tab);
				return NULL;
			}

			// Copy the string into the table
			strcpy( *(tab + i), token );
			token = strtok(NULL, " ");
		}
	}
	return tab;
}


void freeArgTable(argTable tab)
{
	argTable ptr = tab;

	// free each slot of memory
	for (;*(ptr); ptr++)
		free(*ptr);

	// free the table
	free(tab);
}
