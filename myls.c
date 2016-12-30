#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <time.h>

#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/wait.h>

#include "Queue.h"
#include "myls.h"

#define syserror(m,e) perror(m), exit(e)
#define fatalerror(m,e) fprintf(stderr, "%s\n", m), exit(e)
#define couleur(p) printf("\033[1;%sm",p)
#define couleurdefaut() printf("\033[0m")

#define ERR -1
#define FLAG_A 0
#define FLAG_R 1
#define CURRENT_DIR "."

static char flag[2] = {0,0};

//Fonction de verification de la saisie afin d'intialiser les flags aux valeurs correspondantes
void initflags(int nbparam, char *lesparams[])
{
	int i;

	for(i = 1; (i < nbparam); i++)
	{
		//Si les flags deja a 1, on sort
		if((flag[FLAG_A] == 1) && (flag[FLAG_R] == 1))
			return;
		
		else
		{
			//On regarde si le premier caractere est bien un trait d'union
			if(*(lesparams[i]) == '-')
			{
				//Dans le cas ou -aR ou -Ra ont ete saisi en ligne de commande
				if( !(strcmp("-aR",lesparams[i])) || !(strcmp("-Ra",lesparams[i])))
				{
					flag[FLAG_A] = 1;
					flag[FLAG_R] = 1;
				}
			
				else
				{
					//Dans le cas ou -a a ete saisi en ligne de commande 
					if(!(strcmp("-a",lesparams[i])))
						flag[FLAG_A] = 1;
				
					else
					{
						//Dans le cas ou -R a ete saisi en ligne de commande
						if(!(strcmp("-R",lesparams[i])))
							flag[FLAG_R] = 1;
							
						else
							fatalerror("Option invalide",1);						
					}
				}
			}
		}
	}
}

//Fonction qui va stocker les fichiers en parametre sous forme de chaine de caracteres dans la Queue
Queue* stockageFichiers(int nbElt, char *nosFichiers[])
{	int i;
	Queue *q = createQueue();

	if(nosFichiers == NULL)
		enqueue(q, CURRENT_DIR, strlen(CURRENT_DIR) + 1);
		
	else
	{
		for(i = 1; i < nbElt; i++)
			enqueue(q, nosFichiers[i], strlen(nosFichiers[i]) + 1);
	}
	
	return q;
}

//Ajoute tout les fichiers de argv sauf les options
Queue* deleteOption (int nbElt, char *tab[])
{
	int i;
	Queue *q = createQueue();
	
	if(tab == NULL)
		return NULL;
		
	for(i = 1; i < nbElt; i++)
	{
		if((strcmp(tab[i],"-a") != 0) && (strcmp(tab[i],"-R") != 0) && (strcmp(tab[i],"-aR") != 0) && (strcmp(tab[i],"-Ra") != 0))
			enqueue(q, tab[i], strlen(tab[i]) + 1);
		
	}
	return q;
} 

//Verification des parametres (Gere que les repertoires pour le moment)
int verif_fichiers(int nbParam, char * chaine[])
{
	int i, boolean = 1;
	DIR *d;
	struct stat st;
	
	if(chaine == NULL)
		return 1;
	
	for(i = 1; i < nbParam; i++)
	{
		if(*chaine[i] == '-')
			continue;
		
		if((d = opendir(chaine[i])) == NULL)
		{	
			if(stat(chaine[i],&st) == -1)
			{	
				printf("ls: impossible d'accéder à '%s': Aucun fichier ou dossier de ce type\n",chaine[i]);
			}	
		}
		closedir(d);
	}	
	
	return 0;
}

//Affichage des droits
void affiche_droits2(struct stat st, char buffer[])
{	
	int i = 0;
	char chaine[512];

	//Affichage des differents droits associe au fichier 
	sprintf(chaine + i++,"-");
	sprintf(chaine + i++,"%c",(st.st_mode & S_IRUSR) ? 'r' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IWUSR) ? 'w' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IXUSR) ? 'x' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IRGRP) ? 'r' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IWGRP) ? 'w' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IXGRP) ? 'x' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IROTH) ? 'r' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IWOTH) ? 'w' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IXOTH) ? 'x' : '-' );
	
	printf("%s ",chaine);
}

