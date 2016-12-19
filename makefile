interpretor:
	gcc -c argTable.c
	gcc -c shell.c
	gcc argTable.o shell.o -o shell

interpretor-dbg:
	gcc -g -c argTable.c
	gcc -g -c shell.c
	gcc -g argTable.o shell.o -o shell

myls-dbg:
	gcc -g -c Queue.c
	gcc -g -c myls.c
	gcc -g myls.o Queue.o -o myls

make myls:
	gcc -c Queue.c
	gcc -c myls.c
	gcc -o myls myls.o Queue.o

commande:
	gcc -c Commande.c
	gcc -c Redirection.c

commande-dbg:
	gcc -g -c Commande.c
	gcc -g -c Redirection.c

commande-test:
	make commande-dbg
	gcc -g -c "Unit Test"/testCommande.c
	gcc -o TestCommande testCommande.o Commande.o Redirection.o

clean-test:
	rm testCommande.o Commande.o Redirection.o
	rm TestCommande
	test -e Wrong.txt && rm Wrong.txt
	test -e test.txt && rm test.txt
