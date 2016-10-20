sequence_char		(&&|\|\||;)
space			[ \t]
spec			[&\t;\"\']
%%
[^{spec}]+		printf("nom de fichier ou de commande: %s\n", yytext);
.			printf("Caractère non reconnu: %s\n", yytext);
%%
int main(void)
{
	yylex();
}