//Affichage du nom du fichier
void affiche_nom2(struct stat st, char buffer[])
{
	char bufMime[128], type[128];
	int tube[2];
	pid_t pid;
	int status, i;
	char c;
	
	if(pipe(tube) == ERR)
		syserror("Pipe problem",4);

	if((pid = fork()) == ERR)
		syserror("Fork problem", 5);

			
	if(! pid)
	{
		close(tube[0]);
		__fpurge(stdout); // Purge stdout
		close(1);
		dup(tube[1]);
		close(tube[1]);
		execlp("/usr/bin/mimetype","mimetype","-b", buffer,NULL);
		syserror("Execl error", 6);
	}
	
	else
	{
		close(tube[1]);
		wait(&status);

		for(i = 0; (read(tube[0],&c,sizeof(char))) && c != '\n'; i++)
			bufMime[i] = c;
			
		bufMime[i] = '\0';

		strncpy(type,bufMime, strchr(bufMime, '/') - bufMime);				
		type[strchr(bufMime, '/') - bufMime]  ='\0';
		close(tube[0]);
	}
			
	if(((strcmp(type,"audio") == 0) || (strcmp(type,"image") == 0)))
	{
		couleur("35");
	}
	
	else if(strcmp(type,"application") == 0)
		{	
			if( strstr(strchr(bufMime, '/'), "executable")  )
			{
				couleur("32");
			}
			
			else if((strstr(strchr(bufMime, '/'), "compressed")) || (strstr(strchr(bufMime, '/'), "archive")) || (strstr(strchr(bufMime, '/'), "zip")))
				{
					couleur("31");
				}
		}
		
		else
		{
			//Affichage du nom du fichier
			switch (st.st_mode & S_IFMT) 
			{
				//Couleur bleu pour les repertoires
				case S_IFDIR: 
					couleur("34");
					break;

				//Couleur cyan pour les liens symboliques
				case S_IFLNK:
					couleur("36");
					break;

				//Couleur jaune pour les fichiers FIFO ou blocks
				case S_IFIFO:
				case S_IFBLK:
					couleur("33");
					break;	
				
				//Couleur magenta pour les fichiers audios, images ou sockets
				case S_IFSOCK:
					couleur("35");
					break;
			}
		}
		
	//Couleur par defaut pour les fichiers ordinaires
	printf("%s\n", buffer);
	couleurdefaut();
}

//Affichage des donnees
void affiche_donnees2(struct stat st, char buffer[])
{
	char chaine[1024];
	struct passwd *pwd, *pwd2;	
	struct tm *mtime;

	if((pwd = getpwuid (st.st_uid)) == NULL)
		syserror("getpwuid error",3);

	if((pwd2 = getpwuid (st.st_gid)) == NULL)
		syserror("Getpwuid error",3);

	//Affichage des droits
	affiche_droits2(st,buffer);
	
	//Affichage du nombre de liens physique, du nom de l'utilisateur, du groupe, et la taille
	sprintf(chaine,"%u %s %s\t%lld\t",st.st_nlink,pwd -> pw_name, pwd2 -> pw_name,(long long) st.st_size);
	printf("%s",chaine);

	//Affichage de la date de la derniere modification du ficher
	mtime = localtime(&(st.st_mtim.tv_sec));
	
	//Affichage de la date
	affiche_date(mtime);
	
	affiche_nom2(st,buffer);
}

//Parcours le contenu de de la queue
void parcours_queue(Queue *q, int nb)
{
	QueueElt *tmp;
	struct stat st;
	char *s;
	DIR *d;

	if(q == NULL)
		return;
	
	while(! isQueueEmpty(q))
	{
		tmp = dequeue(q);
		s = (char *) tmp -> elt;

		d = opendir(s);
			
		//On ouvre le repertoire, erreur si soucis
		if(d == NULL)
		{
			if(stat(s,&st) != ERR)
			{	
				affiche_donnees2(st,s);
				
				if(!isQueueEmpty(q))
					printf("\n");
			}
		}
				
		else
		{	
			printf("%s:\n", s);
			parcours_rep(q,d,s);
		}
		closedir(d);	
	}	
}

