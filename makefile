mysh:
	yacc -d synthax.y
	lex lexical.lex
	make job
	make sequence
	gcc -c lex.yy.c
	gcc -c y.tab.c
	gcc -c VariableLocale.c
	gcc -g -c struct/LinkedList.c struct/Hashmap.c
	gcc -c cd.c
	gcc -c status.c
	gcc -c VariableEnvironnement.c
	gcc -o mysh Commande.o Sequence.o Redirection.o y.tab.o lex.yy.o Job.o ArrayList.o CoupleVariable.o VariableLocale.o LinkedList.o Hashmap.o cd.o JobCommand.o status.o VariableEnvironnement.o
clean-mysh:
	rm *.o
	rm mysh
myls-dbg:
	gcc -g -c Queue.c
	gcc -g -c myls.c
	gcc -g myls.o Queue.o -o myls

make myls:
	gcc -c Queue.c
	gcc -c myls.c
	gcc -o myls myls.o Queue.o

job:
	gcc -g -c Job.c JobCommand.c struct/ArrayList.c

commande:
	gcc -c CoupleVariable.c
	gcc -c Commande.c
	gcc -c Redirection.c

commande-dbg:
	gcc -g -c CoupleVariable.c
	gcc -g -c Commande.c
	gcc -g -c Redirection.c

commande-test:
	make commande-dbg
	gcc -g -c "Unit Test"/testCommande.c
	gcc -o TestCommande testCommande.o Commande.o Redirection.o

clean-test-commande:
	rm testCommande.o Commande.o Redirection.o
	rm TestCommande
	test -e Wrong.txt && rm Wrong.txt
	test -e test.txt && rm test.txt
sequence:
	make commande
	gcc -c Sequence.c

sequence-dbg:
	make commande-dbg
	gcc -g -c Sequence.c

sequence-test:
	make sequence-dbg
	make job
	gcc -g -c struct/ArrayList.c
	gcc -g -c "Unit Test"/testSequence.c
	gcc -g -o TestSequence testSequence.o Commande.o Redirection.o Sequence.o Job.o JobCommand.o ArrayList.o

clean-test-sequence:
	rm TestSequence testSequence.o Commande.o Redirection.o Sequence.o
