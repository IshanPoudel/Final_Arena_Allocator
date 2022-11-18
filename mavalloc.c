#include <stdio.h>
#include "mavalloc.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LINKED_LIST_SIZE 65534

int initialized=0;
int rootNode=0;
void *arena;
enum ALGORITHM global_algorithm;
int current=-99;
int previous=-99;



enum TYPE
{
    PROCESS , HOLE
};

struct Node
{
    int in_use;
    int previous;
    int next;

    size_t size;
    enum TYPE type;

    void *arena;
};

char * str_for_enum[] = {"PROCESS" , "HOLE"};



static struct Node LinkedList[MAX_LINKED_LIST_SIZE];

void printNode(int index)
{
    printf("\n");
    printf("LinkedList[%d] is a %s of size %zu\n" , index , str_for_enum[LinkedList[index].type] , LinkedList[index].size);
    printf("LinkedList[%d] ------- LinkedList[%d] -------- LinkedList[%d]\n" , LinkedList[index].previous , index , LinkedList[index].next);
    printf("LinkedList[%d] is located at %p\n" , index , LinkedList[index].arena );
    printf("\n");

}

void printList()
{
    int index=rootNode;
    while((index !=-1) & (LinkedList[index].in_use))
    {
        printNode(index);
        index=LinkedList[index].next;

    }
}

int findFreeInternalNode()
{
    //Find a node that is not in use. 
    //We do not need ot check for hole , because we will place the hole as it is. 

    for(int i=0; i<MAX_LINKED_LIST_SIZE;i++)
    {
        if (LinkedList[i].in_use==0)
        {
            return i;
        }
    }

    return -1;

}

int mavalloc_init(size_t size , enum ALGORITHM algorithm)
{
    size_t aligned_size = ALIGN4(size);
    //allocate pool and save the algorithm
    arena = malloc(aligned_size);
    global_algorithm=algorithm;


    //Initialize the linkedl sit.

    for (int i=0; i<MAX_LINKED_LIST_SIZE;i++)
    {
        LinkedList[i].size=0;
        LinkedList[i].in_use=0;
        LinkedList[i].arena=0;
        LinkedList[i].type=HOLE;

    }
    //Initialized the root node and point to the arena
    if(initialized==0)
    {
        LinkedList[0].arena= arena;
        LinkedList[0].previous=-1;
        LinkedList[0].next=-1;
        LinkedList[0].in_use=1;
        LinkedList[0].size = aligned_size;
        LinkedList[0].type = HOLE;
        initialized = 1;
    }

    return 1;




}

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

void mavalloc_free( void * ptr )
{

  

  for (int i=0; i<MAX_LINKED_LIST_SIZE;i++)
  {
    

    if (ptr==LinkedList[i].arena)
    {
        // printf("Found %p equal to %p\n" , ptr , LinkedList[i].arena);
        LinkedList[i].type=HOLE;
        current = LinkedList[i].previous;
        // printf("Changed LinkedList[%d] at %p to a %s\n" , i , LinkedList[i].arena , str_for_enum[LinkedList[i].type]);
        return;
    }
  }

  //Need to check if I am freeing the head.

  //check if adjacent nodes arefree
  //if they are then combine them

  for (int i=0; i<MAX_LINKED_LIST_SIZE-1;i++)
  {
    if (LinkedList[i].type==HOLE && LinkedList[i+1].type ==HOLE)
    {
        //combine them to a size LinkedList[i]
        //remove LinkedList[i+1].
        
    }
  }

  return;
}

void mavalloc_destroy()
{
    for (int i=0; i<MAX_LINKED_LIST_SIZE;i++)
    {
        LinkedList[i].size=0;
        LinkedList[i].in_use=0;
        LinkedList[i].arena=0;
        LinkedList[i].type=HOLE;
    }
    free(arena);
    arena=NULL;
    initialized=0;
    rootNode=0;
    current=-99;
    previous=-99;
    return;
}

