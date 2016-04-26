#ifndef SCHEDULER_H
#define SCHEDULER_H

//#include "pcb.h"
#include "FIFOq.h"

#define ERROR 0
#define SUCCESS 1
#define RANDOM_MIN 0
#define RANDOM_MAX 5

#define TIMER_INTERRUPT 1

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ADT - OS
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef struct os {
	PCB_p current_process;
	FIFOq_p created_q;
	FIFOq_p ready_q;
	FIFOq_p running_q;
	FIFOq_p interrupted_q;
	FIFOq_p terminated_q;
	FIFOq_p io_q;
	unsigned int pc;
} OS;

typedef OS * OS_p;


// prototypes
int initialize(void);
void run_loop(void);
void place_in_ready(PCB_p);
void simulate_run(PCB_p);
void pseudo_isr(void);
void pseudo_iret(void);
void save_state(PCB_p curr_pcb);
void scheduler(int int_type);
void deallocate(void);
void dispatcher(void);
// handlers
void timer_interrupt_handler(void);

#endif
