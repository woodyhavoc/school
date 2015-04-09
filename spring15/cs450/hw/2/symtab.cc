#include <map>
#include <vector>

#include "symtab.h"
#include "symtaben.h"
#include "error.h"

using namespace std;

typedef map<char*,SymtabEntry> ScopeTab;
typedef vector<ScopeTab*> SymTab;
typedef vector<int> OffTab;

static SymTab symtab;
static OffTab offtab;
static int maxdepth=-1;

extern LINKAGE void SymTabPush(void) {
  symtab.push_back(new ScopeTab);
  offtab.push_back(0);
  if (maxdepth<SymTabDepth())
      maxdepth=SymTabDepth();
}

extern LINKAGE void SymTabPop(void) {
  if (!symtab.empty())
    symtab.pop_back();
  if (!offtab.empty())
    offtab.pop_back();
}

static SymtabEntry ScopeTabGet(char* id, ScopeTab* scopetab) {
  ScopeTab::iterator it=scopetab->find(id);
  return it==scopetab->end() ? 0 : it->second;
}

extern LINKAGE SymtabEntry SymTabGet(char* id, int dist) {
  return ScopeTabGet(id,symtab[SymTabDepth()-dist]);
}

extern LINKAGE SymtabEntry SymTabPut(char* id, int dist) {
  SymtabEntry entry;
  if (symtab.empty())
    SymTabPush();
  entry=SymtabEntryNew(id);
  (*symtab[SymTabDepth()-dist])[id]=entry;
  return entry;
}

extern LINKAGE SymtabEntry SymTabFind(char* id) {
  SymtabEntry entry=0;
  for (int i=SymTabDepth(); i>=0 && !entry; i--)
    entry=ScopeTabGet(id,symtab[i]);
  return entry;
}

extern LINKAGE int SymTabDepth() {
  return symtab.size()-1;
}

extern LINKAGE int SymTabMaxDepth() {
  return maxdepth;
}

extern LINKAGE int SymTabOffset(int size) {
  int i=symtab.size()-1;
  int off=offtab[i];
  offtab[i]+=size;
  return off;
}
