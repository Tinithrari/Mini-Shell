%{
	#include <stdlib.h>
	#include <string.h>
	#include "struct/LinkedList.h"
	#include "Commande.h"
	#include "Job.h"
	#include "Sequence.h"
	#include "Redirection.h"
	#include "y.tab.h"

	#define BASE_SIZE 2

	#define and "&&"
	#define or "||"
	#define none ";"

	#define out ">"
	#define appout ">>"
	#define in "<"
	#define err "2>"
	#define apperr "2>>"
	#define errout ">&"
	#define apperrout ">>&"

	extern int first;
	int flux = 0;

	char* traitementString(char *str)
	{
		int i, nbChar;
		char protect = '\0', *newStr = NULL;

		if (str == NULL)
			return NULL;

		for (i = 0, nbChar = 0; i < strlen(str); i++)
		{
			if (str[i] == '"' && protect == '\0')
			{
				if (i == 0 || str[i - 1] != '\\')
					protect = '"';
				else
				{
					char *tmp;
					tmp = (char*) realloc(newStr, sizeof(char) * (BASE_SIZE + nbChar));

					if (tmp == NULL)
					{
						free(newStr);
						return NULL;
					}
					newStr = tmp;
					newStr[nbChar] = str[i];
					nbChar++;
				}
			}	
			else if (str[i] == '"' && protect == '"')
			{
				if (i == 0 || str[i - 1] != '\\')
					protect = '\0';
				else
				{
					char *tmp;
					tmp = (char*) realloc(newStr, sizeof(char) * (BASE_SIZE + nbChar));

					if (tmp == NULL)
					{
						free(newStr);
						return NULL;
					}
					newStr = tmp;
					newStr[nbChar] = str[i];
					nbChar++;
				}

			}
			else if (str[i] == '\'' && protect == '\0')
			{
				if (i == 0 || str[i - 1] != '\\')
					protect = '\'';
				else
				{
					char *tmp;
					tmp = (char*) realloc(newStr, sizeof(char) * (BASE_SIZE + nbChar));

					if (tmp == NULL)
					{
						free(newStr);
						return NULL;
					}
					newStr = tmp;
					newStr[nbChar] = str[i];
					nbChar++;
				}

			}
			else if (str[i] == '\'' && protect == '\'')
			{
				if (i == 0 || str[i - 1] != '\\')
					protect = '\0';
				else
				{
					char *tmp;
					tmp = (char*) realloc(newStr, sizeof(char) * (BASE_SIZE + nbChar));

					if (tmp == NULL)
					{
						free(newStr);
						return NULL;
					}
					newStr = tmp;
					newStr[nbChar] = str[i];
					nbChar++;
				}

			}
			else if (str[i] == '\\' && (! i || str[i - 1] != '\\'))
				continue;

			else
			{
				char *tmp;
				tmp = (char*) realloc(newStr, sizeof(char) * (BASE_SIZE + nbChar));

				if (tmp == NULL)
				{
					free(newStr);
					return NULL;
				}
				newStr = tmp;
				newStr[nbChar] = str[i];
				nbChar++;

			}
				
		}

		newStr[nbChar] = '\0';
		return newStr;
	}
%}
NORMAL    [^ \t\$\*\?\n\\\[\]\^\&\|;><]
SPECIAL    [ \t\$\*\?\n\\\[\]\^\&\|;\"']
VARIABLE    \$([a-zA-Z]|-|_)([0-9a-zA-Z]|-|_)*
SIMPLEQUOTED     '(\\'|[^'])*'
DOUBLEQUOTED    \"(\\\"|[^\"])*\"
STRING    ((\\{SPECIAL}|{NORMAL})|{SIMPLEQUOTED}|{DOUBLEQUOTED})+
SEQUENCE    (\|\||\&\&|;)
FLUX    (\||>|>>|<|2>|2>>|>\&|>>\&)
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
		char *nStr = NULL;

		for (;nStr == NULL;)
			nStr = traitementString(yytext);

		Commande *c = newCommande(nStr);
		yylval.command = newSequence(c);
		first = 0;
		return commande;
	}
	else
	{
		char *nStr = NULL;

		for (;nStr == NULL;)
			nStr = traitementString(yytext);

		yylval.string = nStr;
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
	else if (! strcmp(yytext, apperrout))
		yylval.flux = APPERROUT;
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