int insertNode(int previous_index , int current_index , size_t size)
{
    /* Insert a process at index and link it up with the previous node.
    previous_index->next contains the required hole.
    Make sure that the hole size matches.
    Returns the index of our new process*/

    //Initialize the previous and next of the current_index to be -1.

    LinkedList[current_index].next=-1;
    LinkedList[current_index].previous=-1;

    
    //If nothing is initialized.
        //Check is perfect fit

        //If not perfect fit , add to the head and have a hole there as well.
    
    //If adding at any place beside the rootNode.
        // Check if perfect fit.

        //If not perfect fit , add to previous->next and have a hole behind it.

    //If adding to the hole and there are processes behind it. 
        //Check if perfect fit 
        //If not perfect fit , have a hole behind it.

    //If nothing is initialized

    if (LinkedList[rootNode].previous==-1 && LinkedList[rootNode].next==-1)
    {
        printf("I am adding when nothing is initialized\n");
        //Check if perfect fit. 
        

        if(LinkedList[rootNode].size == size)
        {
            LinkedList[rootNode].type=PROCESS;

            current_index=rootNode;

            //if perfect fit , no need to return arena as it already points to what is required.
            printf("I should be returning %p\n" , LinkedList[rootNode].arena);
            return rootNode;
            
        }

        //Not a perfect fit. 
        //The rootnode is a hole 
        // printf("The hole is bigger than the process\n");
        // printf("The size of the rootnode before doing anything %zu\n" , LinkedList[rootNode].size);

        LinkedList[current_index].next = rootNode;
        LinkedList[rootNode].previous = current_index;
        LinkedList[current_index].previous=-1; //aka make current_index the root.

        //Make Linkedlist[rootnode] a hole of remaining size .
        LinkedList[rootNode].size=LinkedList[rootNode].size-size ;
        LinkedList[rootNode].type=HOLE;
        LinkedList[rootNode].next=-1; //As this is when nothing is initialized so everything should be a hole.

        //Change the size of the holes.
        LinkedList[current_index].size=size;
        // LinkedList[rootNode].size = LinkedList[rootNode].size-size;
        printf("%zu\n" , LinkedList[rootNode].size);


        //At this point we need to change the address of our hole as we have B--HOLE and we need to update the address of the hole
        LinkedList[current_index].arena=LinkedList[rootNode].arena; //Starts from 50000
        LinkedList[rootNode].arena = LinkedList[current_index].arena + LinkedList[current_index].size; //Starts from 50000+100

        
        rootNode = current_index;
        
        //Add pointers for next fit
        previous=-1;
        current=rootNode;

        printf("I should be returning %p\n" , LinkedList[rootNode].arena);
        
        return rootNode;

    } 

    //Adding to any other place besides the rootNode. //A-B-C-Hole
    else if (previous_index>=0)
    {
        printf("i am adding anytime before next fit\n");
        printf("Previous: %d \n" , previous);
        //The index next to previous_index contains a hole . 
        int index_of_hole = LinkedList[previous_index].next;
        printf("Index of hole: %d\n" , index_of_hole);
        printf("Current index is %d\n" , current_index);

        //Check if perfect fit.

        if(LinkedList[index_of_hole].size==size)
        {
            LinkedList[current_index].type = PROCESS;
            LinkedList[current_index].previous = previous_index;
            LinkedList[previous_index].next = current_index;

            //Make the index of hole not in use.
            LinkedList[index_of_hole].in_use=0;
            LinkedList[index_of_hole].next=-1;
            LinkedList[index_of_hole].previous=-1;

            //At this point , for the current_index , we need to add from the previous index.
            // Proceess - NewProcess - Hole. 
            
            LinkedList[current_index].size = size;
            LinkedList[current_index].arena = LinkedList[previous_index].arena+LinkedList[previous_index].size;

            // current=current_index;
            // previous=LinkedList[current_index].previous;
            current=current_index;

            printf("I should be returning %p\n" , LinkedList[current_index].arena);

            return current_index;

        }

        //Not a perfect fit.
        LinkedList[current_index].type=PROCESS;

        LinkedList[current_index].next=index_of_hole;
        LinkedList[current_index].previous=previous_index;

        LinkedList[previous_index].next=current_index;

        LinkedList[index_of_hole].previous=current_index;

        //Set type of ListofHole.
        LinkedList[index_of_hole].type=HOLE;

        //Set arena for current_index
        LinkedList[current_index].size= size;
        LinkedList[current_index].arena = LinkedList[previous_index].arena+LinkedList[previous_index].size;

        //Set size and arena for the hole.
        LinkedList[index_of_hole].size = LinkedList[index_of_hole].size-size;
        LinkedList[index_of_hole].arena = LinkedList[current_index].arena + LinkedList[current_index].size;
        
        //Change pointer to current.
        current=current_index;
        printf("Added LinkedList[%d] \n" , current);
        printNode(current);

        printf("I should be returning %p\n" , LinkedList[current_index].arena);

        

        return current_index;


    }

    else
    {
        // We are adding to the front of the hole , but there are processes behind them
        printf("I am adding to the front of the hole , but there are processes behind them\n");
        printf("Previous: %d \n" , previous_index);

        //Check if perfect fit. 
        if(LinkedList[rootNode].size==size)
        {
            LinkedList[rootNode].type=PROCESS;

            current_index=rootNode;

            printf("I should be returning %p\n" , LinkedList[rootNode].arena);
            return rootNode;
        }

        //Not a perfect fit
        LinkedList[current_index].next=rootNode;
        LinkedList[rootNode].previous=current_index;
        LinkedList[current_index].previous=-1;

        //Make linkedlist a hole . 
        //Only come here when the hole size is bigger.

       
        LinkedList[rootNode].type =HOLE;

        //Set size and arena of the process.
        LinkedList[current_index].size= size;
        LinkedList[current_index].arena = LinkedList[rootNode].arena+LinkedList[rootNode].size;

        //Set size and arena of the hole.
        LinkedList[rootNode].size=LinkedList[rootNode].size-size;
        LinkedList[rootNode].arena = LinkedList[current_index].arena + LinkedList[current_index].size;

        rootNode=current_index;

        //If added to the front of the hole.
        previous=-1;
        current=rootNode;
        printf("I changed current to rootNode %d\n" , current);

        printf("I should be returning %p\n" , LinkedList[rootNode].arena);

        return rootNode;

    }



    return 0;
}


