#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<dirent.h>
#include<time.h>

#include<sys/types.h>
#include<sys/stat.h>

#define ERR -1
#define syserror(m,e) perror(m), exit(e)

int main(int argc, char *argv[])
{
	char *s;
	DIR* d; 		//repertoire
	struct dirent *elt;	//content
	
	for(s=(argc==1) ? "." : *(++argv); s ;){
		
		if(!(d = opendir(s))) syserror("Directory problem",1);
		
		printf("%s : \n",s);
		
		while(elt = readdir(d)) printf("%s \t", elt->d_name);
		
		closedir(d);
		
		putchar('\n');
		
		if(s=*(++argv))putchar('\n');
	}
		
	return 0;
}
