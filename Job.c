#include <stdlib.h>

#include "Job.h"

static unsigned long numJob = 1;

void initJob(Job *j, pid_t pid, EtatProcessuss etat, Commande *commande)
{
    if (j == NULL || pid <= 0 || commande == NULL)
        return;

    j->noJob = numJob++;
    j->pid = pid;
    j->etat = etat;
    j->commande = commande;
}
