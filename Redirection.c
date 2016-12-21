#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "Commande.h"
#include "Redirection.h"

#define IN_DEFAULT 0
#define OUT_DEFAULT 1
#define ERR_DEFAULT 2

#define ERR -1

tube* tubes = NULL;
static int nTubes = 0;

int setInCommande(Commande *c, string p)
{
    int nfd;

    // Checks the arguments
    if (c == NULL || p == NULL || c->in != IN_DEFAULT)
    {
        errno = EINVAL;
        return 0;
    }

    // Open the file in read mode
    nfd = open(p, O_RDONLY);

    // On error, return 0
    if (nfd == ERR)
        return 0;

    c->in = nfd;

    return 1;
}

// TODO : Append
int setOutCommande(Commande *c, string p, int append)
{
    int nfd;
    int flags = O_WRONLY|O_CREAT|(append ? O_APPEND : O_TRUNC);

    // Check the arguments
    if (c == NULL || p == NULL || c->out != OUT_DEFAULT)
    {
        errno = EINVAL;
        return 0;
    }

    // Open the file in write mode
    nfd = open(p, flags, (mode_t) 0664);

    // If an error occurs, return 0
    if (nfd == ERR)
        return 0;

    c->out = nfd;

    return 1;
}

// TODO : Append
int setErrCommande(Commande *c, string p, int append)
{
    int nfd;
    int flags = O_WRONLY|O_CREAT|(append ? O_APPEND : O_TRUNC);

    // Check the arguments
    if (c == NULL || p == NULL || c->err != ERR_DEFAULT)
    {
        errno = EINVAL;
        return 0;
    }

    // Try to open the file in write mode
    nfd = open(p, flags, (mode_t) 0664);

    // If an error occurs, return 0
    if (nfd == ERR)
        return 0;

    c->err = nfd;

    return 1;
}

static int addCloseFd(Commande *c, int fd)
{
    int *ptr;

    // Check the arguments
    if (c == NULL || fd < 0)
    {
        errno = EINVAL;
        return 0;
    }

    // Realloc the tab of file desxriptor to close
    ptr = (int *) realloc(c->toClose, sizeof(int) * (c->nFd + 1));

    if (ptr == NULL)
        return 0;

    // Change the pointer
    c->toClose = ptr;

    // Add the file descriptor in the tab and increment the number of fd
    c->toClose[c->nFd] = fd;
    (c->nFd)++;

    return 1;
}

int pipeCommande(Commande *c1, Commande *c2)
{
    tube *pTubes;

    // Check the arguments
    if (c1 == NULL || c2 == NULL)
    {
        errno = EINVAL;
        return 0;
    }

    // Increase the size of the tab of tube by 1
    pTubes = (tube*) realloc(tubes, sizeof(tube) * (nTubes + 1));

    if (pTubes == NULL)
        return 0;

    tubes = pTubes;

    // Allocate a new tab
    tubes[nTubes] = (int*) malloc(sizeof(int) * 2);

    // if allocation fail resize the tab by its size before the call of this function
    if (tubes[nTubes] == NULL)
    {
        if (! nTubes)
        {
            free(tubes);
        }
        else
        {
            tubes = realloc(tubes, sizeof(tube) * nTubes);
        }
        return 0;
    }

    // Create the tab, if fail, resize the tab by its size before the xall of this function
    if (pipe(tubes[nTubes]) == ERR)
    {
        if (! nTubes)
        {
            free(tubes);
        }
        else
        {
            tubes = realloc(tubes, sizeof(tube) * nTubes);
        }
        return 0;
    }

    // Add the opposite side of the pipe that uninterest us
    if (! addCloseFd(c1, tubes[nTubes][0]) || ! addCloseFd(c2, tubes[nTubes][1]))
    {
        close(tubes[nTubes][0]);
        close(tubes[nTubes][1]);
        if (! nTubes)
        {
            free(tubes);
        }
        else
        {
            tubes = realloc(tubes, sizeof(tube) * nTubes);
        }
        return 0;
    }

    // Prepare the pipe for the commands
    c1->out = tubes[nTubes][1];
    c2->in = tubes[nTubes][0];
    nTubes++;

    return 1;
}

void cleanCommandeEnv(void)
{
    int i;

    // Close each pipes stocked in the variables
    for (i = 0; i < nTubes; i++)
    {
        close(tubes[i][0]);
        close(tubes[i][1]);
    }

    // Free each pipes
    for (i = 0; i < nTubes; i++)
        free(tubes[i]);

    // Free the tab of pipe
    free(tubes);
    tubes = NULL;
    nTubes = 0;
}
