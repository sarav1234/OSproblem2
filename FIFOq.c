#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FIFOq.h"

FIFOq_p FIFOq_construct(void){
        FIFOq_p queue = (FIFOq_p) malloc(sizeof(FIFOq));
        queue->size = 0;
        queue->head = NULL;
        return queue;
}

void FIFOq_destruct(FIFOq_p queue){
        free(queue);
}

int FIFOq_is_empty(FIFOq_p queue){
        return !(queue->size);
}

void FIFOq_enqueue(FIFOq_p queue, PCB_p pcb_param){
        Node* n = malloc(sizeof(Node));
        n->next = NULL;
        n->pcb=pcb_param;

        if(queue->head == NULL) queue->head = n;
        else {
          Node* cur = queue->head;
          while(cur->next!= NULL) cur=(Node*)cur->next;
          cur->next=n;
        }

        queue->size++;
}

PCB_p FIFOq_dequeue(FIFOq_p queue){
        PCB_p temp = (PCB_p) queue->head->pcb;
        
        queue->head=queue->head->next;
        queue->size--;

        return temp;
}

char* FIFOq_tostring(FIFOq_p queue){
        char* result = (char*)calloc(256, sizeof(char));
        char* pids = (char*)calloc(128, sizeof(char));
        char* temp = (char*)calloc(32, sizeof(char));

        sprintf(result, "Q:Count=%d: ", queue->size);
        Node * n=queue->head;
        while(n!=NULL) {
          sprintf(temp, "P%d->", n->pcb->pid);
          strcat(pids,temp);
                n=(Node*)(n->next);
        }

        pids[strlen(pids)-1]='*';
        strcat(result,pids);
        return result;
}
