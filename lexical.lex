%{
	#include <stdlib.h>
	#include <string.h>
	#include "struct/LinkedList.h"
	#include "Commande.h"
	#include "Job.h"
	#include "Sequence.h"
	#include "Redirection.h"
	#include "y.tab.h"

	#define and "&&"
	#define or "||"
	#define none ";"

	#define out ">"
	#define appout ">>"
	#define in "<"
	#define err "2>"
	#define apperr "2>>"
	#define errout "2>&1"

	extern int first;
	int flux = 0;
%}
NORMAL    [^ \t\$\*\?\n\\\[\]\^\&\|;><]
SPECIAL    [ \t\$\*\?\n\\\[\]\^\&\|;]
VARIABLE    \$([a-zA-Z]|-|_)([0-9a-zA-Z]|-|_)*
SIMPLEQUOTED     '(\\'|[^'])+'
DOUBLEQUOTED    \"(\\\"|[^\"])*\"
STRING    ((\\{SPECIAL}|{NORMAL})|{SIMPLEQUOTED}|{DOUBLEQUOTED})+
SEQUENCE    (\|\||\&\&|;)
FLUX    (\||>|>>|<|2>|2>>|2>\&1)
BACKGROUND_MARK    \&
%%
{VARIABLE} {
	yylval.string = yytext;
	return variable;
}
{BACKGROUND_MARK} {return yytext[0];}

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
		if (flux)
		{
			flux = 0;
			return fichier;
		} 
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
{FLUX} {
	flux = 1;
	if (! strcmp(yytext, in))
		yylval.flux = IN;
	else if (! strcmp(yytext, out))
		yylval.flux = OUT;
	else if (! strcmp(yytext, err))
		yylval.flux = ERR;
	else if (! strcmp(yytext, errout))
		yylval.flux = ERROUT;
	else if (! strcmp(yytext, appout))
		yylval.flux = APPOUT;
	else if (! strcmp(yytext, errout))
		yylval.flux = APPERR;
	else
	{
		yylval.flux = PIPE;
		flux = 0;
		first = 1;
	}
	return flow;
}
[ \t]    ;
\n	return yytext[0];

%%
int yywrap (void) {return 1;}
