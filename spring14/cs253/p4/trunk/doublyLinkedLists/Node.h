#ifndef __NODE_H
#define __NODE_H

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

typedef struct node Node;
typedef struct node *NodePtr;

struct node
{
    void * data;
    NodePtr next;
    NodePtr prev;
};

/**
 * Accepts a Job Pointer as a parameter,
 * creates a new Node Pointer, and assigns
 * the Job Pointer as the data.
 *
 * @param data, the job pointer assigned to the node pointer
 *
 * @return a newly created node pointer
 */
NodePtr createNode(void * data);

/**
 * Frees the memory allocated to the node pointer passed
 * as a parameter.
 *
 * @param node, the node pointer whose allocated memory will be freed
 */
void freeNode(NodePtr node);


#endif /* __NODE_H */
