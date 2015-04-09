#ifndef SYMTAB_H
#define SYMTAB_H

#include "scanner/linkage.h"
#include "symtaben.h"

extern LINKAGE void SymTabPush(void);
extern LINKAGE void SymTabPop(void);
extern LINKAGE SymtabEntry SymTabGet(char* id, int dist);
extern LINKAGE SymtabEntry SymTabPut(char* id, int dist);
extern LINKAGE SymtabEntry SymTabFind(char* id);

extern LINKAGE int SymTabDepth();
extern LINKAGE int SymTabMaxDepth();
extern LINKAGE int SymTabOffset(int size);

#endif
