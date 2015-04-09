/**
 * Author: Aaron Woods
 */

#ifndef BREAK_H
#define BREAK_H

typedef struct breakStack *BreakStack;

struct breakStack
{
	char* label;
	BreakStack prev;
};

/**
 * Pushes a new element onto the break stack
 *
 * @param label the element to be added to the break stack
 */
void breakPush(char* label);

/**
 * Pops an element off of the break stack
 */
void breakPop();

/**
 * Returns the label of the current break stack element
 */
char* breakCurr();

#endif /* BREAK_H */
