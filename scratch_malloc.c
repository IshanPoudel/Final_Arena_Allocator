

#include "mavalloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINKED_LIST_SIZE 65534

static int initialized=0;
static int rootNode=0;

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
    int previous;
    int next;

    size_t size;

    enum  TYPE type;

    void* arena;

};

enum ALGORITHM global_algorithm;

static struct Node LinkedList[MAX_LINKED_LIST_SIZE];


int main()
{
  clock_t start , end;

  double execution_time;

  start=clock();


  


  unsigned char* array[10000];

  for (int i=0; i<10000;i++)
  {
    array[i]=malloc(100+i);
  }

  // //printfList();

  for (int i=0; i<10000; i++)
  {
    if (i % 2==0 && i%3==0)
    {
      free(array[i]);
    }
  }



  char *ptr_1 = malloc(2000);
  char *ptr_2 = malloc(5000);
  free(ptr_2);
  char *ptr3=malloc(20);


  free(ptr_1);

  for (int i =5000 ; i<1000;i++)
  {
    if (i%5==0)
    {
      free(array[i]);
    }
  }

  unsigned char* second_array [400];

  for (int i=200; i<600;i++)
  {

    second_array[i]= malloc(i);

  }

  free(ptr3);


  for (int j=0; j<300;j++)
  {
    void* val = malloc(j*230);
  }

  free(ptr_1);

  for (int k=360; k<500; k++)
  {
    void* val = malloc(k*3);
  }

  
  end=clock();

  execution_time=((double) (end-start))/CLOCKS_PER_SEC;
  printf("Execution time for benchmark2.c NEXT_FIT : %f\n" , execution_time);

  // printList();







    // //Test case 7
    
    // mavalloc_init( 75000, BEST_FIT );
    // char * ptr1    = ( char * ) mavalloc_alloc ( 65535 );
    // char * buffer1 = ( char * ) mavalloc_alloc( 1 );
    // char * ptr4    = ( char * ) mavalloc_alloc ( 65 );
    // char * buffer2 = ( char * ) mavalloc_alloc( 1 );
    // char * ptr2    = ( char * ) mavalloc_alloc ( 1500 );
    // printList();

    // //printf("%p\n" , ptr1);

    
    //  //Test case 3
     
    // // mavalloc_init( 65600, BEST_FIT );

    // // char * ptr1    = (char*)mavalloc_alloc( 65536 );
    // // char * ptr2    = (char*)mavalloc_alloc( 64 );
    // // printList();
     
    // // //printf("%p\n" , ptr1);
    // // //printf("%p\n" , ptr2);
    // // //printf("%d\n" ,mavalloc_size());

    // //Test Case 2
    // //  mavalloc_init( 128000, BEST_FIT );
    // //  char * ptr1    = (char*)mavalloc_alloc( 65535 );
    // //  char * ptr2    = (char*)mavalloc_alloc( 65 );
    // //  printList();
     
    // //  //printf("%p\n" , ptr1);
    // //  //printf("%p\n" , ptr2);
    // //  //printf("%d\n" ,mavalloc_size());
    // //Start with test cases for best_fit
    // //Test case 1
    // // mavalloc_init( 65535, BEST_FIT );
    // // char * ptr = ( char * ) mavalloc_alloc ( 65535 );
    // // //printf("I allocated the whole thing\n");
    // // //printf("The return address %p\n" , ptr );
    // // printList();
    
}

//When process is equal to hole size.