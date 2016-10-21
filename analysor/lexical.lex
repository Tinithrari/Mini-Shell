sequence_char			(&&|\|\||;)
space				[ \t]
commande			[^&;\"\'\n]+
%%
{commande}{space}+{sequence_char}{space}+{commande}	{ printf("Expression séquentielle détectée: %s", yytext); }
{commande}						{ printf("nom de fichier ou de commande: %s", yytext); }
\n							ECHO;
.							{ printf("Caractère non reconnu: %s", yytext); }
%%
