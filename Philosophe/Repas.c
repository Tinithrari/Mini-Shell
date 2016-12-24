#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#ifndef NB_PHILOSOPHES
    #error "Le paramètre de compilation NB_PHILOSOPHES doit être spécifié"
#endif // NB_PHILOSOPHES

#ifndef NB_MANGE
    #error "Le paramètre de compilation NB_MANGE doit être spécifié"
#endif // NB_MANGE

#include "Philosophe.h"
#include "Repas.h"

#define LEFT(i, n) (((i) + (n) - 1) % (n))
#define RIGHT(i, n) (((i) + 1) % (n))

Repas* newRepas()
{
    int i;
    Repas *r = (Repas*) malloc(sizeof(Repas));

    // L'allocation a-t-elle réussie ?
    if (r == NULL)
        return NULL;

    // Initialise les philosophes
    for (i = 0; i < NB_PHILOSOPHES; i++)
        initPhilosophe(r->table + i);

    // Initialise le mutex
    pthread_mutex_init(&(r->verrou), NULL);

    return r;
}

void verifierCouvert(Place *p)
{
    // La place existe-t-elle ?
    if (p == NULL)
        return;

    // On verrouille l'accès à la table
    pthread_mutex_lock(&(p->r->verrou));

    // Si aucun des voisins ne mangent, le philosophe réquisitionne ses ustensiles
    if (p->r->table[LEFT(p->no, NB_PHILOSOPHES)].statut != MANGE
        && p->r->table[RIGHT(p->no, NB_PHILOSOPHES)].statut != MANGE)
        requisitionneUstensile(p->r->table + p->no);
    else
        p->r->table[p->no].statut = FAIM; // Sinon, il a faim

    // On déverouille le mutex
    pthread_mutex_unlock(&(p->r->verrou));
}

void poserCouvert(Place *p)
{
    // La place existe-t-elle ?
    if (p == NULL)
        return;

    // Verrouille l'accès à la table
    pthread_mutex_lock(&(p->r->verrou));

    // Si le voisin de gauche a faim et que son voisin de gauche ne mange pas, on lui donne ses ustensiles
    if (p->r->table[LEFT(p->no, NB_PHILOSOPHES)].statut == FAIM
        && p->r->table[LEFT(LEFT(p->no, NB_PHILOSOPHES), NB_PHILOSOPHES)].statut != MANGE)
        requisitionneUstensile(p->r->table + LEFT(p->no, NB_PHILOSOPHES));

    // Si le voisin de droite a faim et que son voisin de droite ne mange pas, on lui donne ses ustensiles
    if (p->r->table[RIGHT(p->no, NB_PHILOSOPHES)].statut == FAIM
        && p->r->table[RIGHT(RIGHT(p->no, NB_PHILOSOPHES), NB_PHILOSOPHES)].statut != MANGE)
        requisitionneUstensile(p->r->table + RIGHT(p->no, NB_PHILOSOPHES));

    p->r->table[p->no].statut = PENSE;

    // On dévérouille l'accès à la table
    pthread_mutex_unlock(&(p->r->verrou));
}

void* participer(void *place)
{
    int i;
    Place *p = (Place*) place;

    for (i = 0; i < NB_MANGE; i++)
    {
        pense(p->no);
        verifierCouvert(p);
        prendreUstensile(p->r->table + p->no);
        mange(p->no);
        poserCouvert(p);
    }

    pthread_exit(0);
}

void deleteRepas(Repas *r)
{
    int i;

    if (r == NULL)
        return;

    for (i = 0; i < NB_PHILOSOPHES; i++)
        sem_destroy(&(r->table[i].ustensiles));

    pthread_mutex_destroy(&(r->verrou));

    free(r);
}
