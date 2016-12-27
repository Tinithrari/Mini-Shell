%{
	#include <stdlib.h>
	#include <string.h>
	#include "struct/LinkedList.h"
	#include "Commande.h"
	#include "Job.h"
	#include "Sequence.h"
	#include "y.tab.h"

	#define and "&&"
	#define or "||"
	#define none ";"

	extern int first;
%}
NORMAL    [^ \t\$\*\?\n\\\[\]\^\&\|;]
SPECIAL    [ \t\$\*\?\n\\\[\]\^\&\|;]
SIMPLEQUOTED     '(\\'|[^'])+'
DOUBLEQUOTED    \"(\\\"|[^\"])*\"
STRING    ((\\{SPECIAL}|{NORMAL})|{SIMPLEQUOTED}|{DOUBLEQUOTED})+
SEQUENCE    (\|\||\&\&|;)
%%
{STRING}   {
	if (first)
	{
		Commande *c = newCommande(yytext);
		yylval.command = newSequence(c);
		first = 0;
		return commande;
	}
	else
	{
		yylval.string = yytext;
		return option;
	}
}
{SEQUENCE} {
	if (! strcmp(yytext, and))
		yylval.logic = AND;
	else if (! strcmp(yytext, or))
		yylval.logic = OR;
	else
		yylval.logic = NONE;
	first = 1;
	return sequence;
}	
[ \t]    ;
\n	return yytext[0];

%%
int yywrap (void) {return 1;}
