%defines
%debug
%verbose

%{
#define YYERROR_VERBOSE 1

extern Tree tree;
extern int yylex(void);
extern void yyerror(char *s);

%}

%expect 0

%union {
  TreeProg  	treeProg;
  TreeBlock    	treeBlock;
  TreeDecls		treeDecls;
  TreeStmts		treeStmts;
  TreeId       	treeId;
  TreeNum      	treeNum;
  TreeReal     	treeReal;
}

%type <treeProgram>  	prog
%type <treeBlock>    	block
%type <treeDecls>		decls
%type <treeStmts>		stmts

%token TOK_BREAK "break"
%token TOK_NE  "!="

%token <treeId>	  TOK_ID
%token <treeNum>  TOK_NUM
%token <treeReal> TOK_REAL

%%

prog: block { tree=t_prog($1); };

block: '{' decls stmts '}' { treeBlock = t_block($2, $3); };

decls:	{return 0;};

stmts:	{return 0;};