//Parcours le contenu du repertoire
void parcours_rep(Queue *q, DIR *d, char * chaine)
{
	struct dirent *elt;
	struct stat st;
	char buffer[512];
	
	if((d == NULL) || (chaine == NULL))
		return;
	
	//On boucle tant qu'il a des elements a lire dans le repertoire
	while(elt = readdir(d))
	{
		memset(buffer,0,sizeof(buffer));
	
		//Dans le cadre du -a on passe au tour de boucle suivant si le flag est a 1 ou si le '.' est le premier caractere
		if ((*(elt->d_name) == '.') && !(flag[FLAG_A]))
			continue;

		//Afin de definir l'arboresence 
		strcat(buffer,chaine);
		strcat(buffer,"/");
		strcat(buffer,elt->d_name);
	
		//On initialise la structure stat, erreur si echec
		if(stat(buffer,&st) == ERR)
			syserror("Stat error",2);
			
		affiche_donnees(q, st, buffer, elt);	
	}
	
	if(!isQueueEmpty(q))
		printf("\n");
}

//Affichage des droits
void affiche_droits(Queue *q, struct stat st, struct dirent *elt, char buffer[])
{	
	int i = 0;
	char chaine[512];

	//On regarde si il s'agit d'un repertoire ou non
    switch (st.st_mode & S_IFMT)
	{
    	case S_IFDIR:     
			sprintf(chaine + i++,"d");
			
			//Dans le cadre du -R on ajoute le repertoire dans la file si le flag est a 1
			if((flag[FLAG_R] == 1) && (strcmp(elt -> d_name,".") != 0) && (strcmp(elt -> d_name,"..") != 0))
			{
				enqueue(q, buffer, strlen(buffer) + 1);
			}
			
			break;
			
		default:       
			sprintf(chaine + i++,"-");               
			break;
	}
	
	//Affichage des differents droits associe au fichier 
	sprintf(chaine + i++,"%c",(st.st_mode & S_IRUSR) ? 'r' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IWUSR) ? 'w' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IXUSR) ? 'x' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IRGRP) ? 'r' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IWGRP) ? 'w' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IXGRP) ? 'x' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IROTH) ? 'r' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IWOTH) ? 'w' : '-' );
	sprintf(chaine + i++,"%c",(st.st_mode & S_IXOTH) ? 'x' : '-' );
	
	printf("%s ",chaine);
}

//Affichage de la date
void affiche_date(struct tm *mtime)
{
	char chaine[512], mois[7], heure[5], minute[5];
	
	switch(mtime -> tm_mon)
	{
		case 0:
			sprintf(mois,"jan.\t");
			break;

		case 1:
			sprintf(mois,"feb.\t");
			break;
					
		case 2:
			sprintf(mois,"mar.\t");
			break;
					
		case 3:
			sprintf(mois,"apr.\t");
			break;
					
		case 4:
			sprintf(mois,"may.\t");
			break;
					
		case 5:
			sprintf(mois,"june.\t");
			break;
					
		case 6:
			sprintf(mois,"jul.\t");
			break;
					
		case 7:
			sprintf(mois,"aug.\t");
			break;
					
		case 8:
			sprintf(mois,"sept.\t");
			break;
					
		case 9:
			sprintf(mois,"oct.\t");
			break;
					
		case 10:
			sprintf(mois,"nov.\t");
			break;
					
		default :
			sprintf(mois,"dec.\t");
			break;
			}

		//Derniere heure de modification
		if(mtime->tm_hour < 10)
			sprintf(heure,"0%d:",mtime -> tm_hour);
			
		else
			sprintf(heure,"%d:",mtime -> tm_hour);

		//Derniere minute de modification
		if(mtime->tm_min < 10)
		 	sprintf(minute,"0%d ",mtime -> tm_min);
		 	
		else
			sprintf(minute,"%d ",mtime->tm_min);
		
		//Affichage complet de la date (mois, jour, et date de derniere modification)
		sprintf(chaine, "%s%d\t%s%s",mois,mtime -> tm_mday,heure,minute);
		printf("%s ",chaine);	
}

