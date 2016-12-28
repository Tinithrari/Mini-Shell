%{
	#include <stdlib.h>
	#include <stdio.h>
	#include <unistd.h>
	#include <string.h>

	#include "Commande.h"
	#include "Job.h"
	#include "Sequence.h"
	#include "Redirection.h"

	#define CWD_SIZE 1024
	#define USERNAME_SIZE 1024
	
	#ifndef HOST_NAME_MAX
	#define HOST_NAME_MAX 64
	#endif

	#define HOME_OFFSET 6

	int first = 1;

	void prompt()
	{
		char hostname[HOST_NAME_MAX + 1];
		char cwd[CWD_SIZE], tmpCwd[CWD_SIZE];
		char buffer[HOME_OFFSET + USERNAME_SIZE];
		int i, usrlen;

		gethostname(hostname, HOST_NAME_MAX);
		getcwd(cwd, sizeof(char) * CWD_SIZE);
		strcpy(tmpCwd, cwd);

		sprintf(buffer, "/home/%s", getenv("LOGNAME"));
		usrlen = strlen(getenv("LOGNAME"));

		if (strstr(cwd, buffer) != NULL)
		{
			cwd[0] = '~';
			for (i = 0; i < strlen(tmpCwd) - strlen(buffer) + 1; i++)
				cwd[i + 1] = tmpCwd[strlen(buffer) + i]; 
		}
		cwd[i] = '\0';

		printf("\x1B[32m%s@%s\x1B[0m:\x1B[34m%s\x1B[0m>", getenv("LOGNAME"), hostname, cwd);
		fflush(stdout);
	}
%}
%union {
	Sequence* command;
	char* string;
	Logique logic;
	Flow flux;
}
%type <command> Command
%token <command> commande
%token <string> option
%token <string> fichier
%token <logic> sequence
%token <flux> flow
%start debut
%%
debut:
     	line debut{}
	|line {}
	;

line : Command '\n' {
		executeSequence($1);
		deleteSequence($1);
		first = 1;
		prompt();
       }

Command:
	Command sequence Command {
		linkSequence($1, $3, $2);
		$$ = $1;
	}
	| Command '&' {
		$1->c->background = 1;
		$$ = $1;
	}
	| Command flow Command {
		pipeCommande($1->c, $3->c);
		linkSequence($1, $3, NONE);
		$$ = $1;
	}
	| Command flow fichier {
		switch ($2)
		{
			case IN:
				setInCommande($1->c, $3);
			break;
			case OUT:
				setOutCommande($1->c, $3, 0);
			break;
			case ERR:
				setErrCommande($1->c, $3, 0);
			break;
			case ERROUT:
				setOutCommande($1->c, $3, 0);
				setErrCommande($1->c, $3, 0);
			break;
			case APPOUT:
				setOutCommande($1->c, $3, 1);
			break;
			case APPERR:
				setOutCommande($1->c, $3, 1);
			break;
		}
		$$ = $1;
	}
	| Command option {
		addOptionCommande($1->c, $2);
		$$ = $1;
	}
	| commande {
		$$ = $1;
	}
	;
%%

int main (void) 
{
	prompt();
	yyparse();
}

void yyerror (char *s) 
{
	fprintf (stderr, "%s\n", s);
}

