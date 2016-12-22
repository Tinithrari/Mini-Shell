#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"

void initLL(LinkedList *ll, size_t eltSize)
{
    // Vérifie les arguments
    if (ll == NULL || ! eltSize)
        return;

    // Remplit la structure
    ll->eltSize = eltSize;
    ll->head = NULL;
    ll->tail = NULL;
}

LinkedList *newLL(size_t eltSize)
{
    LinkedList *ll;

    if (! eltSize)
    {
        errno = EINVAL;
        return NULL;
    }

    // Essaye d'allouer la liste chainée
    ll = (LinkedList*) malloc(sizeof(LinkedList));

    if (ll == NULL)
        return NULL;

    // Initialise la liste chainée
    initLL(ll);

    return ll;
}

int addEltLL(LinkedList *ll, void *elt)
{
    struct nodeLL *node;

    // Vérifie les arguments
    if (ll == NULL || elt == NULL)
    {
        errno = EINVAL;
        return 0;
    }

    // Essaye d'allouer un nœud
    node = (struct nodeLL*) malloc(sizeof(struct nodeLL));

    if (node == NULL)
        return 0;

    // Essaye d'allouer un élément pour le nœud
    node->value = malloc(ll->eltSize);

    if (node->value == NULL)
    {
        free(node->value);
        errno = ENOMEM;
        return 0;
    }

    // Copie la valeur dans le nœud
    memcpy(node->value, elt, ll->eltSize);

    // Si aucun élément dans la liste
    if (tail == NULL)
        ll->head = ll->tail = node;
    else
    {
        ll->tail->next = node;
        ll->tail = ll->tail->next;
    }

    ll->nElement++;

    return 1;
}

void* getEltLL(LinkedList *ll, unsigned int index)
{
    int i;
    struct nodeLL *ptr;

    // Vérifie les arguments
    if (ll == NULL || index >= ll->nElement)
    {
        errno = EINVAL;
        return NULL;
    }

    // Va au ième élément
    for (i = 0, ptr = ll->head; i < index - 1; i++, ptr = ptr->next);

    // retourne la valeur de l'élément
    return ptr->value;
}

void *removeEltLL(LinkedList *ll, unsigned int index)
{
    int i;
    void *value;
    struct nodeLL *ptr;

    // Vérifie les arguments
    if (ll == NULL || index >= ll->nElement)
    {
        errno = EINVAL;
        return NULL;
    }

    // Va au ième élément
    for (i = 0, ptr = ll->head; i < index - 1; i++, ptr = ptr->next);

    // Retire le nœud de la liste, récupère sa valeur, et le libère
    if (! i)
    {
        ll->head = ll->head->next;
        value = ptr->value;
        free(ptr);
    }
    else
    {
        struct nodeLL *node = ptr->next;
        ptr->next = node->next;
        value = node->value;
        free(node);
    }

    ll->nElement--;

    return value;
}

void cleanLL(LinkedList *ll)
{
    if (ll == NULL)
        return;

    for (;ll->nElement;)
        removeEltLL(0);
}

void deleteLL(LinkedList *ll)
{
    if (ll == NULL)
        return;

    cleanLL(ll);
    free(ll);
}
