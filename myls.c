#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<dirent.h>
#include<time.h>
#include<string.h>

#include<sys/types.h>
#include<sys/stat.h>

#define ERR -1
#define syserror(m,e) perror(m), exit(e)

#define FLAG_A 0
#define FLAG_R 1

int main(int argc, char *argv[])
{
	char flag[2] = {0,0};
	char *s;
	DIR* d; 		//repertoire
	struct dirent *elt;	//content
	
	argc--;
	argv++;
	
	for (;argc;)
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
			break;
		argc--; argv++;
	}
	
	for(s=(argc==0) ? "." : *(argv); s ;){
		
		if(!(d = opendir(s))) syserror("Directory problem",1);
		
		printf("%s : \n",s);
		
		while(elt = readdir(d)) printf("%s \t", elt->d_name);
		
		closedir(d);
		
		putchar('\n');
		
		if  (!argc) break;
		
		if(s=*(++argv))putchar('\n');
	}
		
	return 0;
}
