/**
 * Author: Aaron Woods
 */

#include <stdlib.h>
#include "break.h"

static BreakStack curr;
static unsigned int size = 0;

void breakPush(char* label)
{
	BreakStack tmp = (BreakStack)malloc(sizeof(BreakStack));
	tmp->label = label;
	tmp->prev = curr;
	size++;
	curr = tmp;	
}

void breakPop()
{
	if(size == 0)
	{
		return;
	}
	curr = curr->prev;
	size--;
}

char* breakCurr()
{
	return curr->label;
}
