/**
 * Author: Aaron Woods
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "symtaben.h"

extern LINKAGE SymtabEntry SymtabEntryNew(char* lex)
{
	SymtabEntry entry = (SymtabEntry)malloc(sizeof(SymtabEntry));
	entry->depth = SymTabDepth();
	entry->offset = SymTabOffset(0);
	entry->id = (char*)malloc(sizeof(char) * strlen(lex) + 1);
	entry->id = lex;	

	return entry;
}
