#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<dirent.h>
#include<time.h>
#include<string.h>
#include<unistd.h>


#include<sys/types.h>
#include<sys/stat.h>

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
			
			printf("%s \t", elt->d_name);

			printf("File type:                ");

           switch (st.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  printf("directory\n");               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  printf("symlink\n");                 break;
           case S_IFREG:  printf("regular file\n");            break;
           case S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                break;
           }

           printf("I-node number:            %ld\n", (long) st.st_ino);

           printf("Mode:                     %lo (octal)\n",
                   (unsigned long) st.st_mode);

           printf("Link count:               %ld\n", (long) st.st_nlink);
           printf("Ownership:                UID=%ld   GID=%ld\n",
                   (long) st.st_uid, (long) st.st_gid);

           printf("Preferred I/O block size: %ld bytes\n",
                   (long) st.st_blksize);
           printf("File size:                %lld bytes\n",
                   (long long) st.st_size);
           printf("Blocks allocated:         %lld\n",
                   (long long) st.st_blocks);

           printf("Last status change:       %s", ctime(&st.st_ctime));
           printf("Last file access:         %s", ctime(&st.st_atime));
           printf("Last file modification:   %s \n", ctime(&st.st_mtime));

		}

		closedir(d);
		
		//putchar('\n');

		if  (!argc) break;
		
		if(s=*(++argv))putchar('\n');
	}
		
	return 0;
}
