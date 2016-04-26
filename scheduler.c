/*
 * scheduler
 * scheduler.c
 *
 * Programmer: Sara Vandandaigue
 * Date: April 17, 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "pcb.h"
#include "scheduler.h"
#include "FIFOq.h"

#define MAX_LOOPS 6

unsigned int sys_stack;
unsigned int pc;
OS_p os;

// implements the round robin algorithm
// "ready" queue, running queue, interrupted, blocked, terminated
int main(int argc, char** argv) {
	srand(0);

	initialize();

	// Loop 6 times
	for(int i = 0; i < MAX_LOOPS; i++){
		// Create processes and put them in the created queue
		int number_pcbs = rand() % 5 + 1;
		for(int c = 0; c < number_pcbs; c++){
			PCB_p pcb = PCB_construct();
			PCB_init(pcb);
			PCB_set_state(pcb, CREATED); // created state
			FIFOq_enqueue(os->created_q, pcb);
		}

		// Main loop for scheduler and dispatcher
		run_loop();
	}
}

int initialize(void) {
	os = malloc(sizeof(OS));
	os->pc = 0;
	os->current_process = NULL;

	os->created_q = FIFOq_construct();
	os->ready_q = FIFOq_construct();
	os->running_q = FIFOq_construct();
	os->interrupted_q = FIFOq_construct();
	os->terminated_q = FIFOq_construct();
	os->io_q = FIFOq_construct();
}

void run_loop(void) {
	// Move processes from created to ready queue
	PCB_p to_move;
	while(!FIFOq_is_empty(os->created_q)) {
		to_move = FIFOq_dequeue(os->created_q);
		place_in_ready(to_move);
	}

	if(os->current_process != NULL) {
		simulate_run(os->current_process);
		pc = os->current_process->pc;
	}

	// Run ISR
	pseudo_isr();
}

void place_in_ready(PCB_p to_place) {
	printf(">PCB: %s\n",PCB_toString(to_place));
	PCB_set_state(to_place, READY);
	FIFOq_enqueue(os->ready_q, to_place);
}

void simulate_run(PCB_p pcb) {
	int rand_inc = rand() % 1001 + 3000; // Random number between 3000 and 4000
	pcb->pc += rand_inc;
}

// simulates the timer interrupt
/**
 *	Changes the state of current process to "interrupted", save CPU state
 *	to current process PCB (only PC value), and upcall.
 */
void pseudo_isr() {
	if(os->current_process != NULL) {
		PCB_set_state(os->current_process, INTERRUPTED);
		save_state(os->current_process);
	}
	scheduler(TIMER_INTERRUPT);
}

void scheduler(int interrupt_type) {
	switch(interrupt_type) {
		case TIMER_INTERRUPT:
			timer_interrupt_handler();
			break;
	}

	dispatcher();
	// print out the current ready queue
	printf(">Head: %s\n",FIFOq_tostring(os->ready_q));
	deallocate();
}

/**
 *	Dequeues process from ReadyQ and puts into running state.
 */
void dispatcher(void) {
	// get PCB from head readyQ
	PCB_p pcb = FIFOq_dequeue(os->ready_q);
	// switch to new pcb print the ready state pcb
	printf(">Switching to: %s\n",PCB_toString(pcb));

	// change state to running
	PCB_set_state(pcb, RUNNING);

	// print out the running state pcb
	printf(">Now running: %s\n",PCB_toString(pcb));
	os->current_process = pcb;
}

//  when time q expires
//	save context in PCB
//  move PCB to end readyQ
void timer_interrupt_handler() {
	if(os->current_process != NULL) {
		PCB_set_state(os->current_process, READY);
		FIFOq_enqueue(os->ready_q, os->current_process);
		printf(">Returned to Ready Queue: %s\n",PCB_toString(os->current_process));
	}
}

void pseudo_iret() {
	pc = sys_stack;
	printf("%s",PCB_toString(os->current_process));

	// get next process and run it
	PCB_p pcb = FIFOq_dequeue(os->ready_q);

	// switch to new pcb print the ready state pcb
	printf("Switching to: %s\n",PCB_toString(pcb));
	PCB_set_state(pcb, RUNNING);

	// print out the running state pcb
	printf("Now running: %s\n",PCB_toString(pcb));
	FIFOq_enqueue(os->running_q, pcb);
}

/**
 *	Saves the CPU state to the PCB of the current process.
 *	Currently only saving the PC value.
 */
void save_state(PCB_p curr_pcb) {
	if (curr_pcb != NULL)
		PCB_set_pc(os->current_process, pc);
}

/**
 * 		deallocate()
 *
 *		All processes, PCBs, in the terminated_queue are deallocated.
 */
void deallocate() {
	while (!FIFOq_is_empty(os->terminated_q)) {
		PCB_destruct(FIFOq_dequeue(os->terminated_q));
	}
}
