#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "struct/ArrayList.h"
#include "Commande.h"
#include "Redirection.h"
#include "Job.h"

#define IN_DEFAULT 0
#define OUT_DEFAULT 1
#define ERR_DEFAULT 2

#define ERROR -1

#define OPT_SIZE 2
#define OFFSET 2

int lastReturn = 0;
ArrayList *jobs = createArray(sizeof(Job));

Commande* newCommande(string s)
{
    Commande *c;

    // Si le paramètre est NULL, retourne NULL
    if (s == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    // Essaye d'allouer une commande
    c = (Commande*) malloc(sizeof(Commande));

    if (c == NULL)
        return NULL;

    // Mesure la taille de la chaine de caractère
    // Et alloue en conséquence
    c->commande = (string) malloc(sizeof(char) * (strlen(s) + 1));

    if (c->commande == NULL)
    {
        free(c);
        return NULL;
    }

    // Copie la chaine de caractère
    strcpy(c->commande, s);

    // Essaye d'allouer un premier élément pour les options
    c->options = (string*) malloc(sizeof(string) * OPT_SIZE);

    if (c->options == NULL)
    {
        free(c->commande);
        free(c);
        return NULL;
    }

    c->options[0] = (string) malloc( (strlen(s) + 1) * sizeof(char) );

    if (c->options[0] == NULL)
    {
        free(c->options);
        free(c->commande);
        free(c);
        return NULL;
    }

    // Remplit les champs de la structure avec les valeurs par défauts
    strcpy(c->options[0], s);
    c->options[1] = NULL;
    c->in = IN_DEFAULT;
    c->out = OUT_DEFAULT;
    c->err = ERR_DEFAULT;
    c->background = 0;
    c->toClose = NULL;
    c->nOptions = 1;
    c->nFd = 0;

    return c;
}

int addOptionCommande(Commande *c, string p)
{
    string* newOpt;

    // Vérifie les arguments
    if (c == NULL || p == NULL)
    {
        errno = EINVAL;
        return 0;
    }

    // Agrandit le tableau d'option
    newOpt = (string*) realloc(c->options, sizeof(string) * (c->nOptions + OFFSET));

    if (newOpt == NULL)
        return 0;

    // Place le nouveau pointeur dans la structure
    c->options = newOpt;

    // Essaye d'allouer la place nécessaire pour la chaine de caractère
    c->options[c->nOptions] = (string) malloc(sizeof(char) * (strlen(p) + 1));

    if (c->options[c->nOptions] == NULL)
    {
        c->options = realloc(c->options, sizeof(c->nOptions + 1));
        return 0;
    }

    // Effectue la copie et décale le pointeur NULL
    strcpy(c->options[c->nOptions], p);
    c->options[(c->nOptions) + 1] = NULL;

    c->nOptions++;

    return 1;
}

int executeCommande(Commande *c)
{
    pid_t pid;
    int status;

    // Vérifie si la commande n'est pas nulle
    if (c == NULL)
    {
        errno = EINVAL;
        return 0;
    }

    // Essaye de faire un fork, en cas d'erreur, retourne 0
    if ( (pid = fork()) == ERROR)
        return 0;

    // Code du fils
    if (! pid)
    {
        int i;

        // Redirige l'entrée si besoin
        if (c->in != IN_DEFAULT)
        {
            close(IN_DEFAULT);
            dup(c->in);
            close(c->in);
        }

        // Redirige la sortie si besoin
        if (c->out != OUT_DEFAULT)
        {
            close(OUT_DEFAULT);
            dup(c->out);
            close(c->out);
        }

        // Redirige la sortie d'erreur si besoin
        if (c->err != ERR_DEFAULT)
        {
            close(ERR_DEFAULT);
            dup(c->err);
            close(c->err);
        }

        for (i = 0; i < c->nFd; i++)
            close(c->toClose[i]);

        if (execvp(c->commande, c->options) == ERROR)
        {
            perror("Exec error");
            deleteCommande(c);
            cleanCommandeEnv();
            exit(1);
        }
    }

    // Code du père
    if (c->in != IN_DEFAULT)
        close(c->in);

    if (c->out != OUT_DEFAULT)
        close(c->out);

    if (c->err != ERR_DEFAULT)
        close(c->err);

    // Si le code est exécuté en premier plan
    if (! c->background)
    {
        wait(&status);

        lastReturn = (WIFEXITED(status) ? WEXITSTATUS(status) : ERROR);
    }
    else
    {
        Job j;
        // Enregistrement dans la table des jobs
        initJob(&j, pid, RUNNING, c);
        addInArray(jobs, j);
    }

    return 1;
}

void deleteCommande(Commande *c)
{
    int i;

    if (c == NULL)
        return;

    if (c->in != IN_DEFAULT)
        close(c->in);

    if (c->out != OUT_DEFAULT)
        close(c->out);

    if (c->err != ERR_DEFAULT)
        close(c->err);

    for (i = 0; i < c->nFd; i++)
        close(c->toClose[i]);

    for (i = 0; i < c->nOptions; i++)
        free(c->options[i]);

    free(c->options);
    free(c->commande);
    free(c->toClose);
    free(c);
}
