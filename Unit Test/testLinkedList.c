#include <stdlib.h>
#include <assert.h>

#include "../struct/LinkedList.h"

int elt[10] = {0,1,2,3,4,5,6,7,8,9};

int main(void)
{
    int i;
    LinkedList *ll = newLL(sizeof(int));

    for (i = 0; i < 10; i++)
    {
        addEltLL(ll, elt + i);
    }

    for (i = 0; i < 10; i++)
        assert( *((int*)getEltLL(ll, i)) == elt[i]);

    free(removeEltLL(ll, 0));

    assert( *((int*)getEltLL(ll, 0)) == 1);

    free(removeEltLL(ll, 6));

    assert( *((int*)getEltLL(ll, 6)) == 8);

    deleteLL(ll);
}
