#include "CoupleVariable.h"

#include <string.h>
#include <errno.h>

int cutVariable(char* str, CoupleVariable* c)
{
    char *finNom, *name, *value, nameLength, valueLength;

    // Vérifie les arguments
    if (str == NULL || c == NULL)
    {
        errno = EINVAL;
        return 0;
    }

    // Vérifie si un égal est présent
    if ( (finNom = strstr(str, "=")) == NULL)
    {
        errno = EINVAL;
        return 0;
    }
    // Calcule la logueur du nom et alloue l'espace nécessaire pour la chaine
    nameLength = (finNom - str);
    name = (char*) malloc(sizeof(char) * nameLength);

    if (name == NULL)
        return 0;

    // Copie le nom dans la chaine
    strncpy(name, str, (nameLength - 1));
    name[nameLength - 1] = '\0';

    finNom++;

    valueLength = strlen(finNom) + 1;

    value = (char*) malloc(sizeof(char) * valueLength);

    if (value == NULL)
    {
        free(name);
        errno = ENOMEM;
        return 0;
    }

    strcpy(value, finNom);

    c->name = name;
    c->value = value;

    return 1;
}
