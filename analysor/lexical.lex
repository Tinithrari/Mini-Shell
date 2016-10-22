
sequence_char	(&&|\|\||;)
carac	[^&;\|\"\']
%%
{sequence_char}			{ printf("Séquence détecté: %s\n", yytext); }
{carac}+			{ printf("Nom de fichier détecté: %s\n", yytext); }
%%
int main(void)
{
	for(;;)
		yylex();
	return 0;
}
