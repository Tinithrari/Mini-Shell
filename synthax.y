%{
	#include <stdlib.h>
	#include <stdio.h>
	#include "Commande.h"
	#include "Job.h"
	#include "Sequence.h"

	int first = 1;
%}
%union {
	Sequence* command;
	char* string;
	Logique logic;
}
%type <command> Command
%token <command> commande
%token <string> option
%token <logic> sequence
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
       }

Command:
	Command sequence Command {
		linkSequence($1, $3, $2);
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
	yyparse();
}

void yyerror (char *s) 
{
	fprintf (stderr, "%s\n", s);
}

