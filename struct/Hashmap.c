#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "LinkedList.h"
#include "Hashmap.h"

Hashmap* newHashmap(size_t keySize, size_t eltSize, size_t mapSize, hachage hash, compare cmp)
{
    Hashmap *h;
    int i, j;

    // Vérifie les arguments
    if (! keySize || ! eltSize || ! mapSize || hash == NULL || cmp == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    // Essaye d'allouer la structure
    h = (Hashmap*) malloc(sizeof(Hashmap));

    if (h == NULL)
        return NULL;

    // Essaye d'allouer le tableau de liste chainée
    h->map = (LinkedList**) malloc(sizeof(LinkedList*) * mapSize);

    if (h->map == NULL)
    {
        free(h);
        errno = ENOMEM;
        return NULL;
    }

    // Initialise les liste chainées
    for (i = 0; i < mapSize; i++)
    {
        h->map[i] = newLL(sizeof(Pair));

        // Si l'allocation échoue
        if (h->map[i] == NULL)
        {
            for (j = i - 1; j >= 0; j--)
                deleteLL(h->map[i]);

            free(h->map);
            free(h);
            errno = ENOMEM;
            return NULL;
        }
    }

    // Remplit le reste de la structure
    h->keySize = keySize;
    h->eltSize = eltSize;
    h->mapSize = mapSize;
    h->hash = hash;
    h->cmp = cmp;

    return h;
}

static int allocatePair(Pair *p, size_t keySize, size_t eltSize)
{
    // Essaye d'allouer l'espace nécessaire pour stocker la clé
    p->key = malloc(keySize);

    if (p->key == NULL)
        return 0;

    // Essaye d'allouer l'espace nécessaire pour stocker l'élément
    p->elt = malloc(eltSize);

    if (p->elt == NULL)
    {
        free(p->key);
        return 0;
    }
    return 1;
}

int addEltHashmap(Hashmap* h, void* key, void* elt)
{
    int noListe;
    Pair p;

    // Vérifie les arguments
    if (h == NULL || key == NULL || elt == NULL)
    {
        errno = EINVAL;
        return 0;
    }

    if (! allocatePair(&p, h->keySize, h->eltSize))
        return 0;

    // Copie les valeurs dans la structure
    memcpy(p.key, key, h->keySize);
    memcpy(p.elt, elt, h->eltSize);

    // Calcul la liste nécessaire
    noListe = h->hash(key) % h->mapSize;

    // Retourne l'ajout de l'élément dans la liste calculée
    return addEltLL(h->map[noListe], &p);
}

int updateEltHashmap(Hashmap* h, void* key, void* newValue)
{
    Pair* p;
    int i, noListe;

    // Vérifie les arguments
    if (h == NULL || key == NULL || newValue == NULL)
        return 0;

    noListe = h->hash(key) % h->mapSize;

    // Recherche la clé
    for (i = 0; i < h->map[noListe]->nElement; i++)
    {
        p = (Pair*) getEltLL(h->map[noListe], i);

        // Si la clé a été trouvée
        if (! h->cmp(key, p->key))
        {
            Pair newPair;

            // Si la paire n'a pas été allouée
            if (! allocatePair(&newPair, h->keySize, h->eltSize))
                return 0;

            // Copie les valeurs dans la paire
            memcpy(newPair.key, key, h->keySize);
            memcpy(newPair.elt, newValue, h->eltSize);

            // Si on ne réussi pas par la remplacer, on libère la nouvelle association
            if (! addEltLL(h->map[noListe], &newPair))
            {
                free(newPair.elt);
                free(newPair.key);
                errno = ENOMEM;
                return 0;
            }

            // On retire l'élément
            p = removeEltLL(h->map[noListe], i);

            // Libère la paire
            free(p->key);
            free(p->elt);
            free(p);

            return 1;
        }

    }

    // Si la clé n'est pas présente, on ajoute l'élément
    return addEltHashmap(h, key, newValue);
}

void* getEltHashmap(Hashmap* h, void* key)
{
    int i, noListe;

    // Vérifie les arguments
    if (h == NULL || key == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    // Calcul le hashcode
    noListe = h->hash(key) % h->mapSize;

    // On recherche la clé
    for (i = 0; i < h->map[noListe]->nElement; i++)
    {
        Pair *p = (Pair*) getEltLL(h->map[noListe], i);

        // Si on trouve la clé, retourne l'élément
        if (! h->cmp(key, p->key) )
            return p->elt;
    }

    return NULL;

}

void* removeEltHashmap(Hashmap* h, void* key)
{
    int i, noListe;

    // Vérifie les arguments
    if (h == NULL || key == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    // Calcul le hashcode
    noListe = h->hash(key) % h->mapSize;

    // On recherche la clé
    for (i = 0; i < h->map[noListe]->nElement; i++)
    {
        Pair *p = (Pair*) getEltLL(h->map[noListe], i);

        // Si on trouve la clé, supprime la paire et retourne l'élément
        if (! h->cmp(key, p->key) )
        {
            void* elt = p->elt;
            free(p->key);
            free(p);
            return elt;
        }
    }

    return NULL;
}

void deleteHashmap(Hashmap* h)
{
    int i;

    // Vérifie les arguments
    if (h == NULL)
        return;

    // Libère toutes les listes chainées et leurs composant
    for (i = 0; i < h->mapSize; i++)
    {
        for (;h->map[i]->nElement;)
        {
            Pair *p = (Pair*) removeEltLL(h->map[i], 0);
            free(p->elt);
            free(p->key);
            free(p);
        }
        deleteLL(h->map[i]);
    }

    // Libère le reste de la structure
    free(h->map);
    free(h);
}
