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


