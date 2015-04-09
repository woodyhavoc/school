#include <stdio.h>
#include <stdlib.h>
#include "List.h"

ListPtr createList(char *(*toString)(const void *),
		 void (*freeObject)(const void *),
		 int (*compareTo)(const void *, const void *))
{
	ListPtr list;
	list = (ListPtr) malloc(sizeof(List));
	list->size = 0;
	list->head = NULL;

	list->toString = toString;
	list->freeObject = freeObject;
	list->compareTo = compareTo;

	return list;
}

void freeList(ListPtr list)
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
		list->freeObject(list->head->data);
		freeNode(list->head);
		free(list);
		return;
	}

	NodePtr frNode, temp = list->head;

	while(temp->next != list->head)
	{
		frNode = temp;
		temp = temp->next;
		list->freeObject(frNode->data);
		freeNode(frNode);
	}
	list->head = NULL;
	list->size = 0;
	list->freeObject(temp->data);
	freeNode(temp);
	free(list);
}

int getSize(ListPtr list)
{
	return list == NULL ? 0 : list->size;
}

Boolean isEmpty(ListPtr list)
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
		list->head = node;
		list->head->next = node;
		list->head->prev = node;
		list->size++;
		return;
	}

	NodePtr head = list->head;
	NodePtr tail = list->head->prev;

	head->prev = node;
	tail->next = node;
	node->next = head;
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

NodePtr search(ListPtr list, const void * data)
{
	if(isEmpty(list) || data == NULL)
	{
		return NULL;
	}

	NodePtr found = list->head;

	int i, size = list->size;

	for(i = 0; i < size; i++)
	{
		if(list->compareTo(found->data, data) == 0)
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

void printList(ListPtr list)
{
	if(isEmpty(list) || list == NULL)
	{
		return;
	}

	NodePtr printNode = list->head;

	int i;

	for(i = 0; i < getSize(list); i++)
	{
		char * string = list->toString((const void *)printNode->data);
        printf("%s", string);
        free(string);
		printNode = printNode->next;
	}
}
