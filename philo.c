#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#include <sys/sem.h>

#define ERR -1
#define GAUCHE ((i+n-1)%n)
#define DROITE ((i+1)%n)
#define NB_FOIS_MANGE 3

#define syserror(m,e) perror(m), exit(e)
#define fatalerror(m,e) fprintf(stderr,m), exit(e)
#define couleurdefaut printf("\033[0m")

//Décrementation semaphore
struct sembuf P={0,-1,0};

//Incrementation semaphore
struct sembuf V={0,1,0};

//Nombre de philosophes
static int n;

//Semaphore mutex
static int mutex;

//Tableau de semaphores
static int s;

//Les differents etats possible
typedef enum
{
	PENSE = 0,
	FAIM = 1,
	MANGE = 2
} Etat;

//Liste des etats de nos philosophes
static Etat *etatPhilo;

//Determine la couleur de l'affichage
void couleurphilo(int i)
{
	switch(i%8)
	{
		case 0:
			printf("\033[32m");
			break;
			
		case 1:
			printf("\033[01;36m");
			break;
			
		case 2:
			printf("\033[01;32m");
			break;
		
		case 3:
			printf("\033[01;34m");
			break;
		
		case 4:
			printf("\033[01;31m");
			break;
		
		case 5:
			printf("\033[01;35m");
			break;
		
		case 6:
			printf("\033[40;33m");
			break;
		
		case 7:
			printf("\033[40;37m");
			break;	
	}
}

void penser(int i)
{
	sleep(1 + (unsigned int)(20.*rand()/(RAND_MAX+1.0)));
}

void manger(int i)
{
	sleep(1 + (unsigned int)(10.*rand()/(RAND_MAX+1.0)));
}

//Verification des voisins si ils mangent
void test(int i)
{
	struct sembuf VsurIemeEltTab = {i,1,0};

	if (etatPhilo == NULL)
		return;
		
	if((etatPhilo[i] == FAIM) && (etatPhilo[GAUCHE] != MANGE) && (etatPhilo[DROITE] != MANGE))
	{
		etatPhilo[i] = MANGE;
		
		//On fait V(s[i])
		if(semop(s,&VsurIemeEltTab,1) == ERR)
			syserror("V(s[i])",1);
	}
}

//Le philosophe i prend ces couverts si il le peut
void prendre_fourchettes(int i)
{	
	struct sembuf PsurIemeEltTab = {i,-1,0};

	//P(mutex)
	if(semop(mutex,&P,1) == ERR)
		syserror("P(mutex)",1);
		
	couleurphilo(i);
	etatPhilo[i] = FAIM;
	test(i);
	
	//V(mutex)
	if(semop(mutex,&V,1) == ERR)
		syserror("V(mutex)",1);
		
	//P(s[i])
	if(semop(s,&PsurIemeEltTab,1) == ERR)
		syserror("P(s[i])",1);
}

//Le philosophes posent ces couverts
void poser_fourchettes(int i)
{
	//P(mutex)
	if(semop(mutex,&P,1) == ERR)
		syserror("P(mutex)",1);
		
	etatPhilo[i] = PENSE;
	test(GAUCHE);
	test(DROITE);
	
	//V(mutex)
	if(semop(mutex,&V,1) == ERR)
		syserror("V(mutex)",1);	
}

void philosophe(int i)
{
	char j;
	
	couleurphilo(i);
	printf("Philosophe %4d : SE MET A TABLE ET PENSE\n", i);
	couleurdefaut;

	srand((unsigned int)getpid());
	
	//On recupere le semaphore mutex
	if((mutex = semget(ftok("/bin/cat",1),1,0666)) == ERR)
		syserror("Creation mutex",1);
	
	//On recupere le tableau de semaphores s	
	if((s = semget(ftok("/bin/cat",2),2,0666)) == ERR)
		syserror("Creation tableau semaphore",1);
		
	for(j = 0; j < NB_FOIS_MANGE; j++)
	{
		penser(i);
		prendre_fourchettes(i);
		couleurphilo(i);
		printf("Philosophe %4d : MANGE\n", i);
		couleurdefaut;
		manger(i);
		couleurphilo(i);
		printf("Philosophe %4d : PENSE\n", i);
		couleurdefaut;
		poser_fourchettes(i);
	}
	couleurphilo(i);
	printf("Philosophe %4d : QUITTE LA TABLE\n", i);
	couleurdefaut;
	exit(0);
}

int main (int argc, char *argv[])
{
	int i;
	pthread_t th1, th2;
	
	//On ignore le SIGINT
	if(signal(SIGINT,SIG_IGN) == SIG_ERR)
		syserror("Impossible de rediriger Ctl-C",1);
	
	//On teste le nombre de parametres
	if(argc != 2)
		fatalerror("Syntaxe : philo #philosophes\n",1);
	
	//On transforme la chaine en entier	
	sscanf(argv[1],"%d", &n);
	
	etatPhilo = (Etat*)malloc(sizeof(Etat)*n);
	
	//Initialisation du tableau des etats des philosophes
	for (i = 0; i < n ; i++)
		etatPhilo[i] = PENSE;
	
	//Creation du semaphore mutex
	if((mutex = semget(ftok("/bin/cat",1),1,0666|IPC_CREAT)) == ERR)
		syserror("Creation mutex",1);
	
	//Affectation du semaphore mutex
	if(semctl(mutex,0,SETVAL,1) == ERR)
		syserror("Affectation valeur mutex",1);
	
	//Creation du tableau de semaphores
	if((s = semget(ftok("/bin/cat",2),n,0666|IPC_CREAT)) == ERR)
		syserror("Creation tableau semaphore",1);

	//Affectation du tableau de semaphores
	for(i = 0; i < n ; i++)
		if(semctl(s,i,SETVAL,0) == ERR)
			syserror("Affectation valeur",1);
	
	//Destruction du tableau de semaphores
	if(semctl(s,0,IPC_RMID,0) == ERR)
		syserror("Destruction de s",1);
	
	//Destruction du mutex
	if(semctl(mutex,0,IPC_RMID,0) == ERR)
		syserror("Destruction de s",1);
	
	free(etatPhilo);	
	return 0;
}
