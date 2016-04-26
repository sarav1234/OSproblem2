#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/*
 * test.c
 * Test harness for Problem 2. This is a black box testing procedure which 
 * validates the text of the programs output.
 * 
 * Chris Ottersen
 */
int main()
{
  FILE *report = fopen("scheduleTrace.txt", "r");
  int count;
  char currentMessage[50], expectedMessage[50];
  char currentState[50], expectedState[50];
  
  struct state {
    char *message;
    unsigned int pid;
    unsigned int pc;
    char *state;
  } current, expected, tmp;
  current.message = currentMessage;
  expected.message = expectedMessage;
  
  current.state = currentState;
  expected.state = expectedState;
  while(count = fscanf(report, ">%49[a-zA-Z ]: PID: %X, PC: %X, STATE: %49s\n", 
      current.message, &(current.pid), &(current.pc), current.state))
  {
    if(count == 1)//head
    {
      char *tmp;
      int qcount;
      char test[50];
      int id;
      
      if(fscanf(report, "Q:Count=%d: ", &qcount))
      {
        if(strcmp(current.message, "Head") != 0)
        {
          printf("invalid message error: %s", current.message);
          exit(1);
        }
        printf(test);
        printf(">Head: Q:Count=%d:", qcount);
        while(qcount-- >= 0 && fscanf(report, "P%d-", &id))
        {
          
          printf("P%d->", id);
          fscanf(report, ">");
        }
        printf("-*\n");
        if(getc(report) != '*')
        {
          printf("unexpected termination");
          exit(1);
        }
        fscanf(report, "\n");
      }
    }
    else
    {
      //if(strcmp(previous.state))
      
      
      if(strcmp(current.message, "PCB") == 0)
      {
        //TODO:no tests
      }
      else if(strcmp(current.message, "Returned to Ready Queue") == 0)
      {
        //TODO:no tests
        /*if(current.pc != expected.pc)
        {
          printf("bad pc %d (expected %d)\n", current.pc, expected.pc);
          exit(1);
        }*/
      }
      else if(strcmp(current.message, "Switching to") == 0)
      {
        expected.pid = current.pid;
      }
      else if(strcmp(current.message, "Now running") == 0)
      {
        if(current.pid != expected.pid)
        {
          printf("bad pid %d (expected %d)\n", current.pid, expected.pid);
          exit(1);
        }
      }
      else if(strcmp(current.message, "PCB") == 0)
      {
        printf("unrecognized message");
        exit(1);
      }
      printf(">%s: PID: 0x%04X, PC: 0x%04X, STATE: %s\n", current.message, current.pid, current.pc, current.state);
    }
    
  }
  
  fclose(report);
  return 0;
}