#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <dirent.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pwd.h>

#define ERR -1
#define BUFFER_SIZE 512

#define syserror(s, c) perror(s), exit(c)

#define PROC_FOLDER "/proc/"
#define PROC_STAT "/stat"
#define PROC_STATUS "/status"

#define LINE_BEFORE_UID 7

struct sysinfo sysInfo;

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

static ProcInfo getProcessusInformation(struct dirent* processus)
{
	FILE *file, *statusFile;
	unsigned long utime, stime;
	long int waitTimeU, waitTimeS;
	ProcInfo info;
	char buffer[256];
	struct stat procStat;
	int i;

	sprintf(buffer, "%s%s%s", PROC_FOLDER, processus->d_name, PROC_STAT);
	// Open the file descriptor to the stat file
	file = fopen(buffer, "r");
	stat(buffer, &procStat);

	// Get pid, stat and tty
	fscanf(file, "%d%*s%c%*d%*d%*d%d", &(info.pid), &(info.stat), &(info.tty));

	// Skip argument
	fscanf(file, "%*d%*u%*lu%*lu%*lu%*lu");
		
	// Get Time
	fscanf(file, "%lu%lu%ld%ld", &utime, &stime, &waitTimeU, &waitTimeS);

	info.time = utime + stime;

	// Skip other arguments
	fscanf(file, "%*ld%*ld%*ld%*ld");

	// Get start time, vsize, and rss
	fscanf(file, "%llu%lu%ld", &(info.start), &(info.vsz), &(info.rss));

	// Compute the cpu usage percentage
	info.cpu = (info.time / (info.time + waitTimeU + waitTimeS)) * 100;
	info.time /= CLOCKS_PER_SEC;
	info.start /= CLOCKS_PER_SEC;
	

	// Compute the memory usage percentage
	info.mem = ( ( (float) info.rss ) / sysInfo.totalram ) * 100;

	sprintf(buffer, "%s%s%s", PROC_FOLDER, processus->d_name, PROC_STATUS);
	statusFile = fopen(buffer, "r");

	// Skip Lines
	for (i = 0; i < LINE_BEFORE_UID; i++)
		fscanf(statusFile, "%*[^\n]\n");

	// get uid
	fscanf(statusFile, "%*[^0-9]%d", &(info.uid));

	// close the stat file and the status file
	fclose(file);
	fclose(statusFile);

	return info;
}

int main(void)
{
	DIR *procDir;
	struct dirent *processus;
	ProcInfo info;

	// Get system information
	if (sysinfo(&sysInfo) == ERR)
		syserror("sysinfo error", 1);

	// Open the proc folder
	procDir = opendir( PROC_FOLDER );

	if (procDir == NULL)
		syserror("Directory error", 1);

	// Display Header
	printf ("USR\tPID\t%cCPU\t%cMEM\tVSZ\tRSS\tTTY\tSTAT\tSTART\tTIME\tCOMMAND\n", '%', '%');

	for ( processus = readdir( procDir ); processus; processus = readdir( procDir ) )
	{
		// Check if the directory name is a PID
		if (! isNumber(processus->d_name))
			continue;
		
		info = getProcessusInformation(processus);

		printf("%s\t%d\t%.1f\t%.1f\t%lu\t%ld\t%d\t%c\t%llu\t%lu\n", getpwuid(info.uid)->pw_name, info.pid, info.cpu, info.mem, info.vsz, info.rss, info.tty, info.stat, info.start, info.time);
	}
	closedir(procDir);

	exit(0);
}