int insertNode_FirstFit(size_t size)
{
    /* Given the size returns the index of the node where it is inserted. 
    If not inserted , it returns -1.
    */

   //First find a free internal node. 

   

   //if our hole is bigger than the process , we need to find a freeInternal Node to insert the process into. 

   //Start from the beginning of the list. 
   // If we find a node which is a hole , we nned to insert next to it.

   int current = rootNode;
   int previous = -1;
   int ret = -1;

   int holeFound = 0;
   
   //if we find a hole at the very beginning of our list.
   if (LinkedList[current].type==HOLE && LinkedList[current].size>=size)
   {
    holeFound=1;
    previous=-1;

   }
   else
   {
    //We run a loop until we find a hole big enough for our size.

    //Have a flag if you never find a hole that is required.


    //We start and check if node is in use.
     while(current>=0 && LinkedList[current].in_use)
     {
        //Need to check if the curret node's next node is a hole and is of suffecient size.
        int next_hole = LinkedList[current].next;

        if(LinkedList[next_hole].size>=size && LinkedList[next_hole].type==HOLE)
        {
            //At this point our previous hole points to a process and the next node is a hole where we insert our process.

            //Have a flag for the biggest_hole. 
            holeFound=1;
            previous=current;
            break;

        }
        current=LinkedList[current].next;

     }
   }

   //need to have a case where there is no free hole of required size. 
   //In that case current and previous remain the same. 
   if(holeFound==0)
   {
    printf("There is no required size\n");
    return -1;
   }


   //At this point previous is in use. 
   //Two cases :    A-B-C-D-hole-e-f-g-hole , previous points to D and we insert our process there.
   // Hole-a-b-c-d  , we insert our process in the head , which makes previous =-1;

   int index = findFreeInternalNode();

   //We store our node at index.

   if (previous>=-1)
   {

    //We insert node at the previous place at the index.
    //retunr points to the address of our new node.
    //aks the address of previous->next;
    //return value returns the current place of our process , it should be similar to index but there can be differences.
    ret = insertNode(previous , index , size);
    

   }

   else if (current == -1)
   {

       // If max of five then current = E.next which is -1 , previous = E
        //aka A-B-C-D-E. Insert between E.prev aka D and the index

    ret = insertNode(LinkedList[previous].previous , index , size);

   }

   //Here insertNode makes space for our internal nodes .
   //Now we initialize the internalNode with required value. 

   LinkedList[ret].size=size;
   LinkedList[ret].in_use=1;
   LinkedList[ret].type=PROCESS;

   


   return ret;



}

