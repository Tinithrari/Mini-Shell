#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "LinkedList.h"

LinkedList *newLL(size_t eltSize)
{
    LinkedList *ll;

    // Check argument
    if (! eltSize)
    {
        errno = EINVAL;
        return NULL;
    }

    ll = (LinkedList*) malloc(sizeof(LinkedList));

    if (ll == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }

    ll->eltSize = eltSize;
    ll->head = NULL;
    ll->tail = NULL;
    ll->nElement = 0;
}

int addEltLL(LinkedList *ll, void *elt)
{
    struct nodeLL *node;

    if (ll == NULL || elt == NULL)
        return 0;

    node = (struct nodeLL*) malloc(sizeof(struct nodeLL));

    if (node == NULL)
        return 0;

    node->value = malloc(sizeof(ll->eltSize));

    if (node->value == NULL)
    {
        free(node);
        errno = ENOMEM;
        return 0;
    }

    memcpy(node->value, elt, ll->eltSize);
    node->next = NULL;

    if (! ll->nElement)
    {
        ll->head = node;
        ll->tail = node;
    }
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
    struct nodeLL *ptr;
    int i;

    if (ll == NULL || index >= ll->nElement)
    {
        errno = EINVAL;
        return NULL;
    }

    for (i = 0, ptr = ll->head; i < index && ptr != NULL; ptr = ptr->next, i++);

    return ptr == NULL ? NULL : ptr->value;
}

void* removeEltLL(LinkedList *ll, unsigned int index)
{
    struct nodeLL *ptr;
    void *value;

    if (ll == NULL || index >= ll->nElement)
    {
        errno = EINVAL;
        return NULL;
    }

    value = malloc(ll->eltSize);

    if (value == NULL)
        return NULL;

    ptr = ll->head;
    if (! index)
    {

        ll->head = ll->head->next;
        memcpy(value, ptr->value, ll->eltSize);
        free(ptr->value);
        free(ptr);
        ll->nElement--;
        return value;
    }
    else
    {
        struct nodeLL *elt;
        int i;

        for (i = 0; i < index - 1; ptr = ptr->next, i++);

        elt = ptr->next;
        ptr->next = elt->next;

        memcpy(value, elt->value, ll->eltSize);

        free(elt->value);
        free(elt);
        ll->nElement--;
        return value;
    }
}

void cleanLL(LinkedList *ll)
{
    if (ll == NULL)
        return;

    for (;ll->nElement;)
        free(removeEltLL(ll, 0));
}

void deleteLL(LinkedList *ll)
{
    if (ll == NULL)
        return;

    cleanLL(ll);
    free(ll);
}
