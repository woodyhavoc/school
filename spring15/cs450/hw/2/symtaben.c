#include <stdlib.h>
#include "symtaben.h"

SymtabEntry SymtabEntryNew(char* lex)
{
	SymtabEntry entry = (SymtabEntry)malloc(sizeof(SymtabEntry));
	entry->id = lex;

	return entry;
}
