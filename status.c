#include <stdio.h>
#include "status.h"

#define ERR -1

extern int lastReturn;
extern pid_t lastPid;

void status()
{
	if(lastReturn == ERR)
		printf("%d terminé anormalement",lastPid);
		
	else
		printf("%d terminé avec comme code de retour %d",lastPid, lastReturn);
}
