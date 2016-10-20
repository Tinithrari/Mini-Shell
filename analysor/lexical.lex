sequence_char	(&&|\|\||;)
space		[ \t]
spec		[&\t;\"\' \t\n]
%%
{sequence_char}\n		printf("Délimiteur de sequence détecté: %s\n", yytext);
{spec}\n			printf("Caractère speciaux: %s\n", yytext);
.				printf("rien n'a été reconnu\n");
%%
int main(void)
{
	yylex();
}
