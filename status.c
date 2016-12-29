#include <stdio.h>
#include <unistd.h>

#include "status.h"

#define ERR -1

extern int lastReturn;
extern pid_t lastPid;

void status(void)
{
	if(lastReturn == ERR)
		printf("%d terminé anormalement\n",lastPid);
		
	else
		printf("%d terminé avec comme code de retour %d\n",lastPid, lastReturn);
}
