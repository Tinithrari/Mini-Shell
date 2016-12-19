#include "Commande.h"
#include "Redirection.h"
#include "Sequence.h"

#include <stdlib.h>
#include <errno.h>

#define ERR -1

extern int lastReturn;

Sequence *newSequence(Commande *c)
{
    Sequence *s;

    // Vérifie les arguments
    if (c == NULL || c->options == NULL || c->background)
    {
        errno = EINVAL;
        return NULL;
    }

    // Essaye d'allouer la structure
    s = (Sequence*) malloc(sizeof(Sequence));

    if (s == NULL)
        return NULL;

    // Remplit les champs de la structure
    s->c = c;
    s->l = NONE;
    s->suivant = NULL;

    return s;
}

int linkSequence(Sequence *s1, Sequence *s2, Logique op)
{
    Sequence *s;

    // Vérifie les arguments
    if (s1 == NULL || s2 == NULL || s2->c == NULL || s2->l != NONE)
        return 0;

    // Va à la fin de la séquence s1
    for (s = s1; s->suivant != NULL; s = s->suivant);

    // Ajoute s2 à s1 et ajoute l'opérateur
    s->suivant = s2;
    s2->l = op;

    return 1;
}

int executeSequence(Sequence *s)
{
    Sequence *ptr;
    int retour = 0;
    // Vérifie les arguments
    if (s == NULL || s->l != NONE || s->c == NULL)
        return -1;

    // Fait le tour de la séquence
    for (ptr = s; ptr != NULL; ptr = ptr->suivant)
    {
        // Si aucune condition, ou que la commande précédente s'est bien passé et la condition est un ET
        // ou la commande précédente s'est mal passé et la condition est un OU, éxécuter la commande
        if (ptr->l == NONE || (! retour && ptr->l == AND) || (retour > 0 && ptr->l == OR))
        {
            executeCommande(ptr->c);
        }
        else
            return 0;

        // Récupère le retour précédent
        retour = lastReturn;

        // Retourne 0 en cas de signal
        if (retour == ERR)
            return 0;
    }

    return 1;
}

static void deleteLinkedMember(Sequence *s)
{
    if (s == NULL)
        return;

    deleteLinkedMember(s->suivant);
    deleteCommande(s->c);
    free(s);
}

void deleteSequence(Sequence *s)
{
    if (s == NULL)
        return;

    deleteLinkedMember(s);
    cleanCommandeEnv();
}
