#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include "Redirection.h"
#include "Job.h"
#include "JobCommand.h"
#include "struct/ArrayList.h"

extern ArrayList *jobs;
extern Job *running;

static void cleanShell(void)
{
    int i;

    // Supprime les commandes
    for (i = 0; i < jobs->nbElement; i++)
        deleteCommande(jobs->array[i].commande);

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
        kill(jobs->array[i].pid, SIGKILL);

    cleanShell();
    exit(0);
}

void stopJob(int sig)
{
    if (running != NULL)
    {
        kill(running.pid, SIGTSTP);
        running.etat = STOPPED;
        addInArray(jobs, j);
    }
}

void myjobs(void)
{
    int i;

    for (i = 0; i < jobs->nbElement; i++)
        printf("[%d] %d %s %s\n", jobs->array[i].noJob, jobs->array[i].pid, jobs->array[i].etat == RUNNING ? "En cours d'exécution" : "Stoppé", jobs->array[i].commande->commande);
}
