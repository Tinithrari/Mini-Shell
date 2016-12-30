#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "VariableEnvironnement.h"

#define TAILLE 1024 
#define ERR -1
#define FILE "/bin/cat"
#define KEY 4661

#define SEP '\a'

int shmcreer;

static int shmid;
static char * destination;

//Affiche l'ensemble des variables d'environnements
void affiche_variables_environnement(void)
{
	int i, longueur;

	if(destination == NULL)
		return;
	
	for(i = 0; i < TAILLE;)
	{	
		if(destination[i] != '\0')
		{
			char buffer[TAILLE];
			
			sprintf(buffer,"%s",destination[i]);
			longueur = strlen(buffer) + 1;
			
			printf("%s\n",buffer);
			
			i += longueur;
			continue;
		}
		
		i++;
	}
}

//Affiche la valeur associee a une chaine
char * getValeur(char * chaine)
{
	int debut, longueur;
	
	if(chaine == NULL)
		return "";
		
	longueur = strlen(chaine) + 1;

	{
		char buffer[longueur + 1];
	
		strcpy(buffer,chaine);
		strcat(buffer,"\a");
	
		debut = indice_debut(chaine);

		if(debut == ERR)
			return "";
			
		else
		{
			int i;
			
			for(i = debut; destination[i] != SEP; i++);
			
			return destination + (i + 1);
		}
	}
}

//Initialise la memoire partagee
void init_shm(void)
{
	key_t k;

	k = ftok(FILE, KEY);
	shmid = shmget(k, sizeof(char) * TAILLE, 0600 | IPC_CREAT | IPC_EXCL); 
	shmcreer = (shmid != ERR);
	
	if(shmid == ERR)
	{
		printf("Coucou\n");
		shmid = shmget(k, sizeof(char) * TAILLE, 0);
	}
	
	destination = (char *)shmat(shmid,0,0);
	
	if((void *)destination == (void *)ERR)
		perror("Shmat Error"), exit(1);	
}

//Detache un segment de la memoire partagee
void detache_memoire(void)
{
	shmdt(destination);
}

//Supprime la memoire partagee
void delete_shm(void)
{
	shmdt(destination);
	if(shmcreer)
		shmctl(shmid,IPC_RMID,0);
	
}

//Determine si il y a des caracteres autre que '\0' sur un intervalle donné
int strlen_interv(int debut, int fin)
{
    int i;
    
    for (i = debut; i <= fin; i++)
         if (destination[i] != '\0')
                 return 0;	      
    return 1;
}

//Modifie la valeur associee a une variable
int modifier(char * nomVariable, char * valeurVariable)
{
	char buffer[128];
	int debut, i, j, k, tailleChaine;

	if((nomVariable == NULL) || (valeurVariable == NULL))
		return ERR;
		
	if((strlen(nomVariable) + strlen(valeurVariable) + 1) >= 128)
		return ERR;
	
	strcpy(buffer,nomVariable);
	strcat(buffer, "\a");
	
	
	//Si le debut vaut ERR, on cree le nom de variable avec la valeur associée
	if((debut = indice_debut(buffer)) == ERR)
	{
		strcat(buffer,valeurVariable);
		inserer(buffer);
		return 1;
	}
	
	else
	{
		int tailleMeme,tailleVarMem;
		
		strcat(buffer,valeurVariable);
		tailleChaine = strlen(buffer);
		tailleMeme = strlen(destination + debut);
		
		if(tailleChaine > tailleMeme)
		{
			int echoue = inserer(buffer);
		
			if(!echoue)
				for(j = 0; *(destination + j); j++)
					*(destination + j) = '\0';
				
			return echoue;
		}
		
		for(i = debut; *(destination + i)!= SEP;i++);
		i++;
		
		tailleVarMem = strlen(destination + i);
		
		for(k = 0; *(valeurVariable + k) != '\0'; k++)
		{
			*(destination + i + k) = *(valeurVariable + k);
			
		}
		
		*(destination + i + k) = '\0';
		
		for(;k < tailleVarMem; k++)
			*(destination + i + k) = '\0';
		
		return 0;	 
	}
}

//Inserer une chaine de caracteres dans une autre chaine de caracteres sans ecraser de contenu
int inserer(char * chaine)
{
	int tailleDest = 1, i, j ,k, b;

	if(chaine == NULL)
		return 1;
	
	if(TAILLE < strlen(chaine))
		return 1;
	
	for(tailleDest = 1; tailleDest <= strlen(chaine); tailleDest = tailleDest << 1);
	
	for(i = 0; i < (TAILLE - tailleDest - 1) ; i+=tailleDest)
	{		
		if((strlen_interv(i,(i + tailleDest + 1)) == 0) ||  ((i>0) && destination[i-1] != '\0'))
			continue;
			
		else
		{
			b = 0;
		
			for(j = i, k = 0; (chaine[k] != '\0'); j++, k++)
			{	
				if(!b && (chaine[k] == '='))
				{
					destination[j] = SEP;
					b = 1;
				}
				
				else
					destination[j] = chaine[k];
			}
			destination[j] = '\0';
			return 0;
		}
	}
	return 1;
}

//Teste si chaine est dans destination ou non
int estPresente ( char * chaine)
{
	int i;

	if(chaine == NULL)
		return 0;

	for (i = 0; i < TAILLE; )
	{
    	if (strstr(destination + i, chaine) == NULL)
       		i += strlen(destination + i) + 1;
    	else
        	return 1;
	}
	return 0;
}

//Retourne l'indice ou commence la chaine
int indice_debut (char * chaine)
{
	int i, j, k, tmp;
	
	if(chaine == NULL)
		return ERR;

	if(estPresente(chaine) == 0)
		return ERR;

	for (i = 0; i < TAILLE; )
	{
    	if (strstr(destination + i, chaine) == NULL)
       		i += strlen(destination + i) + 1;
    	else
        	return i;      
	}
	return ERR;
}

//Supprime la chaine de destination
int supp_chaine (char * chaine)
{
	int i, j, debut, longueur;
	
	

	if(chaine == NULL)
		return 0;
	
	longueur = strlen(chaine) + 1;
	
	{
		char buffer[longueur + 1];
		int index, i;
		
		strcpy(buffer,chaine);
		strcat(buffer,"\a");
		
		index = indice_debut(buffer);
	

		if(index == ERR)
			return 0;
		
		for(i = index; destination[i]; i++)
			destination[i] = '\0';
		
		return 1;
	}
}
