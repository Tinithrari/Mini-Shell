#include <stdlib.h>

#include "struct/ArrayList.h"
#include "Commande.h"
#include "Job.h"

static unsigned long numJob = 1;
extern ArrayList *jobs;

void initJob(Job *j, pid_t pid, EtatProcessus etat, Commande *commande)
{
	if (jobs == NULL)
		jobs = createArray(sizeof(Job));
    if (j == NULL || pid <= 0 || commande == NULL)
        return;

    j->noJob = numJob++;
    j->pid = pid;
    j->etat = etat;
    j->commande = commande;
}
