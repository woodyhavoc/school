/**
 * Author: Aaron Woods
 */

#ifndef SYMTABEN_H
#define SYMTABEN_H

#include "type.h"
#include "linkage.h"

typedef struct symtabEntry
{
	Type type;
	int depth;
	int offset;
	char* id;
} *SymtabEntry;

/**
 * Creates a new symbol table entry
 *
 * @param lex the string identifier of the element to be
 * 				  added to the symbol table
 */
extern LINKAGE SymtabEntry SymtabEntryNew(char* lex);

#endif /* SYMTAB_H */
