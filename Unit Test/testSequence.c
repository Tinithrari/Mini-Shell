#include "../Commande.h"
#include "../Sequence.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define command1 "/usr/bin/test"
#define command2 "mkdir"
#define command3 "rmdir"
#define arg1 "-d"
#define arg2 "test"


void test1()
{
    Sequence *s1, *s2;
    Commande *c1, *c2;
    DIR *dir;

    c1 = newCommande(command1);

    if (c1 == NULL)
    {
        fprintf(stderr, "Impossible d'effectuer les tests unitaires\n");
        exit(1);
    }

    if (! addOptionCommande(c1, arg1) || ! addOptionCommande(c1, arg2))
    {
        perror("Impossible d'effectuer les tests unitaires\n");
        exit(1);
    }

    c2 = newCommande(command2);

    if (c2 == NULL)
    {
        fprintf(stderr, "Impossible d'effectuer les tests unitaires\n");
        exit(1);
    }

    if (! addOptionCommande(c2, arg2))
    {
        fprintf(stderr, "Impossible d'effectuer les tests unitaires\n");
        exit(1);
    }

    s1 = newSequence(c1);

    if (s1 == NULL)
    {
        fprintf(stderr, "Impossible d'effectuer les tests unitaires\n");
        exit(1);
    }

    assert(s1->c == c1);
    assert(s1->l == NONE);

    s2 = newSequence(c2);

    if (s2 == NULL)
    {
        fprintf(stderr, "Impossible d'effectuer les tests unitaires\n");
        exit(1);
    }

    assert(linkSequence(s1, s2, OR));
    executeSequence(s1);
    deleteSequence(s1);

    dir = opendir(arg2);
    assert(dir != NULL);

    closedir(dir);
}

void test2()
{
    Sequence *s1, *s2;
    Commande *c1, *c2;
    DIR *dir;

    c1 = newCommande(command1);

    if (c1 == NULL)
    {
        fprintf(stderr, "Impossible d'effectuer les tests unitaires\n");
        exit(1);
    }

    if (! addOptionCommande(c1, arg1) || ! addOptionCommande(c1, arg2))
    {
        perror("Impossible d'effectuer les tests unitaires\n");
        exit(1);
    }

    c2 = newCommande(command3);

    if (c2 == NULL)
    {
        fprintf(stderr, "Impossible d'effectuer les tests unitaires\n");
        exit(1);
    }

    if (! addOptionCommande(c2, arg2))
    {
        fprintf(stderr, "Impossible d'effectuer les tests unitaires\n");
        exit(1);
    }

    s1 = newSequence(c1);
    s2 = newSequence(c2);

    assert(linkSequence(s1, s2, AND));
    executeSequence(s1);
    deleteSequence(s1);

    dir = opendir(arg2);
    assert(dir == NULL && errno == ENOENT);
    closedir(dir);
}

int main(void)
{
    test1();
    test2();
    printf("Test successfully passed!\n");
}
