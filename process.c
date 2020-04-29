#define _GNU_SOURCE
#include <fcntl.h>
#include "process.h"
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>

int proc_assign_cpu(int pid, int core){
	if(core > sizeof(cpu_set_t)){
		return -1;
	}
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core, &mask);
	return 0;
}

void dmesg(const char *tag, const char *msg, const int len){
    const int TAG_LEN = 3;
    char buffer[128] = {0};
    memcpy(&buffer[0], tag, TAG_LEN);
    memcpy(&buffer[TAG_LEN], msg, len);
    int fd_kmsg = open("/dev/kmsg", O_WRONLY);
    write(fd_kmsg, &buffer, TAG_LEN+len);
    close(fd_kmsg);
}

int proc_exec(struct process proc){
	int pid = fork();
	if(pid < 0){
		return -1;
	}
	if(pid == 0){
		unsigned long start_sec, start_msec, end_sec, end_msec;
		char to_dmesg[200];
		struct timeval tv; 
    	struct timezone tz; 
    	gettimeofday(&tv, &tz);
    	start_sec = tv.tv_sec;
    	start_msec = tv.tv_usec;
    	for(int i = 0 ; i < proc.t_exec ; i++) {
			UNIT_T();
		}
		gettimeofday(&tv, &tz);
    	end_sec = tv.tv_sec;
    	end_msec = tv.tv_usec;
    	sprintf(to_dmesg, "[Project1] %d %lu.%09lu %lu.%09lu\n", getpid(), start_sec, start_msec, end_sec, end_msec);
		dmesg("<5>", to_dmesg, strlen(to_dmesg));
		exit(0);
	}
	/* Assign child to another core prevent from interupted by parant */
	proc_assign_cpu(pid, CHILD_CPU);
	return pid;
}

int proc_block(int pid){
	struct sched_param param;
	/* SCHED_IDLE should set priority to 0 */
	param.sched_priority = 0;
	int ret = sched_setscheduler(pid, SCHED_IDLE, &param);
	if(ret < 0){
		return -1;
	}
	return ret;
}

int proc_wakeup(int pid){
	struct sched_param param;
	/* SCHED_OTHER should set priority to 0 */
	param.sched_priority = 0;
	int ret = sched_setscheduler(pid, SCHED_OTHER, &param);
	if(ret < 0){
		return -1;
	}
	return ret;
}