int insertNode_NextFit(size_t size)
{
    int index=findFreeInternalNode();

    printf("i am here when adding size %zu . The current and previous are %d and %d\n" , size ,current , previous);
    

    if (current==-99 & previous==-99)
    {
        //nothing initialized yet
        ////printf("I initialized current and previous once\n");
        current=rootNode;
        previous=-1;
    }

    int local_previous = previous;


    
    
   


    

    
    ////printf("The current root is at index %d\n" , current);
    ////printf("The previous is at index %d\n " , previous);
    
    int ret=-1;
    
    //we need to insert at index.
    //need to iterate until we find index?
    //need to find where to insert it to end of

    //need to iterate until you find a hole which is bigger than what you need and add it there.
    //check if hole at the head , if then insert from the head.
    //if we are starting from the beginning.
    if(LinkedList[rootNode].type== HOLE && LinkedList[rootNode].size>=size)
    {
        ////printf("There is a free node at the head.\n");
        previous=-1 ;
        
    }
    
    

    while( current>=0 && LinkedList[current].in_use )
    {
        ////printf("Current node at index %d is a %s and has size %zu\n" , current , enum_string[LinkedList[current].type] , LinkedList[current].size);
        // if(LinkedList[current].size>size && LinkedList[current].type==HOLE)
        if(LinkedList[LinkedList[current].next].size>=size && LinkedList[LinkedList[current].next].type==HOLE)
        {
            
            printf("MATCH: Current node at index %d is a %s and has size %zu\n" , current ,str_for_enum[LinkedList[current].type] , LinkedList[current].size);
            printf("MATCH: Next index is a hole\n");
            previous=current;
            break;
        

        }
        current=LinkedList[current].next;

        // if (current==local_previous)
        // {
        //     printf("All the list traversed.\n");
        //     return -1;
        //     break;
        // }

        // if (current==-1)
        // {
        //     current=rootNode;
        // }
        
        
    }

    

    

    //at this point the previous is in_use. 
    ////printf("The previous value %d  is currently filled with a process and the next one is either not in use or a hole . \n" , previous );
    if (previous>=0)
    {
         printNode(previous);

    }

    if (previous>=-1)
    {
        ret = insertNode(previous , index , size );
        //we insert a not between previoud and index which is always a process.
    }
    else if (current==-1)
    {
        // We ran of the end of the list. 
        // If max of five then current = E.next which is -1 , previous = E
        //aka A-B-C-D-E. Insert between E.prev aka D and the index
        ret = insertNode(LinkedList[previous].previous , index , size );


    }

    //After our node is linked , we need to change some node parameters. 
    LinkedList[index].size=size;
    LinkedList[index].in_use=1;
    LinkedList[index].type=PROCESS;

    
    //Updating arena
    //index contains our current inserted node. 
    //index starts from the nex node's pointer.
    //the previous pointer 
    // int next = LinkedList[index].next;
    // LinkedList[index].arena = LinkedList[next].arena;
    // LinkedList[next].arena = LinkedList[index].arena+ LinkedList[index].size;


    // ////printf("\nTWO NEW NODES THAT WERE CREATED ARE:\n");
    
    // printNode(index);
    // printNode(LinkedList[index].next);

    


   
    
    return ret;


}

