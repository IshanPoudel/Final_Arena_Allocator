#include "mavalloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINKED_LIST_SIZE 65534

// Declare global variables.
int rootNode;
int previous;
int current;
void *arena;

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

    enum  TYPE type;

    void* arena;

};

enum ALGORITHM global_algorithm;

static struct Node LinkedList[MAX_LINKED_LIST_SIZE];

char* const enum_string=["PROCESS" , "HOLE"];

int main()
{
    return 0;
}