//Affichage du nom du fichier
void affiche_nom(struct stat st, char buffer[], struct dirent *elt)
{
	char bufMime[128], type[128];
	int tube[2];
	pid_t pid;
	int status, i;
	char c;
	
	if(pipe(tube) == ERR)
		syserror("Pipe problem",4);

	if((pid = fork()) == ERR)
		syserror("Fork problem", 5);

			
	if(! pid)
	{
		close(tube[0]);
		__fpurge(stdout); // Purge stdout
		close(1);
		dup(tube[1]);
		close(tube[1]);
		execlp("/usr/bin/mimetype","mimetype","-b", buffer,NULL);
		syserror("Execl error", 6);
	}
	
	else
	{
		close(tube[1]);
		wait(&status);

		for(i = 0; (read(tube[0],&c,sizeof(char))) && c != '\n'; i++)
			bufMime[i] = c;
			
		bufMime[i] = '\0';

		strncpy(type,bufMime, strchr(bufMime, '/') - bufMime);				
		type[strchr(bufMime, '/') - bufMime]  ='\0';
		close(tube[0]);
	}
			
	if(((strcmp(type,"audio") == 0) || (strcmp(type,"image") == 0)))
	{
		couleur("35");
	}
	
	else if(strcmp(type,"application") == 0)
		{	
			if( strstr(strchr(bufMime, '/'), "executable")  )
			{
				couleur("32");
			}
			
			else if((strstr(strchr(bufMime, '/'), "compressed")) || (strstr(strchr(bufMime, '/'), "archive")) || (strstr(strchr(bufMime, '/'), "zip")))
				{
					couleur("31");
				}
		}
		
		else
		{
			//Affichage du nom du fichier
			switch (st.st_mode & S_IFMT) 
			{
				//Couleur bleu pour les repertoires
				case S_IFDIR: 
					couleur("34");
					break;

				//Couleur cyan pour les liens symboliques
				case S_IFLNK:
					couleur("36");
					break;

				//Couleur jaune pour les fichiers FIFO ou blocks
				case S_IFIFO:
				case S_IFBLK:
					couleur("33");
					break;	
				
				//Couleur magenta pour les fichiers audios, images ou sockets
				case S_IFSOCK:
					couleur("35");
					break;
			}
		}
		
	//Couleur par defaut pour les fichiers ordinaires
	printf("%s\n", elt->d_name);
	couleurdefaut();
}


//Affichage des donnees
void affiche_donnees(Queue *q, struct stat st, char buffer[], struct dirent *elt)
{
	char chaine[1024];
	struct passwd *pwd, *pwd2;	
	struct tm *mtime;

	if((pwd = getpwuid (st.st_uid)) == NULL)
		syserror("getpwuid error",3);

	if((pwd2 = getpwuid (st.st_gid)) == NULL)
		syserror("Getpwuid error",3);

	//Affichage des droits
	affiche_droits(q, st, elt, buffer);
	
	//Affichage du nombre de liens physique, du nom de l'utilisateur, du groupe, et la taille
	sprintf(chaine,"%u %s %s\t%lld\t",st.st_nlink,pwd -> pw_name, pwd2 -> pw_name,(long long) st.st_size);
	printf("%s",chaine);

	//Affichage de la date de la derniere modification du ficher
	mtime = localtime(&(st.st_mtim.tv_sec));
	
	//Affichage de la date
	affiche_date(mtime);
	
	affiche_nom(st,buffer,elt);
}

int main (int argc, char *argv[])
{
	Queue* nosreps;
	int nbArgvRed;
	
	if(argc == 1)
	{
		//On stocke les donnees du repertoire courant
		nosreps = stockageFichiers(argc, NULL);
	
		//On affiche les donnees
		parcours_queue(nosreps,0);
	}
	
	else
	{
		//On regarde les flags
		initflags(argc, argv);
	
		//Les supprimes de argv
		nosreps = deleteOption(argc,argv);
	
		//On regarde si il reste des choses dans argv
		nbArgvRed = queueSize(nosreps);
		
		if(nbArgvRed != 0)
		{
			//si oui
			//On regarde si les chaines de caracteres dans argv existent : destination correct
			if(verif_fichiers(argc,argv) == 0)
			{
				//si oui
				//On regarde les flags
				parcours_queue(nosreps,nbArgvRed);
			}
		}
		
		else
		{
			//si non
			//On stocke les données du repertoire courant
			nosreps = stockageFichiers(argc, NULL);
			
			//On affiche les données en fonction des options saisies
			parcours_queue(nosreps,nbArgvRed);
		}
	}
	
	freeQueue(nosreps);
	return 0;
}