int insertNode_BestFit(size_t size)
{
    /* Given the size returns the index of the node where it is inserted. 
    If not inserted , it returns -1.
    */

   //First find a free internal node. 

   

   //if our hole is bigger than the process , we need to find a freeInternal Node to insert the process into. 

   //Start from the beginning of the list. 
   // If we find a node which is a hole , we nned to insert next to it.

   int current = rootNode;
   int previous = -1;
   int ret = -1;

   int holeFound = 0;

   int smallest_hole = INT32_MAX;
   int smallest_hole_index = current;
   
   //if we find a hole at the very beginning of our list.
   if (LinkedList[current].type==HOLE && LinkedList[current].size>=size)
   {
    holeFound=1;
    previous=-1;
    smallest_hole=LinkedList[current].size;
    smallest_hole_index=current;

   }

   
    //We run a loop until we find a hole big enough for our size.

    //Have a flag if you never find a hole that is required.


    //We start and check if node is in use.
    while(current>=0 && LinkedList[current].in_use)
    {
    //Need to check if the curret node's next node is a hole and is of suffecient size.
    int next_hole = LinkedList[current].next;

    if(LinkedList[next_hole].size>=size && LinkedList[next_hole].type==HOLE && LinkedList[next_hole].size < smallest_hole)
    {
        //At this point our previous hole points to a process and the next node is a hole where we insert our process.
        smallest_hole = LinkedList[next_hole].size;
        smallest_hole_index=next_hole;

        //Have a flag for the biggest_hole. 
        holeFound=1;
        previous=current;
        break;

    }
    current=LinkedList[current].next;

    }
   

   //need to have a case where there is no free hole of required size. 
   //In that case current and previous remain the same. 
   if(holeFound==0)
   {
    printf("There is no required size\n");
    return -1;
   }


   //At this point previous is in use. 
   //Two cases :    A-B-C-D-hole-e-f-g-hole , previous points to D and we insert our process there.
   // Hole-a-b-c-d  , we insert our process in the head , which makes previous =-1;

   int index = findFreeInternalNode();

   //We store our node at index.

   if (previous>=-1)
   {

    //We insert node at the previous place at the index.
    //retunr points to the address of our new node.
    //aks the address of previous->next;
    //return value returns the current place of our process , it should be similar to index but there can be differences.
    ret = insertNode(previous , index , size);
    

   }

   else if (current == -1)
   {

       // If max of five then current = E.next which is -1 , previous = E
        //aka A-B-C-D-E. Insert between E.prev aka D and the index

    ret = insertNode(LinkedList[previous].previous , index , size);

   }

   //Here insertNode makes space for our internal nodes .
   //Now we initialize the internalNode with required value. 

   LinkedList[ret].size=size;
   LinkedList[ret].in_use=1;
   LinkedList[ret].type=PROCESS;

   


   return ret;

}


