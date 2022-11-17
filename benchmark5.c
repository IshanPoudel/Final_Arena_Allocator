
#include "mavalloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINKED_LIST_SIZE 65534



int current=-99;
int previous=-99;

enum TYPE 
{
    PROCESS=0 , HOLE
};

const char *enum_string[] = {
    "PROCESS",
    "HOLE",
    
};


struct Node
{
    int in_use;
    struct Node* previous;
    struct Node* next;

    size_t size;

    enum  TYPE type;

    void* arena;

};

struct Node *head;
struct Node *temp;
void *arena;





void * insertNode(int size)
{

  temp = (struct Node*) malloc(sizeof(struct Node));
  temp->size=size;
  temp->type=PROCESS;
  temp->arena=temp;
  temp->next=NULL;
  temp->previous = NULL;

  if(head==NULL)
  {
    head=temp;
    return head->arena;
  }

  struct Node* temp_2=head;

  while(temp_2->next!=NULL)
  {
    temp_2=temp_2->next;
  }

  temp_2->next=temp;
  temp->previous=temp_2;

  return temp->arena;

  // printf("Added process of size %zu\n" , temp->size);


  

}

int removeNode(void *ptr)
{
  temp=head;

  struct Node* prev ;

  while(temp!=NULL && temp->arena!=ptr)
  {
    prev=temp;
    temp=temp->next;  

  }

  if(temp==NULL)
  {
    return -1;
  }
  prev->next=temp->next;
  temp->next->previous=prev;
  // printf("I freed value at %p\n" , ptr);
  free(temp);
  return 0;

  
}

int main()
{

  clock_t start , end;

  double execution_time;

  start=clock();

  unsigned char* array[10000];

  for (int i=0; i<10000;i++)
  {
    array[i]=insertNode(100+i);
  }

  for (int i=1; i<10000; i++)
  {
    if (i % 2==0 && i%3==0)
    {
      removeNode(array[i]);
    }
  }

  char *ptr_1 = insertNode(2000);
  char *ptr_2 = insertNode(5000);
  // free(ptr_2);
  char *ptr3=insertNode(20);

  removeNode(ptr_1);

  for (int i =5000 ; i<1000;i++)
  {
    if (i%5==0)
    {
      removeNode(array[i]);
    }
  }

  unsigned char* second_array [400];

  for (int i=200; i<600;i++)
  {

    second_array[i]= insertNode(i);

  }

  removeNode(ptr3);


  for (int j=0; j<300;j++)
  {
    insertNode(j*230);
  }

  removeNode(ptr_1);

  for (int k=360; k<500; k++)
  {
    insertNode(k*3);
  }

 

  end=clock();

  execution_time=((double) (end-start))/CLOCKS_PER_SEC;
  printf("Execution time for benchmark5.c MALLOC : %f\n" , execution_time);


  

  

}