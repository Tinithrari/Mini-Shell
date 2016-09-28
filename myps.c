#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define ERR -1
#define BUFFER_SIZE 512

#define SKIP_REF(dir) readdir( dir ), readdir( dir )
#define syserror(s, c) perror(s), exit(c)

#define PROC_FOLDER "/proc"

int main(void)
{
	DIR *procDir, *pidDir;
	struct dirent *processus;

	procDir = opendir( PROC_FOLDER );

	if (procDir == NULL)
		syserror("Directory error", 1);

	SKIP_REF(procDir);

	printf ("PID\tCMD\n");

	for ( processus = readdir( procDir ); processus; processus = readdir( procDir ) )
	{
		struct stat* status;
		char buffer[BUFFER_SIZE];
		int fd;
		ssize_t size;

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
		sprintf(buffer, "%s/%s/cmdline", procDir->d_name, processus->d_name);
		fd = open(buffer, O_RDONLY);

		if (fd == ERR)
		{
			closedir(procDir);
			closedir(processus);
			syserror("Open error", 3);
		}
		
		for (; (size = read(fd, buffer, BUFFER_SIZE)) == BUFFER_SIZE ;)
			printf("%s", buffer);

		close(fd);

		if (size)
		{
			closedir(procDir);
			closedir(processus);
			syserror("Read error", 4);
		}
		putchar('\n');
		closedir(processus);
	}
	closedir(procdir);
}
