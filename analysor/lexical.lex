/* --
lex lexical.lex
gcc lex.yy.c –lfl –o lexical
*/

delim			" "|\t
bl				{delim}+

chiffre			[0-9]
lettre			[a-zA-Z]

fonction		{lettre}+
option 			-(-)?{lettre}+
commande		{fonction}({bl}{option})*

caractAccepte	[^\\\:\/\*\?\"<>\|\n\t" "]
nomFichier		({caractAccepte}|{wildcard})+|(\"(.)+\")
formatFichier	\.({lettre}|{chiffre}|{wildcard})+
fichier			{nomFichier}{formatFichier}

background		\&
pipe			\|
wildcard		\*|\?|(\[(.)*\])
flux			(>)|(>>)|(2>)|(2>>)|(>&)|(>>&)|(<)

enchainement	(&&)|(\|\|)|(;)

courant			\.\/
pere			\.\.\/

variable		\$([a-zA-Z_]{1})(.)*


notreCommande	{commande}{bl}(({flux}{bl})?{fichier}{bl})*
enchCommande	{notreCommande}(({pipe}|{enchainement}){bl})?

%%

^{enchCommande}+{background}?{bl}*		{printf("Commande valide\n");}
\n
.							

%%

int main(void)
{
	yylex();
}
