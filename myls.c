#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<dirent.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
#include<stdio_ext.h>

#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<pwd.h>

#include "Queue.h"

#define couleur(p) printf("\033[%sm",p)
#define syserror(m,e) perror(m), exit(e)
#define fatalerror(m,e) fprintf(stderr, "%s\n", m), exit(e)

#define ERR -1
#define FLAG_A 0
#define FLAG_R 1

int main(int argc, char *argv[])
{
	char * defaut = ".";
	char flag[2] = {0,0}, bufferErr[128], bufferMimeType[128], type[128];
	char *s;
	DIR* d; 		//repertoire
	struct dirent *elt;	//content
	struct stat st;
	struct passwd *pwd;	
	struct tm *mtime;
	pid_t pid;
	int tube[2];
	int i;

	Queue * tail;
	QueueElt * tmp;

	argc--;
	argv++;
	
	tail = createQueue(sizeof(char *));

	//Initialisation des flags en fonction des options choisis
	for (;argc && (**argv) == '-';)
	{
		//Dans le cas ou -aR ou -Ra ont ete saisi en ligne de commande
		if ( ! strcmp("-aR", *(argv)) || ! strcmp("-Ra", *(argv)) )
		{	
			flag[FLAG_A] = 1;
			flag[FLAG_R] = 1;
		}
		//Dans le cas ou -a a ete saisi en ligne de commande 
		else if (! strcmp("-a", *(argv)))
			flag[FLAG_A] = 1;
		//Dans le cas ou -R a ete saisi en ligne de commande
		else if (! strcmp("-R", *(argv)))
			flag[FLAG_R] = 1;
		//Une option invalide a ete saisi, cas d'erreur
		else
		{
			sprintf(bufferErr, "Option invalide : %s", *(argv));
			fatalerror(bufferErr, 2);
		}
		argc--; argv++;
	}
	
	if(argc == 0)
		enqueue(tail,defaut);

	else
		for(; argv ; argv++)
			enqueue(tail,*argv);

	for(; ! isQueueEmpty(tail) ;){
		printf("%d\n", !isQueueEmpty(tail));
		tmp = dequeue(tail);
		s = (char *) tmp -> elt;

		//On ouvre le repertoire, si echec, erreur
		if(!(d = opendir(s))) syserror("Directory problem",1);
		
		if(argc > 1)
			printf("%s:\n",s);

		//On boucle tant qu'il a des elements a lire dans le repertoire
		while(elt = readdir(d))
		{ 
			char buffer[512];
			char *ext;

			memset(buffer,0,sizeof(buffer));

			//Dans le cadre du -a on passe au tour de boucle suivant si le flag est a 1 ou si le '.' est le premier caractere
			if ((*(elt->d_name) == '.') && !(flag[FLAG_A]))
				continue;

			//Afin de definir l'arboresence 
			strcat(buffer,s);
			strcat(buffer,"/");
			strcat(buffer,elt->d_name);

			//On initialise la structure stat, erreur si echec
			if(stat(buffer,&st) == ERR)
				syserror("Stat error",2);
			
			//On regarde si il s'agit d'un repertoire ou non
            	switch (st.st_mode & S_IFMT) 
		    	{
           		case S_IFDIR:     
					printf("d");
					
					//Dans le cadre du -R on ajoute le repertoire dans la file si le flag est a 1
					if(!flag[FLAG_R])
					{
						enqueue(tail,buffer);
					}
					break;
           		
				default:       
					printf("-");               
					break;
            	}

			//Affichage des differents droits associe au fichier 
			printf("%c",(st.st_mode & S_IRUSR) ? 'r' : '-' );
			printf("%c",(st.st_mode & S_IWUSR) ? 'w' : '-' );
		    printf("%c",(st.st_mode & S_IXUSR) ? 'x' : '-' );
		    printf("%c",(st.st_mode & S_IRGRP) ? 'r' : '-' );
		    printf("%c",(st.st_mode & S_IWGRP) ? 'w' : '-' );
		    printf("%c",(st.st_mode & S_IXGRP) ? 'x' : '-' );
			printf("%c",(st.st_mode & S_IROTH) ? 'r' : '-' );
			printf("%c",(st.st_mode & S_IWOTH) ? 'w' : '-' );
		 	printf("%c ",(st.st_mode & S_IXOTH) ? 'x' : '-' );
		    
			//Affichage du nombre de liens physique
			printf("%u ",st.st_nlink);

			//Affichage du nom de l'utilisateur tout en testant la valeur de retour de pwd, erreur sinon
			if((pwd = getpwuid (st.st_uid)) == NULL)
				syserror("getpwuid error",3);

			printf("%s\t",pwd->pw_name);

			//Affichage du nom de groupe tout en testant la valeur de retour de pwd, erreur sinon
			if((pwd = getpwuid (st.st_gid)) == NULL) //getgrgid pour nom du group + group * a definir *
				syserror("Getpwuid error",3);

			printf("%s\t",pwd->pw_name);

			//Affichage de la taille
        	printf("%lld\t",(long long) st.st_size);

			//Affichage de la date  de la derniere modification du ficher
			mtime = localtime(&(st.st_mtim.tv_sec));

			//On affiche le mois
			switch(mtime->tm_mon)
			{
				case 0:
					printf("jan.\t");
					break;

				case 1:
					printf("feb.\t");
					break;
					
				case 2:
					printf("mar.\t");
					break;
					
				case 3:
					printf("apr.\t");
					break;
					
				case 4:
					printf("may.\t");
					break;
					
				case 5:
					printf("june.\t");
					break;
					
				case 6:
					printf("jul.\t");
					break;
					
				case 7:
					printf("aug.\t");
					break;
					
				case 8:
					printf("sept.\t");
					break;
					
				case 9:
					printf("oct.\t");
					break;
					
				case 10:
					printf("nov.\t");
					break;
					
				default :
					printf("dec.\t");
					break;
			}

			//On affiche le jour
			printf("%d\t",mtime->tm_mday);


			//Affichege de la derniere heure de modification
			if(mtime->tm_hour < 10)
				printf("0");
			printf("%d:",mtime->tm_hour);

			if(mtime->tm_min <10)
			 	printf("0");
			printf("%d\t ",mtime->tm_min);
			
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
				execlp("/usr/bin/mimetype","mimetype","-b",elt->d_name,NULL);
				syserror("Execl error", 6);
			}
			else
			{
				int status, i;
				char c;

				close(tube[1]);
				wait(&status);

				
				for(i = 0; (read(tube[0],&c,sizeof(char))) && c != '\n'; i++)
					bufferMimeType[i] = c ;
	
				bufferMimeType[i] = '\0';
				strncpy(type,bufferMimeType,strchr(bufferMimeType, '/') - bufferMimeType );				
				type[strchr(bufferMimeType, '/') - bufferMimeType ]  ='\0';
			}
			if(((strcmp(type,"audio") == 0) || (strcmp(type,"image") == 0)))
			{
				couleur("35");
			}
			else if(strcmp(type,"application") == 0)
			{	
				if( strstr(strchr(bufferMimeType, '/'), "x-executable")  )
				{
					couleur("32");
				}
				else if(! strstr(strchr(bufferMimeType, '/'), "x-object"))
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
			couleur("0");
		}

		//Fermeture du repertoire
		closedir(d);

		freeQueueElt(tmp);
	}
	freeQueue(tail);
	return 0;
}
