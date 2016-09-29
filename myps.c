#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ERR -1
#define BUFFER_SIZE 512

#define syserror(s, c) perror(s), exit(c)

#define PROC_FOLDER "/proc"

int isNumber(char* s)
{
	char *c;

	if (s == NULL)
		return 0;

	for (c = s; (*c) && (*c >= '0' && *c <= '9') ; c++);

	return !(*c);
}

int main(void)
{
	DIR *procDir;
	struct dirent *processus;

	procDir = opendir( PROC_FOLDER );

	if (procDir == NULL)
		syserror("Directory error", 1);


	printf ("PID\tCMD\n");

	for ( processus = readdir( procDir ); processus; processus = readdir( procDir ) )
	{
		struct stat* status;
		char buffer[BUFFER_SIZE], c;
		int fd;
		ssize_t size;

		if (! isNumber(processus->d_name))
			continue;

		printf("%s\t", processus->d_name);
/*
		sprintf(buffer, "%s/%s", procdir->d_name, processus->d_name);

		if (stat(buffer, status) == ERR)
		{
			closedir(procDir);
			closedir(processus);
			syserror("Status error", 2);
		}

		printf("%d:%d:%d\t", status->st_mtim.tm_hour, status->st_mtim.tm_min, status-> st_mtim.tm_sec);
*/

		sprintf(buffer, "%s/%s/cmdline", PROC_FOLDER, processus->d_name);

		fd = open(buffer, O_RDONLY);

		if (fd == ERR)
		{
			closedir(procDir);
			syserror(buffer, 3);
		}
	
		memset(buffer, 0, BUFFER_SIZE);
	
		for (; (size = read(fd, &c, sizeof(char))) > 0;)
			putchar(c);

		if (size == ERR)
			syserror("Read error", 4);
		
		close(fd);
		putchar('\n');
	}
	closedir(procDir);
}