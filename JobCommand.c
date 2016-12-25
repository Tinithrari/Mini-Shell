#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "Commande.h"
#include "struct/ArrayList.h"
#include "Redirection.h"
#include "Job.h"
#include "JobCommand.h"

extern ArrayList *jobs;
extern Job *running;

static void cleanShell(void)
{
    int i;

    // Supprime les commandes
    for (i = 0; i < jobs->nbElement; i++)
	    deleteCommande(((Job*)jobs->array)[i].commande);

    // Supprime le tableau de Job
    deleteArray(jobs);

    // Libère l'environnement des commandes
    cleanCommandeEnv();
}

void exitShell(void)
{
    cleanShell();
    exit(0);
}

void interruption(int sig)
{
    int i;

    // Tue les processus
    for (i = 0; i < jobs->nbElement; i++)
        kill(((Job*)jobs->array)[i].pid, SIGKILL);

    cleanShell();
    exit(0);
}

void stopJob(int sig)
{
    if (running != NULL)
    {
        kill(running->pid, SIGTSTP);
        running->etat = STOPPED;
        addInArray(jobs, running);
	deleteCommande(running->commande);
	free(running);
    }
}

void myjobs(void)
{
    int i;

    for (i = 0; i < jobs->nbElement; i++)
        printf("[%d] %d %s %s\n", ((Job*)jobs->array)[i].noJob, ((Job*)jobs->array)[i].pid, ((Job*)jobs->array)[i].etat == RUNNING ? "En cours d'exécution" : "Stoppé", ((Job*)jobs->array)[i].commande->commande);
}
