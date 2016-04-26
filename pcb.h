#ifndef PCB_H_
#define PCB_H_

#define ERROR 0
#define SUCCESS 1

typedef enum state_type {CREATED, READY, RUNNING, INTERRUPTED, WAITING, HALTED} state_t;

typedef struct pcb {
    // ToDo: include data to be used
	unsigned long pid;        // process ID #, a unique number
	state_t state;    // process state (running, waiting, etc.)
	unsigned short priority;  // priorities 0=highest, 15=lowest
	unsigned short pc;         // holds the current pc value when preempted
} PCB;
typedef PCB * PCB_p;

PCB_p PCB_construct (void); // returns a pcb pointer to heap allocation
void PCB_destruct(PCB_p pcb);  // deallocates pcb from the heap
int PCB_init (PCB_p pcb);       // sets default values for member data
int PCB_set_pid (PCB_p pcb, unsigned long pid);
int PCB_set_pc (PCB_p pcb, unsigned long pc);
int PCB_set_priority (PCB_p pcb, unsigned short priority);
int PCB_set_state(PCB_p pcb, state_t state);

char * PCB_toString (PCB_p);  // returns a string representing the contents of the pcb

#endif
