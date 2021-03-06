#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "../Commande.h"
#include "../Redirection.h"

#define command1 "echo"
#define arg "Hello World!"

#define command2 "cat"
#define OUT "test.txt"

#define command3 "ls"
#define arg2 "Wrong.txt"
#define arg1 "/moquerie"

#define ERROR -1

extern tube *tubes;

static void test1(void)
{
    Commande *c;

    c = newCommande(command1);

    if (c == NULL)
    {
        perror("Impossible d'effectuer les tests unitaires");
        exit(1);
    }

    /*
        Test de la création d'une commande
     */
    assert(c->commande != NULL);
    assert(! strcmp(c->commande, command1));
    assert(c->options != NULL);
    assert(! strcmp(c->commande, c->options[0]));
    assert(c->options[1] == NULL);
    assert(! c->in);
    assert(c->out == 1);
    assert(c->err == 2);
    assert(! c->background);
    assert(! c->nFd);
    assert(c->nOptions == 1);
    assert(c->toClose == NULL);

    if (! addOptionCommande(c, arg))
    {
        perror("Impossible d'exécuter les tests unitaires");
        exit(1);
    }

    /*
        Test de l'ajout d'une option
    */
    assert(c->nOptions == 2);
    assert(! strcmp(c->options[1], arg));
    assert(c->options[2] == NULL);

    assert(executeCommande(c));

    deleteCommande(c);
}

static void test2(void)
{
    Commande *c1, *c2;

    if ( (c1 = newCommande(command1)) == NULL || (c2 = newCommande(command2)) == NULL )
    {
        perror("Impossible d'effectuer les tests unitaires");
        exit(1);
    }

    if (! addOptionCommande(c1, arg))
    {
        perror("Impossible d'effectuer les tests unitaires");
        exit(1);
    }

    if (! pipeCommande(c1, c2))
    {
        perror("Impossible d'effectuer les tests unitaires");
        exit(1);
    }

    assert(c1->out != 1);
    assert(c2->in != 0);
    assert(tubes != NULL);
    assert(tubes[0] != NULL);

    assert(setOutCommande(c2, OUT, 1));

    assert(executeCommande(c1) && executeCommande(c2));
    deleteCommande(c1);
    deleteCommande(c2);
    cleanCommandeEnv();

    assert(access(OUT, F_OK) != ERROR);
    assert(tubes == NULL);
}

static void test3(void)
{
    Commande *c;

    if ( (c = newCommande(command3)) == NULL )
    {
        perror("Impossible d'effectuer les tests unitaires");
        exit(1);
    }

    setErrCommande(c, arg2, 0);
    assert(c->err != 2);

    if (! addOptionCommande(c, arg1))
    {
        perror("Impossible d'effectuer les tests unitaires");
        exit(1);
    }

    assert(executeCommande(c));
    deleteCommande(c);

    assert(access(arg2, F_OK) != ERROR);
}


int main(void)
{
    test1();
    test2();
    test3();

    printf("Test passed successfully\n");
}
