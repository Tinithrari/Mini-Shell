#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "Commande.h"
#include "struct/ArrayList.h"
#include "Redirection.h"
#include "Job.h"
#include "JobCommand.h"
#include "VariableLocale.h"

#define ERR -1

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

    cleanVariable();

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

    if (running != NULL)
    {
	    kill(running->pid, SIGINT);
	    running = NULL;
	    return;
    }
    
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
	running = NULL;
    }
}

void childDead(int sig)
{
	int i;

	for (i = 0; i < jobs->nbElement; i++)
	{
		Job j = ( (Job*)jobs->array )[i];
		int ret = kill( j.pid, 0);

		if (ret == ERR && errno == ESRCH)
		{
			int status, code;
			waitpid(j.pid, &status, 0);

			code = WIFSIGNALED(status) ? -1 : WEXITSTATUS(status);
			
			printf("%s (jobs=[%d], pid=%d) terminée avec status=%d\n", j.commande->commande, j.noJob, j.pid, code);
			deleteCommande(j.commande);
			removeFromArrayAt(jobs, i);
		}
	}

	if (! jobs->nbElement)
		resetCounter();
}

void myjobs(void)
{
    int i;

    for (i = 0; i < jobs->nbElement; i++)
        printf("[%d] %d %s %s\n", ((Job*)jobs->array)[i].noJob, ((Job*)jobs->array)[i].pid, ((Job*)jobs->array)[i].etat == RUNNING ? "En cours d'exécution" : "Stoppé", ((Job*)jobs->array)[i].commande->commande);
}
