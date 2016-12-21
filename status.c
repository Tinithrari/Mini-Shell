#include <stdio.h>
#include "status.h"

extern int lastReturn;
extern pid_t lastPid;

void status()
{
	if(lastReturn == -1)
		printf("%d terminé anormalement",lastPid);
		
	else
		printf("%d terminé avec comme code de retour %d",lastPid, lastReturn);
}
