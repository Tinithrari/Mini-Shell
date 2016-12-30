#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "VariableEnvironnement.h"

#define TAILLE 1024
#define ERR -1 

//Determine si il y a des caracteres autre que '\0' sur un intervalle donné
int strlen_interv(const char *s, int debut, int fin)
{
    int i;
    
    for (i = debut; i <= fin; i++)
         if (s[i] != '\0')
                 return 0;	      
    return 1;
}

//Modifie la valeur associee a une variable
int modifier(char * destination, char * nomVariable, char * valeurVariable)
{
	char buffer[128];
	int debut, i, j, k, tailleChaine;

	if((nomVariable == NULL) || (valeurVariable == NULL))
		return ERR;
		
	if((strlen(nomVariable) + strlen(valeurVariable) + 1) >= 128)
		return ERR;
	
	strcpy(buffer,nomVariable);
	strcat(buffer, "=");
	
	
	//Si le debut vaut ERR, on cree le nom de variable avec la valeur associée
	if((debut = indice_debut(destination,buffer)) == ERR)
	{
		strcat(buffer,valeurVariable);
		inserer(destination,buffer);
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
			int echoue = inserer(destination,buffer);
		
			if(!echoue)
				for(j = 0; *(destination + j); j++)
					*(destination + j) = '\0';
				
			return echoue;
		}
		
		for(i = debut; *(destination + i)!= '=';i++);
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
int inserer(char * destination, const char * chaine)
{
	int tailleDest = 1, i, j ,k;

	if(chaine == NULL)
		return 1;
	
	if(TAILLE < strlen(chaine))
		return 1;
	
	for(tailleDest = 1; tailleDest <= strlen(chaine); tailleDest = tailleDest << 1);
	
	printf("taille dest : %d , strlen chaine : %d\n", tailleDest, strlen(chaine));
	
	printf("#%d#%d#\n",TAILLE - tailleDest ,TAILLE - tailleDest - 1);
	
	for(i = 0; i < (TAILLE - tailleDest - 1) ; i+=tailleDest)
	{
		printf("<<%d>>\n",strlen_interv(destination,i,(i + tailleDest - 1)));
		
		if((strlen_interv(destination,i,(i + tailleDest + 1)) == 0) ||  ((i>0) && destination[i-1] != '\0'))
			continue;
			
		
		else
		{
			for(j = i, k = 0; (chaine[k] != '\0'); j++, k++)
				destination[j] = chaine[k];
			printf("$%d$\n",j);
			destination[j] = '\0';
			return 0;
		}
	}
	
	printf("&& %d && %d &&", (TAILLE - tailleDest - 1), i);
	
	printf("%s : Ajout impossible\n", chaine);
	return 1;
}

//Teste si chaine est dans destination ou non
int estPresente (char * destination, const char * chaine)
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
int indice_debut (const char * destination, char * chaine)
{
	int i, j, k, tmp;
	
	if(chaine == NULL)
		return ERR;

	if(estPresente(destination, chaine) == 0)
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
int supp_chaine (char * destination, const char * chaine)
{
	int i, j, debut;

	if(chaine == NULL)
		return 0;

	if(estPresente(destination, chaine) == 0)
		return 0;
		
	else
	{
		for (i = 0; i < TAILLE; )
		{
    		if (strstr(destination + i, chaine) == NULL)
       			i += strlen(destination + i) + 1;
    		
    		else
        	{
        		debut = indice_debut(destination,chaine);
        		if(debut == ERR)
        			return 0;
        			
        		else
        		{
        			for(j = debut; j <= (debut + strlen(chaine)); j++)
        				*(destination + j) = '\0';
        			
        			return 1;
        		}	
        	}
		}
	return 0;
	}
}

int main(void)
{	
	int shmid, i;
	char * shared_char;
	char tab[5][70] = {"Je m'appelle jacky=el matador","petit lapin rose","je suis amoureux","il y a des jours ou franchement je ferais mieux de rester coucher","MDR"}; 
	
	shmid = shmget(IPC_PRIVATE, sizeof(char) * TAILLE, 0600);
	shared_char = (char *)shmat(shmid,0,0);
	
	if(fork() == 0)
	{
		for(i = 0; i < 5; i++)
		{
			printf("Ajout %d : %d\n\n",i,inserer(shared_char,tab[i]));
		}
		
		printf("%d : <%s>\n",i, shared_char);
		printf("%d : <%s>\n",i, shared_char+32);
		printf("%d : <%s>\n",i, shared_char+64);
		printf("%d : <%s>\n",i, shared_char+96);
		printf("%d : <%s>\n",i, shared_char+128);
		printf("%d : <%s>\n",i, shared_char+52);
		
		printf("<%d>\n",modifier(shared_char,"Je m'appelle jacky","uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu"));
		printf("%d : <%s>\n",i, shared_char);
		
		
		printf("%d : <%s>\n",i, shared_char);
		printf("%d : <%s>\n",i, shared_char+32);
		printf("%d : <%s>\n",i, shared_char+64);
		printf("%d : <%s>\n",i, shared_char+96);
		printf("%d : <%s>\n",i, shared_char+128);
		printf("%d : <%s>\n",i, shared_char+52);
		printf("%d : <%s>\n",i, shared_char+256);
		shmdt((char *)shared_char);
	
	}
	
	else
	{
		wait(NULL);
		shmdt((char *)shared_char);
		shmctl(shmid,IPC_RMID,0);
	}
	
	
	return 0;
}
