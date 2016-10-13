#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<dirent.h>
#include<time.h>
#include<string.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<pwd.h>

#define ERR -1
#define syserror(m,e) perror(m), exit(e)
#define fatalerror(m,e) fprintf(stderr, "%s\n", m), exit(e)

#define FLAG_A 0
#define FLAG_R 1

int main(int argc, char *argv[])
{
	char flag[2] = {0,0}, bufferErr[128];
	char *s;
	DIR* d; 		//repertoire
	struct dirent *elt;	//content
	struct stat st;
	struct passwd *pwd;	
	struct tm *mtime;

	argc--;
	argv++;
	
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
	
	for(s=(argc==0) ? "." : *(argv); s ;){
		
		//On ouvre le repertoire, si echec, erreur
		if(!(d = opendir(s))) syserror("Directory problem",1);
		
		if(argc > 1)
			printf("%s:\n",s);

		//On boucle tant qu'il a des elements a lire dans le repertoire
		while(elt = readdir(d))
		{ 
			char buffer[512];

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
			printf("%d ",st.st_nlink);

			//Affichage du nom de l'utilisateur
			pwd = getpwuid (st.st_uid); //appel systeme, test valeur de retour
			printf("%s ",pwd->pw_name);

			//Affichage du nom de groupe
			pwd = getpwuid (st.st_gid); //appel systeme, test valeur de retour
										//getgrgid pour nom du group + group * a definir *
			printf("%s ",pwd->pw_name);


			//Affichage de la taille
        	printf("%lld ",(long long) st.st_size);

			//Affichage de la date  de la derniere modification du ficher
			mtime = localtime(&(st.st_mtim.tv_sec));

			//On affiche le mois
			switch(mtime->tm_mon)
			{
				case 0:
					printf("jan. ");
					break;

				case 1:
					printf("feb. ");
					break;
					
				case 2:
					printf("mar. ");
					break;
					
				case 3:
					printf("apr. ");
					break;
					
				case 4:
					printf("may. ");
					break;
					
				case 5:
					printf("june. ");
					break;
					
				case 6:
					printf("jul. ");
					break;
					
				case 7:
					printf("aug. ");
					break;
					
				case 8:
					printf("sept. ");
					break;
					
				case 9:
					printf("oct. ");
					break;
					
				case 10:
					printf("nov. ");
					break;
					
				default :
					printf("dec. ");
					break;
			}

			 //On affiche le jour
			 printf("%d ",mtime->tm_mday);


			 //Affichege de la derniere heure de modification
			 if(mtime->tm_hour < 10)
			 	printf("0");
			 printf("%d:",mtime->tm_hour);

			 if(mtime->tm_min <10)
			 	printf("0");
			 printf("%d ",mtime->tm_min);
			
			 //Affichage du nom du ficher 
			 printf("%s \n", elt->d_name);
		}

		//Fermeture du repertoire
		closedir(d);

		if  (!argc) break;
		
		if(s=*(++argv))putchar('\n');
	}
		
	return 0;
}
