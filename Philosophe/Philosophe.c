#include <semaphore.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "Philosophe.h"

void initPhilosophe(Philosophe *p)
{
    // Le philosophe existe-t-il ?
    if (p == NULL)
        return;

    // Par défaut, le philosophe pense et n'a pas d'ustensile.
    p->statut = PENSE;
    sem_init( &(p->ustensiles), 0, 0);
}

void prendreUstensile(Philosophe *p)
{
    // Le philosophe existe-t-il ?
    if (p == NULL)
        return;

    // Prend ses ustensiles
    sem_wait(&(p->ustensiles));
}

void requisitionneUstensile(Philosophe *p)
{
    // Le philosophe existe-t-il ?
    if (p == NULL)
        return;

    sem_post(&(p->ustensiles));
    p->statut = MANGE;
}

static void attendre()
{
   // Pense entre 0 et 2 secondes
    int time = rand() % 2000;

    usleep(time);
}

void pense(int i)
{
    printf("Philosophe %d: Je pense\n", i);
    attendre();
}

void mange(int i)
{
    printf("Philosophe %d: Je mange\n", i);
    attendre();
}
