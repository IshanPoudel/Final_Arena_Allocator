

#include "mavalloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINKED_LIST_SIZE 65534

static int initialized=0;
static int rootNode=0;

int current=-99;
int previous=-99;

enum TYPE 
{
    PROCESS , HOLE
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
    printf("\n\n");

    while( (index != -1) & (LinkedList[index].in_use))
    {
        printNode(index);
        index=LinkedList[index].next;
    }

}

int findfreeNodeInternal(size_t size)
{
    for(int i=0; i<MAX_LINKED_LIST_SIZE;i++)
    {
        if (LinkedList[i].in_use==0 )
        {
            
            printf("\n I found a free node at %d to insert process of size%zu\n" , i , size);
            printNode(i);
            
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
       

        printf("I am adding when everything is empty. Need to initialize with a hole\n");

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
       
        // printf("The next free is at %d\n" , next_free);

        printf("I am returning the index %d where i added %d when I added to an empty head\n" , rootNode , size );

        
        

    }
    else if (previous_index>=0)
    {

        
        
        
        printf("I am here when previous_index >= 0  . The value of previous_index is %d. \n\n" , previous_index);
        
        // A-Hole , Insert B
        
        // B-A-Hole
        //prev=A
        //INSERT C such that B--A----HOLE

        //  B--A--(500) C is also 500.
        
        int  index_of_hole = LinkedList[previous_index].next;

       
        if (LinkedList[index_of_hole].size==size)
        {
            printf("I found the perfect hole\n\n");
            current_index=index_of_hole;
            LinkedList[current_index].type=PROCESS;
            LinkedList[current_index].previous=previous_index;
            LinkedList[previous_index].next= current_index;
            printf("I am returning the index %d where i added %d when I added an in between in a perfect hole\n" , current_index , size );
            return current_index;
            
            

        }
        else
        {
           
            printf("The index of the hole is %d\n" , index_of_hole);
            
            

            LinkedList[current_index].previous=previous_index;
            printf("Set LinkedList[%d].previous_index to %d\n" , current_index , previous_index);

            LinkedList[previous_index].next=current_index;
            printf("Set LinkedList[%d].next to %d\n" , previous_index , current_index);

            LinkedList[current_index].next=index_of_hole;
        
            
            
            
            // LinkedList[current_index].previous_index=LinkedList[previous_index].previous_index;
            // printf("Added Linkedlist[%d].previous_index is Linkedlist[%d]\n" , current_index , LinkedList[previous_index].previous_index);
            // LinkedList[current_index].next=previous_index;
            // LinkedList[previous_index].next=index_of_hole;
            // LinkedList[previous_index].previous_index=current_index;
            //Now change the values of the hole.
            
            LinkedList[index_of_hole].type=HOLE;
            // LinkedList[index_of_hole].next=;
            LinkedList[index_of_hole].previous=current_index;
            //set arena
            LinkedList[index_of_hole].size=LinkedList[index_of_hole].size-size;
            printf("I am returning the index %d where i added %d and the remaining was a hole\n" , current_index , size );
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

        
        printf("I am adding to the front of the head , but there are processes behind it.\n");
        printf("The current_index index is %d , the previous_index index is %d \n" , current_index , previous_index);

        //need to check if it is a perfect fit.

        

        if (LinkedList[rootNode].size==size)
        {
            printf("We have a perfect fit at the front of the head\n");
            LinkedList[rootNode].type=PROCESS;
            printf("i am adding to front of the head but processes behind and it is a perferct fit. I returned %d\n" , rootNode);
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
            
            printf("i am adding to front of the head but processes behind and created a hole. I returned %d\n" , rootNode);
            return rootNode;

        }
        
       
        

        

    }

    return 0;
}

/*Implementation_of_four_fits*/




int insertNode_BestFit(size_t size)
{
    int index=findfreeNodeInternal(size);

    current = rootNode;
    printf("The current root is at index %d\n" , current);
    int previous = -1;
    int ret=-1;
    
    int smallest_hole = INT32_MAX;
    int smallest_hole_index = current;

    //we need to insert at index.
    //need to iterate until we find index?
    //need to find where to insert it to end of

    //need to iterate until you find a hole which is bigger than what you need and add it there.
    //check if hole at the head , if then insert from the head.

    if(LinkedList[current].type== HOLE && LinkedList[current].size>=size)
    {
        printf("There is a free node at the head.\n");
        previous=-1 ;
        if (LinkedList[current].size<smallest_hole)
        {
            smallest_hole=LinkedList[current].size;
            smallest_hole_index=0;
            
        }
        
    }

    
    
    while( current>=0 && LinkedList[current].in_use )
    {
        printf("Current node at index %d is a %s and has size %zu\n" , current , enum_string[LinkedList[current].type] , LinkedList[current].size);
        // if(LinkedList[current].size>size && LinkedList[current].type==HOLE)
        if(LinkedList[LinkedList[current].next].size>=size && LinkedList[LinkedList[current].next].type==HOLE && LinkedList[LinkedList[current].next].size < smallest_hole )
        {
            smallest_hole= LinkedList[LinkedList[current].next].size;
            smallest_hole_index=LinkedList[current].next;

            printf("\nThe hole at index %d is of size %d\n" , smallest_hole_index , smallest_hole);
             
            
            previous=current;
            
        

        }
        current=LinkedList[current].next;
        
        
    }

    printf("\nThe best place to insert a process of size %zu is at hole [%d] of size %d\n" ,size , smallest_hole_index , smallest_hole  );

    

    

    //at this point the previous is in_use. 
    printf("The previous value %d  is currently filled with a process and the next one is either not in use or a hole . \n" , previous );
    if (previous>=0)
    {
         printNode(previous);

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


    // printf("\nTWO NEW NODES THAT WERE CREATED ARE:\n");
    
    // printNode(index);
    // printNode(LinkedList[index].next);

    


   
    
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

  printf("-----------------------------------------------------\n");
  printf("Initialized head\n");
  printNode(0);
  printf("-----------------------------------------------------\n");

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

  size_t aligned_size = ALIGN4(size);
  int index;


  if(global_algorithm == FIRST_FIT)
  {
    // index = insertNode_FirstFit(aligned_size);
    return LinkedList[index].arena;
  }
  else if(global_algorithm == NEXT_FIT)
  {
    // index = insertNode_NextFit(aligned_size);
    return LinkedList[index].arena;
  }
  else if(global_algorithm == BEST_FIT)
  {
    index = insertNode_BestFit(aligned_size);
    // if (index==-1)
    // {
    //     return NULL;

    // } 
    return LinkedList[index].arena;
  }
  else if(global_algorithm == WORST_FIT)
  {
    // index = insertNode_WorstFit(aligned_size);
    return LinkedList[index].arena;
  }
  else
  {

  }

  // only return NULL on failure
  return NULL;
}

int removeNodeInternal(int node)
{
  if(node<0 || node>=MAX_LINKED_LIST_SIZE)
  {
    printf("EROROR:\n");

  }

  if(LinkedList[node].in_use==0)
  {
    printf("Cannot remove node. Not in use\n");
  }

  // check_if next is also a hole 
  LinkedList[node].type=HOLE;
  
  int next = LinkedList[node].next;

  //Have it happen a such that there are no hole.
  
  if (LinkedList[next].in_use && LinkedList[next].type==HOLE)
  {
    //Change the previous and the next 
    // A--B--C--HOLE---D
    // Trying to remove C. a-b-hole-d
    LinkedList[node].size = LinkedList[node].size + LinkedList[next].size;
    //Increase size of first hole where c was.

    LinkedList[node].next = LinkedList[next].next;
    //Link with the next hole D.

    LinkedList[next].previous = node;

    //Make the next hole available for use
    LinkedList[next].in_use=0;



  }

  return 0;

}

void mavalloc_free( void * ptr )
{

  int index=rootNode;

  while (index!=-1)
  {
    if (LinkedList[index].arena==ptr)
    {

      removeNodeInternal(index);
     
    //   printf("I removed node at index %d  whose pointer is supposed to be %p\n" , index , ptr);
    //   printf("%p\n" , LinkedList[index].arena);
    }
    index = LinkedList[index].next;
  }

  


  return;
}

int main()
{
    //Start with test cases for best_fit
    mavalloc_init( 65535, BEST_FIT );
    char * ptr = ( char * ) mavalloc_alloc ( 65535 );
    printf("I allocated the whole thing\n");
    printf("The return address %p\n" , ptr );
    printList();
    
}

//When process is equal to hole size.