#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pcb.h"

int id = 0;

// Returns a pcb pointer to heap allocation
PCB_p PCB_construct (void) {
	PCB_p pcb = (PCB_p) malloc(sizeof(PCB));
	return pcb;
}

// Deallocates pcb from the heap
void PCB_destruct(PCB_p pcb){
	free(pcb);
}

// sets default values for member data
int PCB_init (PCB_p pcb){
	pcb->pid = id;
	id++;
	pcb->state = 0;
	pcb->priority = 15;
	pcb->pc = 0x1000;
	return 0;
}

int PCB_set_pid (PCB_p pcb, unsigned long pid){
	pcb->pid = pid;
	return 0;
}

int PCB_set_pc (PCB_p pcb, unsigned long pc){
	pcb->pc = pc;
	return 0;
}

int PCB_set_priority (PCB_p pcb, unsigned short priority){
	pcb->priority = priority;
	return 0;
}

int PCB_set_state(PCB_p pcb, state_t state) {
	pcb->state = state;
	return 0;
}

// Get a string representing the contents of the pcb
char * PCB_toString (PCB_p pcb){
	char* str_buffer = (char*) calloc(128, sizeof(char));
	char* state_string = (char*) calloc(16, sizeof(char));
	switch(pcb->state) {
		case CREATED:
			strcpy(state_string, "CREATED");
			break;
		case READY:
			strcpy(state_string, "READY");
			break;
		case RUNNING:
			strcpy(state_string, "RUNNING");
			break;
		case INTERRUPTED:
			strcpy(state_string, "INTERRUPTED");
			break;
		case WAITING:
			strcpy(state_string, "WAITING");
			break;
		case HALTED:
			strcpy(state_string, "HALTED");
			break;
	}
	sprintf(str_buffer, "PID: 0x%04X, PC: 0x%04X, STATE: %s",
			(unsigned int) pcb->pid, pcb->pc, state_string);

	return str_buffer;
}
