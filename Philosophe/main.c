#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#ifndef NB_PHILOSOPHES
    #error Le nombre de philosophes doit-etre spécifié
#endif // NB_PHILOSOPHES

#include "Philosophe.h"
#include "Repas.h"

pthread_t threads[NB_PHILOSOPHES];
Place places[NB_PHILOSOPHES];

int main()
{
    int i;
    Repas *r = newRepas();

    srand(time(NULL));

    if (r == NULL)
    {
        fprintf(stderr, "Erreur lors de l'exécution du programme des philosophes... sorties\n");
        exit(1);
    }

    for (i = 0; i < NB_PHILOSOPHES; i++)
    {
        places[i].r = r;
        places[i].no = i;
        pthread_create(threads + i, NULL, participer, places + i);
    }

    for (i = 0; i < NB_PHILOSOPHES; i++)
        pthread_join(threads[i], NULL);

    deleteRepas(r);

    exit(0);
}

