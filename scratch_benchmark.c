

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

/* Helper functions for printing*/
void printNode(int index)
{
    printf("\n");
    printf("LinkedList[%d] is a %s of size %zu\n" , index , enum_string[LinkedList[index].type] , LinkedList[index].size);
    printf("LinkedList[%d] ------- LinkedList[%d] -------- LinkedList[%d]\n" , LinkedList[index].previous , index , LinkedList[index].next);
    printf("LinkedList[%d] is located at %p\n" , index , LinkedList[index].arena );
    printf("\n");

}

void printList()
{
    int index=rootNode;
    //printf("\n\n");

    while( (index != -1) & (LinkedList[index].in_use))
    {
        // printNode(index);
        index=LinkedList[index].next;
    }

}

int findfreeNodeInternal(size_t size)
{
    for(int i=0; i<MAX_LINKED_LIST_SIZE;i++)
    {
        if (LinkedList[i].in_use==0 )
        {
            
            //printf("\n I found a free node at %d to insert process of size%zu\n" , i , size);
            // printNode(i);
            
            return i;

        }

        
    }

    return-1;
}
/*End of helper functions*/

int mavalloc_size( )
{
    int count=0;
    for (int i=0; i<MAX_LINKED_LIST_SIZE;i++)
    {
        if(LinkedList[i].in_use)
        {
            count++;
        }
    }

    return count;
}
/*Insertion of node function*/
int insertNodeInternal(int previous_index , int current_index , int size )
{
    //Insert node after previous_index
    //A--B--C--D--E--F--Hole
    //previous_index=E , current_index=H
    //A--B--C--D--E--H--F--HOLE

    // Insert the process after previous_index .
    // The node after previous_index is always a hole.
    // Insert process , add hole.
    // Insert at current_index.

   


    LinkedList[current_index].next=-1;
    LinkedList[current_index].previous=-1;

    

    if (LinkedList[rootNode].previous==-1 && LinkedList[rootNode].next==-1)
    {
       

        //printf("I am adding when everything is empty. Need to initialize with a hole\n");

        //Need to also check if the head is a perfect hole. 
        if(LinkedList[rootNode].size==size)
        {
            LinkedList[rootNode].type=PROCESS;
            return rootNode;

        }
        LinkedList[current_index].next=rootNode;
        LinkedList[rootNode].previous=current_index;
        LinkedList[current_index].previous=-1;

        //make linkedlist[rootnode] a hole.//Only come here when the hole size is bigger then requested size
        LinkedList[rootNode].size=LinkedList[rootNode].size-size;
        LinkedList[rootNode].type=HOLE;
        LinkedList[rootNode].next=-1;
        rootNode=current_index;
       
        // //printf("The next free is at %d\n" , next_free);

        //printf("I am returning the index %d where i added %d when I added to an empty head\n" , rootNode , size );
        return rootNode;

        
        

    }
    else if (previous_index>=0)
    {

        
        
        
        //printf("I am here when previous_index >= 0  . The value of previous_index is %d. \n\n" , previous_index);
        
        // A-Hole , Insert B
        
        // B-A-Hole
        //prev=A
        //INSERT C such that B--A----HOLE

        //  B--A--(500) C is also 500.
        
        int  index_of_hole = LinkedList[previous_index].next;

       
        if (LinkedList[index_of_hole].size==size)
        {
            //printf("I found the perfect hole\n\n");
            current_index=index_of_hole;
            LinkedList[current_index].type=PROCESS;
            LinkedList[current_index].previous=previous_index;
            LinkedList[previous_index].next= current_index;
            //printf("I am returning the index %d where i added %d when I added an in between in a perfect hole\n" , current_index , size );
            return current_index;
            
            

        }
        else
        {
           
            //printf("The index of the hole is %d\n" , index_of_hole);
            
            

            LinkedList[current_index].previous=previous_index;
            //printf("Set LinkedList[%d].previous_index to %d\n" , current_index , previous_index);

            LinkedList[previous_index].next=current_index;
            //printf("Set LinkedList[%d].next to %d\n" , previous_index , current_index);

            LinkedList[current_index].next=index_of_hole;
        
            
            
            
            // LinkedList[current_index].previous_index=LinkedList[previous_index].previous_index;
            // //printf("Added Linkedlist[%d].previous_index is Linkedlist[%d]\n" , current_index , LinkedList[previous_index].previous_index);
            // LinkedList[current_index].next=previous_index;
            // LinkedList[previous_index].next=index_of_hole;
            // LinkedList[previous_index].previous_index=current_index;
            //Now change the values of the hole.
            
            LinkedList[index_of_hole].type=HOLE;
            // LinkedList[index_of_hole].next=;
            LinkedList[index_of_hole].previous=current_index;
            //set arena
            LinkedList[index_of_hole].size=LinkedList[index_of_hole].size-size;
            //printf("I am returning the index %d where i added %d and the remaining was a hole\n" , current_index , size );
            return current_index;

        }

        
        

    }
    else 
    {
         //if you are adding to an hole at the front 
        // HOLE  and adding node B
        // B----HOLE
        // if (LinkedList[previous_index].type==HOLE)
        // {

        // }

        
        //printf("I am adding to the front of the head , but there are processes behind it.\n");
        //printf("The current_index index is %d , the previous_index index is %d \n" , current_index , previous_index);

        //need to check if it is a perfect fit.

        

        if (LinkedList[rootNode].size==size)
        {
            //printf("We have a perfect fit at the front of the head\n");
            LinkedList[rootNode].type=PROCESS;
            //printf("i am adding to front of the head but processes behind and it is a perferct fit. I returned %d\n" , rootNode);
            return rootNode;
        }
        else
        {
            //Create a new node to enter the value
            LinkedList[current_index].next=rootNode;
            LinkedList[rootNode].previous=current_index;
            LinkedList[current_index].previous=-1;

            //make linkedlist[rootnode] a hole.//Only come here when the hole size is bigger then requested size
            LinkedList[rootNode].size=LinkedList[rootNode].size-size;
            LinkedList[rootNode].type=HOLE;
            rootNode=current_index;
            
            //printf("i am adding to front of the head but processes behind and created a hole. I returned %d\n" , rootNode);
            return rootNode;

        }
        
       
        

        

    }

    return 0;
}

