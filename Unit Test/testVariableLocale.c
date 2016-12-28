#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "../VariableLocale.h"

char *var1 = "toto";
char *var2 = "tutu";

char *val1 = "tata";
char *val2 = "titi";

int main(void)
{
	char *res1, *res2;
	assert(getValeurVariableLocale(&var1) == NULL);

	assert(setVariableLocale(&var1, &val1));
	assert(setVariableLocale(&var2, &val2));

	res1 = getValeurVariableLocale(&var1);
	res2 = getValeurVariableLocale(&var2);

	printf("%s\n", res1);
	
	assert(! strcmp(res1, val1));
	assert(! strcmp(res2, val2));

	printf("Test passed\n");

	cleanVariable();

	return 0;
}
