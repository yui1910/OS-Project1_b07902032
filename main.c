#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include "process.h"
#include "scheduler.h"

int main(){
	char sched_policy[32];
	int policy;
	int proc_amount;
	struct process *proc;
	scanf("%s", sched_policy);
	scanf("%d", &proc_amount);
	proc = (struct process *)malloc(proc_amount*sizeof(struct process));
	for(int i = 0 ; i < proc_amount ; i++){
		scanf("%s%d%d", proc[i].name, &proc[i].t_ready, &proc[i].t_exec);
	}
	if(strcmp(sched_policy, "FIFO") == 0){
		policy = FIFO;
	}
	else if(strcmp(sched_policy, "RR") == 0){
		policy = RR;
	}
	else if(strcmp(sched_policy, "SJF") == 0){
		policy = SJF;
	}
	else if(strcmp(sched_policy, "PSJF") == 0){
		policy = PSJF;
	}
	else{
		exit(0);
	}
	scheduling(proc, proc_amount, policy);
	exit(0);
}