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
	
	for (;argc && (**argv) == '-';)
	{
		if ( ! strcmp("-aR", *(argv)) || ! strcmp("-Ra", *(argv)) )
		{
			flag[FLAG_A] = 1;
			flag[FLAG_R] = 1;
		}
		else if (! strcmp("-a", *(argv)))
			flag[FLAG_A] = 1;
		else if (! strcmp("-R", *(argv)))
			flag[FLAG_R] = 1;
		else
		{
			sprintf(bufferErr, "Option invalide : %s", *(argv));
			fatalerror(bufferErr, 2);
		}
		argc--; argv++;
	}
	
	for(s=(argc==0) ? "." : *(argv); s ;){
		
		if(!(d = opendir(s))) syserror("Directory problem",1);
		
		printf("%s : \n",s);
		
		while(elt = readdir(d))
		{ 
			if(stat(elt->d_name,&st) == ERR)
				syserror("Stat error",2);

           switch (st.st_mode & S_IFMT) 
		   {
           		case S_IFDIR: 
				    printf("d");               
					break;
           		default:       
				   	printf("-");               
				   	break;
           }

		    printf("%c",(st.st_mode & S_IRUSR) ? 'r' : '-' );
			printf("%c",(st.st_mode & S_IWUSR) ? 'w' : '-' );
		    printf("%c",(st.st_mode & S_IXUSR) ? 'x' : '-' );
		    printf("%c",(st.st_mode & S_IRGRP) ? 'r' : '-' );
		    printf("%c",(st.st_mode & S_IWGRP) ? 'w' : '-' );
		    printf("%c",(st.st_mode & S_IXGRP) ? 'x' : '-' );
			printf("%c",(st.st_mode & S_IROTH) ? 'r' : '-' );
			printf("%c",(st.st_mode & S_IWOTH) ? 'w' : '-' );
		 	printf("%c ",(st.st_mode & S_IXOTH) ? 'x' : '-' );
		    
			printf("%d ",st.st_nlink);

			pwd = getpwuid (st.st_uid); //appel systeme, test valeur de retour
			printf("%s ",pwd->pw_name);

			pwd = getpwuid (st.st_gid); //appel systeme, test valeur de retour
										//getgrgid pour nom du group + group * a definir *
			printf("%s ",pwd->pw_name);

        	printf("%lld ",(long long) st.st_size);

			/*printf("%c%d:%c%d",
			((st.st_mtim.tv_sec/3600)>10) ? '' : '0',
			 st.st_mtim.tv_sec/3600,
			(((st.st_mtim.tv_sec%3600)/60)>10) ? '' : '0',
			 (st.st_mtim.tv_sec%3600)/60);*/

			mtime = localtime(&(st.st_mtim.tv_sec));

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

			 printf("%d ",mtime->tm_mday);

			 if(mtime->tm_hour < 10)
			 	printf("0");
			 printf("%d:",mtime->tm_hour);

			 if(mtime->tm_min <10)
			 	printf("0");
			 printf("%d ",mtime->tm_min);
			

			printf("%s \n", elt->d_name);
		}

		closedir(d);
		
		//putchar('\n');

		if  (!argc) break;
		
		if(s=*(++argv))putchar('\n');
	}
		
	return 0;
}
