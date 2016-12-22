#include <errno.h>

#include "LinkedList.h"
#include "Hashmap.h"

Hashmap *newHashmap(size_t keySize, size_t eltSize, size_t mapSize, hachage hash, compare comp)
{
    Hashmap *h;
    int i;

    // Vérifie les arguments
    if (! keySize || ! eltSize || ! mapSize || hash == NULL || comp == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    // Essaye d'allouer la structure de la table de hachage
    h = (Hashmap*) malloc(sizeof(Hashmap));

    if (h == NULL)
        return NULL;

    // Essaye d'allouer le tableau de liste chainée
    h->map = (LinkedList*) malloc(sizeof(LinkedList) * mapSize);

    if (h->map == NULL)
    {
        free(h);
        errno = ENOMEM;
        return NULL;
    }

    // Initialise les listes chainées
    for (i = 0; i < mapSize; i++)
        initLL((h->map + i), sizeof(Pair));

    // Remplit les structures
    h->keySize = keySize;
    h->eltSize = eltSize;
    h->mapSize = mapSize;
    h->hash = hash;
    h->comp = comp;
}

int addEltHashmap(Hashmap* h, void *key, void *elt)
{
    int index;
    Pair p;

    // Vérifie les arguments
    if (h == NULL || key == NULL || elt == NULL)
    {
        errno = EINVAL;
        return 0;
    }

    index = h->hash(key) % h->mapSize;
    p.key = key;
    p.elt = elt;

    if (! addEltLL(h->map + index, p))
        return 0;
    return 1;
}

int updateEltHashmap(Hashmap *h, void *key, void *newValue)
{
    int i, index, trouve = 0;

    // Vérifie les arguments
    if (h == NULL || key == NULL || newValue == NULL)
        return NULL;

    index = h->hash(key) % h->mapSize;

    for (i = 0; i < h->map[index].nElement; i++)
    {
        if (! h->cmp(key, getEltLL(h->map + index, i)))
        {
            getEltLL(h->map + index, i).elt = newValue;
            trouve = 1;
        }
    }

    if (! trouve)
        return addEltHashmap(h, key, newValue);

    return 1;
}

void* getEltHashmap(Hashmap *h, void *key)
{
    int i, index;

    // Vérifie les arguments
    if (h == NULL || key == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    index = h->hash(key) % h->mapSize;

    for (i = 0; i < h->map[index].nElement; i++)
    {
        if (! h->cmp(key, getEltLL(h->map + index, i).elt))
            return getEltLL(h->map + index, i).elt;
    }

    return NULL;
}

void* removeEltHashmap(Hashmap *h, void *key)
{
    int i, index;

     // Vérifie les arguments
    if (h == NULL || key == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    index = h->hash(key) % h->mapSize;

    for (i = 0; i < h->map[index].nElement; i++)
    {
        if (! h->cmp(key, getEltLL(h->map + index, i).elt))
        {
            Pair p = getEltLL(h->map + index, i);

            removeEltLL(h->map + index, i)

            return p.elt;
        }
    }

    return NULL;
}

void deleteHashmap(Hashmap *h)
{
    int i;

    if (h == NULL)
        return;

    for (i = 0; i < h->mapSize; i++)
        deleteLL(h + i);

    free(h);
}
