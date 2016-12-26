#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Affichage
void affichage(int nbTrou,int tabTrou[10])
{
	int i = 0;
	
	printf("Trou:");
	
	for(i = 0; i < nbTrou; i++)
		printf("%d\n",tabTrou[i]);
}

//First Fit
void firstfit(int nbProc, int nbTrou, int tabTrou[10], int tabProc[10])
{
	int i, j, k, flag = 0, tmp = nbTrou;
   
	for(i = 0; i < nbProc; i++)
	{
		flag=0;
		for(j = 0; j < nbTrou; j++)
		{
			if(tabProc[i] <= tabTrou[j])
			{
				tabTrou[j] -= tabProc[i];
				printf("\nMemoire alloué par le processus %d :(%d)", i, tabProc[i]);
				flag = 1;
				break;             
			}
		}
		
		if(flag == 0)
		{
			printf("\nTrou non disponible.....\n");
			break;
		}  
	}
	
	for(i = 0; i < tmp; i++)
		printf("%d : %d\n", i ,tabTrou[i]);
}

int main(void)
{
	int nbProc, nbTrou=0, i, tabProc[10], tabTrou[10], tmp[10];
		
	printf("Entrer un nombre de processus :\n");
	scanf("%d",&nbProc);
	
	printf("Entrer le nombre de trous :\n");
	scanf("%d",&nbTrou);
	
	for(i = 0; i < nbProc; i++)
	{
		printf("Processus %d :",i+1);  
		scanf("%d",&tabProc[i]);  
	}
	
	for(i = 0; i < nbTrou; i++)
	{
		printf("Trou %d:",i+1);  
		scanf("%d",&tabTrou[i]);  
		tmp[i] = tabTrou[i];
	}

	firstfit(nbProc, nbTrou, tmp, tabProc);

	return 0;    
}