/*Implementation_of_four_fits*/




int insertNode_WorstFit(size_t size)
{
    int index=findfreeNodeInternal(size);

    current = rootNode;
    ////printf("The current root is at index %d\n" , current);
    int previous = -1;
    int ret=-1;
    
    int largest_hole = 0;
    int largest_hole_index=current;

    //we need to insert at index.
    //need to iterate until we find index?
    //need to find where to insert it to end of

    //need to iterate until you find a hole which is bigger than what you need and add it there.
    //check if hole at the head , if then insert from the head.

    if(LinkedList[current].type== HOLE && LinkedList[current].size>=size)
    {
        ////printf("There is a free node at the head.\n");
        previous=-1 ;
        if (LinkedList[current].size>largest_hole)
        {
            largest_hole=LinkedList[current].size;
            largest_hole_index=0;
            
        }
        
    }

    
    
    int count=0;
    while( current>=0 && LinkedList[current].in_use && count<5)
    {
        ////printf("Current node at index %d is a %s and has size %zu\n" , current , enum_string[LinkedList[current].type] , LinkedList[current].size);
        // if(LinkedList[current].size>size && LinkedList[current].type==HOLE)
        if(LinkedList[LinkedList[current].next].size>=size && LinkedList[LinkedList[current].next].type==HOLE && LinkedList[LinkedList[current].next].size > largest_hole )
        {
            largest_hole= LinkedList[LinkedList[current].next].size;
            largest_hole_index=LinkedList[current].next;

            ////printf("\nThe hole at index %d is of size %d\n" , largest_hole_index , largest_hole);
             
            
            previous=current;
            
        

        }
        count++;
        // if (x==10)
        // {
        //     break;
        // }
        current=LinkedList[current].next;
        
        
    }
    
    ////printf("\nThe best place to insert a process of size %zu is at hole [%d] of size %d\n" ,size , largest_hole_index , largest_hole  );

    

    

    //at this point the previous is in_use. 
    ////printf("The previous value %d  is currently filled with a process and the next one is either not in use or a hole . \n" , previous );
    if (previous>=0)
    {
        //  //printf(previous);

    }

    if (previous>=-1)
    {
        ret = insertNodeInternal(previous , index , size );
        //we insert a not between previoud and index which is always a process.
    }
    else if (current==-1)
    {
        // We ran of the end of the list. 
        // If max of five then current = E.next which is -1 , previous = E
        //aka A-B-C-D-E. Insert between E.prev aka D and the index
        ret = insertNodeInternal(LinkedList[previous].previous , index , size );


    }

    //After our node is linked , we need to change some node parameters. 
    LinkedList[index].size=size;
    LinkedList[index].in_use=1;
    LinkedList[index].type=PROCESS;

    
    //Updating arena
    //index contains our current inserted node. 
    //index starts from the nex node's pointer.
    //the previous pointer 
    int next = LinkedList[index].next;
    LinkedList[index].arena = LinkedList[next].arena;
    LinkedList[next].arena = LinkedList[index].arena+ LinkedList[index].size;


    // ////printf("\nTWO NEW NODES THAT WERE CREATED ARE:\n");
    
    // //printf(index);
    // //printf(LinkedList[index].next);

    


   
    
    return ret;


}



