#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/sysmacros.h>
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
#define TTY_LINE_SIZE 128
struct sysinfo sysInfo;

typedef struct
{
	int uid; // x
	int pid; // x
	float cpu;
	float mem;
	unsigned long vsz;
	long int rss;
	char tty[TTY_LINE_SIZE];
	char stat; // x
	unsigned long long start; //x
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

/**
 * Get processus information
 */
static ProcInfo getProcessusInformation(struct dirent* processus)
{
	FILE *file, *statusFile;
	ProcInfo info;
	char buffer[256];
	int i;
	double tempsCpu, tempsTotal;
	unsigned long utime, stime, guest_time, rsslim;
	long int cutime, cstime, cguest_time;

	// Create the stat file path and open it
	sprintf(buffer, "%s%s%s", PROC_FOLDER, processus->d_name, PROC_STAT);
	file = fopen(buffer, "r");

	// Get pid(1) 
	fscanf(file, "%d", &(info.pid));

	// Skip comm(2)
	fscanf(file, " %*s");

	// Get state(3)
	fscanf(file, " %c", &(info.stat));

	// Skip ppid(4), pgrp(5), session(6), tty(7)
	// tpgid(8), flags(9), minflt(10), cminflt(11), majflt(12),
	// cmajflt(13)
	fscanf(file, " %*d");
	fscanf(file, " %*d");
	fscanf(file, " %*d");
	fscanf(file, " %*d");
	fscanf(file, " %*d");
	fscanf(file, " %*u");
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");

	// Stocking utime(14), stime(15), cutime(16), cstime(17)
	fscanf(file, " %lu", &utime);
	fscanf(file, " %lu", &stime);
	fscanf(file, " %ld", &cutime);
	fscanf(file, " %ld", &cstime);

	// Skipping priority(18), nice(19), num_threads(20)
	// itrealvalue(21)
	fscanf(file, " %*ld");
	fscanf(file, " %*ld");
	fscanf(file, " %*ld");
	fscanf(file, " %*ld");

	// Get starttime(22), vsize(23), rss(24), rsslim(25)
	fscanf(file, " %llu", &(info.start));
	fscanf(file, " %lu", &(info.vsz));
	fscanf(file, " %ld", &(info.rss));
	fscanf(file, " %lu", &rsslim);
	
	// Skip startcode(26), endcode(27), startstack(28)
	// kstkesp(29), kstkeip(30), signal(31), blocked(32),
	// sigignore(33), sigcatch(34), wchan(35), nswap(36), cnswap(37),
	// exit_signal(38), processor(39), rt_priority(40), policy(41),
	// delayacct_blkio_ticks(42)
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");
	fscanf(file, " %*lu");
	fscanf(file, " %*d");

	fscanf(file, " %*d");
	fscanf(file, " %*u");
	fscanf(file, " %*u");
	fscanf(file, " %*llu");

	// Get guest_time(43), cguest_time(44)
	fscanf(file, " %lu %ld", &guest_time, &cguest_time);

	tempsCpu = (double)(utime + stime + cutime - guest_time);
	tempsTotal = (double)(utime + stime + cutime + cstime - guest_time - cguest_time);
	
	tempsCpu /= (tempsTotal > tempsCpu ? tempsTotal : tempsCpu);

	printf("Temps CPU Utilisé : %d%%\n", tempsCpu);

	// Create the status file path and open it
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
/*
		printf("%s\t%d\t%.1f\t%.1f\t%lu\t%ld\t%d\t%c\t%llu\t%lu\n", getpwuid(info.uid)->pw_name, info.pid, info.cpu, info.mem, info.vsz, info.rss, info.tty, info.stat, info.start, info.time);*/
	}
	closedir(procDir);

	exit(0);
}
