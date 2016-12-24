philosophe = 5
mange = 3

all: philosophe

clear:
	rm *.o
	rm Philosophe

philosophe:
	gcc -c Philosophe.c
	gcc -DNB_PHILOSOPHES=$(philosophe) -DNB_MANGE=$(mange) -c Repas.c
	gcc -DNB_PHILOSOPHES=$(philosophe) -c main.c
	gcc -o Philosophe main.o Philosophe.o Repas.o -lpthread
