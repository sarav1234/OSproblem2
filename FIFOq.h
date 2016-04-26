#ifndef FIFOQ_H_
#define FIFOQ_H_

#include "pcb.h"

typedef struct node {
  PCB_p pcb;
  struct node* next;
} Node;

typedef struct {
  Node* head;
  int size;
} FIFOq;

typedef FIFOq* FIFOq_p;

FIFOq_p FIFOq_construct(void);
void FIFOq_destruct(FIFOq_p);
int FIFOq_is_empty(FIFOq_p);
void FIFOq_enqueue(FIFOq_p, PCB_p);
PCB_p FIFOq_dequeue(FIFOq_p);
char* FIFOq_tostring(FIFOq_p);

#endif
