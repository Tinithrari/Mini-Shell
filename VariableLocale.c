#include <stdlib.h>
#include <stdio.h>

#include "VariableLocale.h"
#include "struct/LinkedList.h"
#include "struct/Hashmap.h"

#include <string.h>
#include <assert.h>

#define HASH_POW 31
#define HASHMAP_SIZE 1024

typedef char* string;

static int hashString(void *str)
{
    int i, len, hashValue = 0;
    string s = *((string*)str);

    len = strlen(s);

    for (i = 0; i < len; i++)
        hashValue = (hashValue * HASH_POW) + s[i];

    return hashValue;
}

static int stringCompare(void *s1, void *s2)
{
	string str1 = *((string*)s1), str2 = *((string*)s2);

	return strcmp(str1, str2);
}

static Hashmap *variablesLocales = NULL;

static void initHashmap()
{
    variablesLocales = newHashmap(sizeof(char*), sizeof(char*),
                                  HASHMAP_SIZE, hashString, stringCompare);
}

int setVariableLocale(char **name, char **value)
{
    if (variablesLocales == NULL)
        initHashmap();

    return updateEltHashmap(variablesLocales, name, value);
}

char* getValeurVariableLocale(char **name)
{
	char **res;
	if (variablesLocales == NULL)
		initHashmap();
	res = (char**)getEltHashmap(variablesLocales, name);
	
	return res == NULL ? "" : *res;
}

void removeVariableLocale(char **name)
{
	string* str;

	if (variablesLocales == NULL)
		initHashmap();
	
	if ( name == NULL )
		return;

	str = removeEltHashmap(variablesLocales, name);

	if (str != NULL)
		free(*str);
}

void cleanVariable(void)
{
    deleteHashmap(variablesLocales);
}

void afficheEnsembleVariableLocale(void)
{
	int i;

	if (variablesLocales == NULL)
		initHashmap();
	
	for (i = 0; i < variablesLocales->mapSize; i++)
	{
		int j;

		for (j = 0; j < variablesLocales->map[i]->nElement; j++)
		{
			Pair *p = getEltLL(variablesLocales->map[i], j);
			printf("<%s> => <%s>\n", *((char**)p->key), *((char**)p->elt));
		}
	}
}
