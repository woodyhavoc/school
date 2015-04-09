#include <stdio.h>
#include <stdlib.h>
#include "List.h"

ListPtr createList(int(*compareTo)(const void *, const void *),
				   char*(*toString)(const void *),
				   void (*freeObject)(const void *))
{
	ListPtr list;
	list = (ListPtr) malloc(sizeof(List));
	list->size = 0;
	list->head = NULL;
	list->compareTo = compareTo;
	list->toString = toString;
	list->freeObject = freeObject;
	return list;
}

void freeList(const ListPtr list)
{
	if(isEmpty(list) && list != NULL)
	{	
		free(list);
		return;
	}

	if(list == NULL)
	{
		return;
	}

	if(getSize(list) == 1)
	{
		freeNode(list->head, list->freeObject);
		free(list);
		return;
	}

	NodePtr frNode, temp = list->head;

	while(temp->next != list->head)
	{
		frNode = temp;
		temp = temp->next;
		freeNode(frNode, list->freeObject);
	}
	list->head = NULL;
	list->size = 0;
	freeNode(temp, list->freeObject);
    free(list);
}

int getSize(const ListPtr list)
{
    return list == NULL ? 0 : list->size;
}

Boolean isEmpty(const ListPtr list)
{
    return getSize(list) == 0 ? TRUE : FALSE;
}

void addAtFront(ListPtr list, NodePtr node)
{
	if(list == NULL || node == NULL)
	{
		return;
	}

	if(isEmpty(list) && node != NULL)
	{
		list->head = node;
		node->next = list->head;
		node->prev = list->head;
		list->size++;
		return;
	}
	
	node->next = list->head;
	node->prev = node->next->prev;
	node->next->prev = node;
	node->prev->next = node;
	list->head = node;
	list->size++;
}

void addAtRear(ListPtr list, NodePtr node)
{
	if(list == NULL || node == NULL)
	{
		return;
	}

	if(isEmpty(list) && list != NULL)
	{
		node->next = node;
		node->prev = node;
		list->head = node;
		list->size++;
		return;
	}
	
	NodePtr tail = list->head->prev;
	
	tail->next = node;
	list->head->prev = node;
	node->next = list->head;
	node->prev = tail;
	list->size++;
}

NodePtr removeFront(ListPtr list)
{
	if(isEmpty(list))
	{
		return NULL;
	}

	if(getSize(list) == 1)
	{
		NodePtr front = list->head;
		front->next = NULL;
		front->prev = NULL;
		list->head = NULL;
		list->size--;
		return front;
	}

	NodePtr front = list->head;
	front->prev->next = front->next;
	front->next->prev = front->prev;
	list->head = front->next;
	front->next = NULL;
	front->prev = NULL;
	list->size--;
	return front;
}

NodePtr removeRear(ListPtr list)
{
	if(isEmpty(list))
	{
		return NULL;
	}

	if(getSize(list) == 1)
	{
		NodePtr rear = list->head;
		rear->next = NULL;
		rear->prev = NULL;
		list->head = NULL;
		list->size--;
		return rear;
	}

	NodePtr rear = list->head->prev;
	rear->prev->next = rear->next;
	rear->next->prev = rear->prev;
    list->head->prev = rear->prev;
	rear->next = NULL;
	rear->prev = NULL;
	list->size--;
	return rear;
}

NodePtr removeNode(ListPtr list, NodePtr node)
{
	if(isEmpty(list) || node == NULL)
	{
		return NULL;
	}

	if(getSize(list) == 1)
	{
		if(list->head == node)
		{
			list->head->next = NULL;
			list->head->prev = NULL;
			list->head = NULL;
			list->size--;
			return node;
		}
		else
		{
			return NULL;
		}
	}
    
    NodePtr rmNode = list->head;

	while(rmNode != node && rmNode->next != list->head)
	{
		rmNode = rmNode->next;
	}

	if(rmNode != node)
	{
		return NULL;
	}
	else
	{
		rmNode->next->prev = rmNode->prev;
		rmNode->prev->next = rmNode->next;
        
        if(list->head == node)
        {
            list->head = rmNode->next;
        }

		rmNode->next = NULL;
		rmNode->prev = NULL;
		list->size--;
		return rmNode;
	}
}

NodePtr search(const ListPtr list, const void *obj)
{
	if(isEmpty(list))
	{
		return NULL;
	}

	if(getSize(list) == 1)
	{
		if(list->compareTo(list->head->obj, obj))
		{
			return list->head;
		}
		else
		{
			return NULL;
		}
	}

    if(list->compareTo(list->head->obj, obj))
    {
        return list->head;
    }
		
	NodePtr found = list->head;

	while(found->next != list->head)
	{
		if(list->compareTo(list->head->obj, obj))
		{
			 return found;
		}
	
		found = found->next;
	}

    return NULL;
}

void reverseList(ListPtr list)
{
	if(isEmpty(list))
	{
		return;
	}
	
	NodePtr swap, current, tail;
	
	swap = list->head;
	tail = swap->prev;
	current = swap->next;

	while(swap != tail)
	{
		swap->next = swap->prev;
		swap->prev = current;
		swap = current;
		current = swap->next;
	}
	
	swap->next = swap->prev;
	swap->prev = list->head;

	list->head = swap;
}

void printList(const ListPtr list)
{
	if(isEmpty(list))
	{
		return;
	}

	NodePtr printNode = list->head;
	int i;
	
	for(i = 0; i < getSize(list); i++)
	{
		char * string = list->toString(printNode->obj);
		printf("%s\n", string);
		free(string);
		printNode = printNode->next;
	}

	printf("\n");
}
