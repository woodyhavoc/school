#include "Node.h"

NodePtr createNode(void * data)
{
    NodePtr newNode = (NodePtr) malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->data = data;
    return newNode;
}

void freeNode(NodePtr node)
{
    if(node == NULL)
	{
		return;
	}
    free(node);
}
