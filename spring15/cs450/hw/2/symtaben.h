#ifndef SYMTABEN_H
#define SYMTABEN_H

struct SymtabEntry
{
	int depth;
	int offset;
	char* id;
};

typedef struct SymtabEntry *SymtabEntry;

SymtabEntry SymtabEntryNew(char* lex);

#endif /* SYMTAB_H */
