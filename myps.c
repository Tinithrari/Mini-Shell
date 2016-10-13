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

#define PROC_FOLDER "/proc/"
#define PROC_STAT "/stat"

typedef struct
{
	int uid;
	int pid;
	float cpu;
	float mem;
	unsigned long vsz;
	long int rss;
	int tty;
	char stat;
	unsigned long long start;
	unsigned long time;
} ProcInfo;

/**
 * @fn int isNumber(char *s)
 * @brief check if a string is a number
 * @param s the string to check
 * @return 1 if the string is a number, 0 otherwise
 */
static int isNumber(char* s)
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
	ProcInfo info;

	procDir = opendir( PROC_FOLDER );

	if (procDir == NULL)
		syserror("Directory error", 1);

	// Display Header
	printf ("USR\tPID\t%cCPU\t%cMEM\tVSZ\tRSS\tTTY\tSTAT\tSTART\tTIME\tCOMMAND\n", '%', '%');

	for ( processus = readdir( procDir ); processus; processus = readdir( procDir ) )
	{
		FILE* file;
		unsigned long utime, stime;

		// Check if the directory name is a PID
		if (! isNumber(processus->d_name))
			continue;
		
		// Open the file descriptor to the stat file
		file = open(PROC_FOLDER + processus->d_name  + PROC_STAT, "r");

		// Get pid, stat and tty
		fscanf(file, "%d%s*%c%d*%d*%d*%d", &(info.pid), &(info.stat), &(info.tty));

		// Skip argument
		fscanf(file, "%d*%u*%lu*%lu*%lu*%lu*");
		
		// Get Time
		fscanf("%lu%lu", &utime, &stime);

		info.time = utime + stime;

		// Skip other arguments
		fscanf(file, "%ld*%ld*%ld*%ld*%ld*%ld*");

		// Get start time, vsize, and rss
		fscanf(file, "%llu%lu%ld", &(info.start), &(info.vsize), &(info.rss));

		// close the stat file
		fclose(file);

		// Display PID
		printf("%s\t", processus->d_name);
	}
	closedir(procDir);
}
