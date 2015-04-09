#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "List.h"

static void __addAtFront(ListPtr list, NodePtr node);
static void __addAtRear(ListPtr list, NodePtr node);
static NodePtr __removeFront(ListPtr list);
static NodePtr __removeRear(ListPtr list);
static NodePtr __removeNode(ListPtr list, NodePtr node);
static NodePtr __search(const ListPtr list, const void *obj);
static void __reverseList(ListPtr list);
static void __printList(const ListPtr list);

ListPtr createList(int(*compareTo)(const void *, const void *),
				   char*(*toString)(const void *),
				   void (*freeObject)(const void *),
				   int size)
{
	ListPtr list;
	list = (ListPtr) malloc(sizeof(List));
	list->size = 0;
	list->head = NULL;
	list->compareTo = compareTo;
	list->toString = toString;
	list->freeObject = freeObject;
	list->maxSize = size;
	pthread_mutex_init(&list->listMutex, NULL);
	pthread_cond_init(&list->listCondAdd, NULL);
	pthread_cond_init(&list->listCondRm, NULL);
	list->stop = FALSE;
	return list;
}

static void __freeList(const ListPtr list)
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
		list->size--;
	}
	list->head = NULL;
	list->size = 0;
	freeNode(temp, list->freeObject);
	free(list);
}

void freeList(const ListPtr list)
{
	pthread_mutex_lock(&list->listMutex);
	pthread_mutex_t tmpMutex = list->listMutex;
	pthread_cond_t tmpCondAdd = list->listCondAdd;
	pthread_cond_t tmpCondRm = list->listCondRm;

	__freeList(list);

	pthread_mutex_unlock(&tmpMutex);
	pthread_mutex_destroy(&tmpMutex);
	pthread_cond_destroy(&tmpCondAdd);
	pthread_cond_destroy(&tmpCondRm);
}

int getSize(const ListPtr list)
{
    return list == NULL ? 0 : list->size;
}

Boolean isEmpty(const ListPtr list)
{
    return getSize(list) == 0 ? TRUE : FALSE;
}

static void __addAtFront(ListPtr list, NodePtr node)
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

void addAtFront(ListPtr list, NodePtr node)
{
	pthread_mutex_lock(&list->listMutex);
	while(list->size == list->maxSize)
	{
		pthread_cond_wait(&list->listCondRm, &list->listMutex);
	}
	__addAtFront(list, node);
	pthread_mutex_unlock(&list->listMutex);
	pthread_cond_signal(&list->listCondAdd);
}

static void __addAtRear(ListPtr list, NodePtr node)
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

void addAtRear(ListPtr list, NodePtr node)
{
	pthread_mutex_lock(&list->listMutex);
	while(list->size == list->maxSize)
	{
		pthread_cond_wait(&list->listCondRm, &list->listMutex);
	}
	__addAtRear(list, node);
	pthread_mutex_unlock(&list->listMutex);
	pthread_cond_signal(&list->listCondAdd);
}

static NodePtr __removeFront(ListPtr list)
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

NodePtr removeFront(ListPtr list)
{
	pthread_mutex_lock(&list->listMutex);
	while(list->size == 0 && !list->stop)
	{
		pthread_cond_wait(&list->listCondAdd, &list->listMutex);
	}
	NodePtr rmFront = __removeFront(list);
	pthread_mutex_unlock(&list->listMutex);
	pthread_cond_signal(&list->listCondRm);
	return rmFront;
}

static NodePtr __removeRear(ListPtr list)
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

NodePtr removeRear(ListPtr list)
{
	pthread_mutex_lock(&list->listMutex);
	while(list->size == 0 && !list->stop)
	{
		pthread_cond_wait(&list->listCondAdd, &list->listMutex);
	}
	NodePtr rmRear = __removeRear(list);
	pthread_mutex_unlock(&list->listMutex);
	pthread_cond_signal(&list->listCondRm);
	return rmRear;
}

static NodePtr __removeNode(ListPtr list, NodePtr node)
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

NodePtr removeNode(ListPtr list, NodePtr node)
{
	pthread_mutex_lock(&list->listMutex);
	while(list->size == 0 && !list->stop)
	{
		pthread_cond_wait(&list->listCondAdd, &list->listMutex);
	}
	NodePtr rmNode = __removeNode(list, node);
	pthread_mutex_unlock(&list->listMutex);
	pthread_cond_signal(&list->listCondRm);
	return rmNode;
}

static NodePtr __search(const ListPtr list, const void *obj)
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

NodePtr search(const ListPtr list, const void *obj)
{
	pthread_mutex_lock(&list->listMutex);
	NodePtr search = __search(list, obj);
	pthread_mutex_unlock(&list->listMutex);

	return search;
}

static void __reverseList(ListPtr list)
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

void reverseList(ListPtr list)
{
	pthread_mutex_lock(&list->listMutex);
	__reverseList(list);
	pthread_mutex_lock(&list->listMutex);
}

static void __printList(const ListPtr list)
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

void printList(const ListPtr list)
{
	pthread_mutex_lock(&list->listMutex);
	__printList(list);
	pthread_mutex_unlock(&list->listMutex);
}

//TODO: implement function
void finishUp(ListPtr list)
{
	list->stop = TRUE;
	pthread_mutex_unlock(&list->listMutex);
	pthread_cond_broadcast(&list->listCondAdd);
	pthread_cond_broadcast(&list->listCondRm);
}
