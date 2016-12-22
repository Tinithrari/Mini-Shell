#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "ArrayList.h"

#define BASE_SIZE 10

ArrayList* createArray(const size_t eltSize)
{
    ArrayList *a;

    // Check if the size is strictly positive
    if (! eltSize)
    {
        errno = EINVAL;
        return NULL;
    }

    // Try to allocate the structure
    a = (ArrayList*) malloc(sizeof(ArrayList));

    if (a == NULL)
        return NULL;

    // Try to allocate the array
    a->array = malloc(eltSize * BASE_SIZE);

    if (a->array == NULL)
    {
        free(a);
        errno = ENOMEM;
        return NULL;
    }

    // Fill the other field of the structure
    a->eltSize = eltSize;
    a->arrSize = BASE_SIZE;
    a->nbElement = 0;

    return a;
}

int addInArray(ArrayList *a, const void *elt)
{
    char* ptr;

    // Check parameter
    if ((a == NULL) || (elt == NULL))
    {
        errno = EINVAL;
        return 0;
    }

    // Check if the array is not full
    if (a->nbElement == a->arrSize)
    {
        // Extend the array
        void *new = realloc(a->array, a->eltSize * (a->arrSize << 1));

        if (new == NULL)
            return 0;
        
        a->array = new;
        a->arrSize <<= 1; 
    }

    // Get a pointer on the array to do pointer's arithmetic
    ptr = (char*) a->array;

    // Copy the content of void *elt into the array and increment the number of element
    memcpy(ptr + ((a->nbElement) * (a->eltSize)), elt, a->eltSize);
    (a->nbElement)++;

    return 1;
}

int removeFromArray(ArrayList *a, void *elt, eqFunc fct)
{
    unsigned int i, f = 0;
    char* ptr;
    // Check the argument
    if (a == NULL || elt == NULL || fct == NULL || ! a->nbElement)
        return 0;

    // Check each element and remove it
    for (i = 0, ptr = a->array; i < a->nbElement; i++, ptr += a->eltSize)
        if (fct(ptr, elt))
            f++;


    return f;
}

int removeFromArrayAt(ArrayList *a, const unsigned int index)
{
    char *ptr, *elt;
    unsigned int i;

    // Check the argument
    if ( a == NULL || index >= a->nbElement)
    {
        errno = EINVAL;
        return 0;
    }

    // Get the element
    ptr = a->array;
    // Reorder the elements
    for (i = index; i < (a->nbElement - 1); i++)
        memcpy((ptr + (i * a->eltSize)), (ptr + ((i+1) * a->eltSize)), a->eltSize);

    // If the number of elements is greater than BASE_SIZE and inferior to the half of the array size, reduce the array
    if ( (--(a->nbElement)) <= (a->arrSize >> 1) && (a->nbElement > BASE_SIZE))
    {
	void *ret;
        a->arrSize >>= 1;
        ret = realloc(a->array, a->arrSize * a->eltSize);
	if (ret == NULL)
		errno = ENOMEM;
    }

    return 1;
}

void deleteArray(ArrayList *a)
{
    // Check the arguments
    if (a == NULL)
        return;
    
    free(a->array);
    free(a);
}