/*End of four fits*/

int mavalloc_init( size_t size, enum ALGORITHM algorithm )
{
  size_t aligned_size = ALIGN4(size);
  void* arena = malloc(aligned_size);

  if(aligned_alloc<0)
  {
    return -1;
  }

  for(int i=0; i<MAX_LINKED_LIST_SIZE;i++)
  {
    LinkedList[i].size=0;
    LinkedList[i].in_use=0;
    LinkedList[i].arena=0;
    LinkedList[i].type=HOLE;
  }

  LinkedList[0].arena= arena;
  LinkedList[0].previous=-1;
  LinkedList[0].next=-1;
  LinkedList[0].in_use=1;
  LinkedList[0].size = aligned_size;
  LinkedList[0].type = HOLE;
  initialized = 1;

  //printf("-----------------------------------------------------\n");
  //printf("Initialized head\n");
  // printNode(0);
  //printf("-----------------------------------------------------\n");

  // Setting global variables.
  global_algorithm=algorithm;
   
    

  



  return 0;
}

void mavalloc_destroy( )
{
  memset(LinkedList , 0 , sizeof(LinkedList));
  
  return;
}

void * mavalloc_alloc( size_t size )
{

  int index = findfreeNodeInternal(size);
  LinkedList[index] = malloc(sizeof(struct* Node));

}

void mavalloc_free( void * ptr )
{

  free(ptr);

  


  return;
}

int main()
{
    
//   clock_t start , end;

//   double execution_time;

//   start=clock();
  

  
//   mavalloc_init( 75000000, WORST_FIT );

//   unsigned char* array[10000];

//   for (int i=0; i<10000;i++)
//   {
//     array[i]=mavalloc_alloc(100+i);
//   }

//   // //printfList();

//   for (int i=0; i<10000; i++)
//   {
//     if (i % 2==0 && i%3==0)
//     {
//       mavalloc_free(array[i]);
//     }
//   }



//   char *ptr_1 = mavalloc_alloc(2000);
//   char *ptr_2 = mavalloc_alloc(5000);
//   mavalloc_free(ptr_2);
//   char *ptr3=mavalloc_alloc(20);


//   mavalloc_free(ptr_1);

//   for (int i =5000 ; i<1000;i++)
//   {
//     if (i%5==0)
//     {
//       mavalloc_free(array[i]);
//     }
//   }

//   unsigned char* second_array [400];

//   for (int i=200; i<600;i++)
//   {

//     second_array[i]= mavalloc_alloc(i);

//   }

//   mavalloc_free(ptr3);


//   for (int j=0; j<300;j++)
//   {
//     mavalloc_alloc(j*230);
//   }

//   mavalloc_free(ptr_1);

//   for (int k=360; k<500; k++)
//   {
//     mavalloc_alloc(k*3);
//   }
//   end=clock();

//   execution_time=((double) (end-start))/CLOCKS_PER_SEC;
//   printf("Execution time for benchmark1.c WORST_FIT : %f\n" , execution_time);

  

 




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