int insertNode_WorstFit(size_t size)
{
    /* Given the size returns the index of the node where it is inserted. 
    If not inserted , it returns -1.
    */

   //First find a free internal node. 

   

   //if our hole is bigger than the process , we need to find a freeInternal Node to insert the process into. 

   //Start from the beginning of the list. 
   // If we find a node which is a hole , we nned to insert next to it.

   int current = rootNode;
   int previous = -1;
   int ret = -1;

   int holeFound = 0;

   int biggest_hole = 0;
   int biggest_hole_index = current;
   
   //if we find a hole at the very beginning of our list.
   if (LinkedList[current].type==HOLE && LinkedList[current].size>=size)
   {
    holeFound=1;
    previous=-1;
    biggest_hole=LinkedList[current].size;
    biggest_hole_index=current;

   }

   
    //We run a loop until we find a hole big enough for our size.

    //Have a flag if you never find a hole that is required.


    //We start and check if node is in use.
    while(current>=0 && LinkedList[current].in_use)
    {
    //Need to check if the curret node's next node is a hole and is of suffecient size.
    int next_hole = LinkedList[current].next;

    if(LinkedList[next_hole].size>=size && LinkedList[next_hole].type==HOLE && LinkedList[next_hole].size > biggest_hole)
    {
        //At this point our previous hole points to a process and the next node is a hole where we insert our process.
        biggest_hole = LinkedList[next_hole].size;
        biggest_hole_index=next_hole;

        //Have a flag for the biggest_hole. 
        holeFound=1;
        previous=current;
        break;

    }
    current=LinkedList[current].next;

    }
   

   //need to have a case where there is no free hole of required size. 
   //In that case current and previous remain the same. 
   if(holeFound==0)
   {
    printf("There is no required size\n");
    return -1;
   }


   //At this point previous is in use. 
   //Two cases :    A-B-C-D-hole-e-f-g-hole , previous points to D and we insert our process there.
   // Hole-a-b-c-d  , we insert our process in the head , which makes previous =-1;

   int index = findFreeInternalNode();

   //We store our node at index.

   if (previous>=-1)
   {

    //We insert node at the previous place at the index.
    //retunr points to the address of our new node.
    //aks the address of previous->next;
    //return value returns the current place of our process , it should be similar to index but there can be differences.
    ret = insertNode(previous , index , size);
    

   }

   else if (current == -1)
   {

       // If max of five then current = E.next which is -1 , previous = E
        //aka A-B-C-D-E. Insert between E.prev aka D and the index

    ret = insertNode(LinkedList[previous].previous , index , size);

   }

   //Here insertNode makes space for our internal nodes .
   //Now we initialize the internalNode with required value. 

   LinkedList[ret].size=size;
   LinkedList[ret].in_use=1;
   LinkedList[ret].type=PROCESS;

   


   return ret;

}

void * mavalloc_alloc( size_t size )
{
    //if inserting without initializing or after destroying. 
    void *ptr=NULL;

    if (initialized==0)
    {
        return NULL;
    }

    size = ALIGN4(size);

    if(global_algorithm==FIRST_FIT)
    {

        int index = insertNode_FirstFit(size);
        ptr =  LinkedList[index].arena;
        printf("The pointer  returned is %p \n" , ptr);

        
        return ptr;

    }

    else if (global_algorithm==NEXT_FIT)
    {

        int index = insertNode_NextFit(size);
        ptr =  LinkedList[index].arena;
        printf("The pointer  returned is %p \n" , ptr);

        
        return ptr;
    }
    else if (global_algorithm==BEST_FIT)
    {
        int index = insertNode_BestFit(size);
        ptr = LinkedList[index].arena;
        printf("The pointer returned is %p\n" , ptr);
        return ptr;

    }
    else if (global_algorithm==WORST_FIT)
    {
        int index = insertNode_WorstFit(size);
        ptr = LinkedList[index].arena;
        printf("The pointer returned is %p\n" , ptr);
        return ptr;

        

    }
    return ptr;
}


// int main()
// {
//     mavalloc_init( 71608, NEXT_FIT );
//     char * ptr1 = ( char * ) mavalloc_alloc ( 1024 );
//     //Cannot add 1024 at the first.
//     char * buf1 = ( char * ) mavalloc_alloc ( 4 );
//     char * ptr6 = ( char * ) mavalloc_alloc ( 16 );
//     char * buf2 = ( char * ) mavalloc_alloc ( 4 );
//     char * ptr2 = ( char * ) mavalloc_alloc ( 2048 );
//     char * buf3 = ( char * ) mavalloc_alloc ( 4 );
//     char * ptr7 = ( char * ) mavalloc_alloc ( 16 );
//     char * buf4 = ( char * ) mavalloc_alloc ( 4 );
//     char * ptr3 = ( char * ) mavalloc_alloc ( 1024 );

//     printf("PTR 3 IS %p\n" , ptr3);

//     mavalloc_free( ptr1 ); 
//     printf("FINAL: \n\n\n");
    
//     mavalloc_free( ptr2 ); 
//     mavalloc_free( ptr3 ); 

    

//     char * ptr5 = ( char * ) mavalloc_alloc ( 2000 );
//     char * ptr4 = ( char * ) mavalloc_alloc ( 1000 );
//     printList();

//     // once you free the memory , the previous and the current pointer should change to the previous one?


    
    
//     // char * ptr = ( char * ) mavalloc_alloc ( 1000 );

   


// }