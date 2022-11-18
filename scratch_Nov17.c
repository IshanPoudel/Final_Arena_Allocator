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



static struct Node LinkedList[MAX_LINKED_LIST_SIZE];

void printNode(int index)
{
    printf("\n");
    printf("LinkedList[%d] is a %d of size %zu\n" , index , LinkedList[index].type , LinkedList[index].size);
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

  int index=rootNode;

  for (int i=0; i<MAX_LINKED_LIST_SIZE;i++)
  {
    if (ptr==LinkedList[i].arena)
    {
        LinkedList[i].type==HOLE;
        return;
    }
  }

  //check if adjacent nodes arefree
  //if they are then combine them

  for (i=0; i<MAX_LINKED_LIST_SIZE-1;i++)
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
    return;
}

void * mavalloc_alloc( size_t size )
{
    void *ptr=NULL;

    if(global_algorithm==FIRST_FIT)
    {

        //start at the beginning of the list
        int i;

        for (i=0; i<MAX_LINKED_LIST_SIZE;i++)
        {
             //if the node type is hole and in_use and size<node size
             if (LinkedList[i].type==HOLE && LinkedList[i].in_use && LinkedList[i].size>=size)
             {
                //We found one.
                //check if perfect fit. 
                if (LinkedList[i].size==size)
                {
                    LinkedList[i].type == PROCESS;
                    return LinkedList[i].arena;
                }
                //else make a hole.
                {
                    //split it off into a new node. 
                    //first find a node that is not in use. 

                }
             }
             //if there is leftover size then insert a new node as a hole that holds that leftover space.
             //then return that node arena pointer

        }

        

        return ptr;

    }

    else if (global_algorithm==NEXT_FIT)
    {
        return ptr;
    }
    else if (global_algorithm==BEST_FIT)
    {
        return ptr;

    }
    else if (global_algorithm==WORST_FIT)
    {
        return ptr;

    }
    return ptr;
}