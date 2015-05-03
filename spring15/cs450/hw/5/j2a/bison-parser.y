%defines
%debug
%verbose

%{
#define YYERROR_VERBOSE 1

#include "tree.h"
#include "symtaben.h"
#include "symtab.h"
extern Tree tree;
extern int yylex(void);
extern void yyerror(const char *s);
static int blockDepth = -1;
%}

%expect 1

%union {
  TreeProg  	treeProg;
  TreeBlock    	treeBlock;
  TreeDecls		treeDecls;
  TreeDecl		treeDecl;
  TreeType		treeType;
  TreeType1		treeType1;
  TreeBasic		treeBasic;
  TreeStmts		treeStmts;
  TreeStmt		treeStmt;
  TreeLoc		treeLoc;
  TreeLoc1		treeLoc1;
  TreeBool		treeBool;
  TreeBool1		treeBool1;
  TreeJoin		treeJoin;
  TreeJoin1		treeJoin1;
  TreeEquality	treeEquality;
  TreeEquality1	treeEquality1;
  TreeRel		treeRel;
  TreeExpr		treeExpr;
  TreeExpr1		treeExpr1;
  TreeTerm		treeTerm;
  TreeTerm1		treeTerm1;
  TreeUnary		treeUnary;
  TreeFactor	treeFactor;
  char*       	treeId;
  char*      	treeNum;
  char*     	treeReal;
  void*			ifElse;
  void*			error;
}

%type <treeProg>  		prog
%type <treeBlock>    	block
%type <treeDecls>		decls
%type <treeDecl>		decl
%type <treeType>		type
%type <treeType1>		type1
%type <treeBasic>		basic
%type <treeStmts>		stmts
%type <treeStmt>		stmt
%type <treeLoc>			loc
%type <treeLoc1>		loc1
%type <treeBool>		bool
%type <treeBool1>		bool1
%type <treeJoin>		join
%type <treeJoin1>		join1
%type <treeEquality>	equality
%type <treeEquality1>	equality1
%type <treeRel>			rel
%type <treeExpr>		expr
%type <treeExpr1>		expr1
%type <treeTerm>		term
%type <treeTerm1>		term1
%type <treeUnary>		unary
%type <treeFactor>		factor

%token TOK_BREAK 		"break"
%token TOK_WHILE 		"while"
%token TOK_AND			"&&"
%token TOK_LTE			"<="
%token TOK_OR			"||"
%token TOK_IF			"if"
%token TOK_ELSE			"else"
%token TOK_DO			"do"
%token TOK_READ			"read"
%token TOK_WRITE		"write"
%token TOK_EQ			"=="
%token TOK_NE  			"!="
%token TOK_GTE			">="
%token TOK_INT			"int"
%token TOK_FLOAT		"float"
%token TOK_TRUE			"true"
%token TOK_FALSE		"false"

%token <ifElse>			TOK_IF_ELSE
%token <error>			TOK_ERROR
%token <treeId>			TOK_ID
%token <treeNum>		TOK_NUM
%token <treeReal>		TOK_REAL
%token <treeBlock>		TOK_BLOCK
%token <treeDecls>		TOK_DECLS
%token <treeDecl>		TOK_DECL
%token <treeType>		TOK_TYPE
%token <treeType1>		TOK_TYPE1
%token <treeBasic>		TOK_BASIC
%token <treeStmt>		TOK_STMT
%token <treeLoc>		TOK_LOC
%token <treeLoc1>		TOK_LOC1
%token <treeBool>		TOK_BOOL
%token <treeBool1>		TOK_BOOL1
%token <treeJoin>		TOK_JOIN
%token <treeJoin1>		TOK_JOIN1
%token <treeEquality>	TOK_EQUALITY
%token <treeEquality1>	TOK_EQUALITY1
%token <treeRel>		TOK_REL
%token <treeExpr>		TOK_EXPR
%token <treeExpr1>		TOK_EXPR1
%token <treeTerm>		TOK_TERM
%token <treeTerm1>		TOK_TERM1
%token <treeUnary>		TOK_UNARY
%token <treeFactor>		TOK_FACTOR

%%

prog		: 	block	{ 
							$$ = t_prog($1); 
						};

block		:	'{' decls stmts '}' {	
										blockDepth++; 
										SymTabPush();
										$$ = t_block($2, $3, blockDepth);
										blockDepth--;
										SymTabPop();
									};

decls		:	decl decls	{
								$$ = t_decls($1, $2);
							}
			|				{
								$$ = 0;
							}

decl		:	type TOK_ID ';'	{
									$$ = t_decl($1, SymtabEntryNew(yylval.treeId));
								};

type		:	basic type1	{
								$$ = t_type($1, $2);
							};

type1		:	'[' TOK_NUM ']' type1	{
											// TODO: Figure how to determine
											// number of dims for multi-dim
											// arrays.
											$$ = t_type1($4, atoi(yylval.treeNum), 1);
										}
			|							{
											$$ = 0;
										}
								
basic		:	TOK_INT		{
								$$ = t_basic_int();
							}
			|	TOK_FLOAT	{
								$$ = t_basic_float();
							};

stmts		:	stmt stmts
				{
					$$ = t_stmts($1, $2);
				}
			|	{
					$$ = 0;
				}

stmt		:	loc TOK_EQ bool ';'	
				{
					$$ = t_stmt_assign($1, $3);
				}
			|	TOK_IF '(' bool ')' stmt
				{
					$$ = t_stmt_if($3, $5);
				}
			|	TOK_IF	'('	bool ')' stmt TOK_ELSE stmt
				{
					$$ = t_stmt_if_else($3, $5, $7);
				}
			|	TOK_WHILE '(' bool ')' stmt
				{
					$$ = t_stmt_while($3, $5);
				}
			|	TOK_DO stmt TOK_WHILE '(' bool ')' ';'
				{
					$$ = t_stmt_do($2, $5);
				}
			|	TOK_BREAK ';'
				{
					$$ = t_stmt_break();
				}
			|	block
				{
					$$ = t_stmt_block($1);
				}
			|	TOK_READ loc ';'
				{
					$$ = t_stmt_read($2);
				}
			|	TOK_WRITE bool ';'
				{
					$$ = t_stmt_write($2);
				};

loc			:	TOK_ID loc1
				{
					$$ = t_loc(SymtabEntryNew(yylval.treeId), $2);
				};

loc1		:	'[' bool ']' loc1
				{
					$$ = t_loc1($2, $4);
				}
			|	{
					$$ = 0;
				};

bool		:	join bool1
				{
					$$ = t_bool($1, $2);
				};

bool1		:	TOK_OR join bool1
				{
					$$ = t_bool1($2, $3);
				}
			|	{
					$$ = 0;
				};

join		:	equality join1
				{
					$$ = t_join($1, $2);
				};

join1		:	TOK_AND equality join1
				{
					$$ = t_join1($2, $3);
				}
			|	{
					$$ = 0;
				};

equality	:	rel equality1
				{
					$$ = t_equality($1, $2);
				};

equality1	:	TOK_EQ rel equality1
				{
					$$ = t_equality1_eq($2, $3);
				}
			|	TOK_NE rel equality1
				{
					$$ = t_equality1_ne($2, $3);
				}
			|	{
					$$ = 0;
				};

rel			:	expr '<' expr
				{
					$$ = t_rel_lt($1, $3);
				}
			|	expr TOK_LTE expr
				{
					$$ = t_rel_lte($1, $3);
				}
			|	expr TOK_GTE expr
				{
					$$ = t_rel_gte($1, $3);
				}
			|	expr '>' expr
				{
					$$ = t_rel_gt($1, $3);
				}
			|	expr
				{
					$$ = t_rel_expr($1);
				};

expr		:	term expr1
				{
					$$ = t_expr($1, $2);
				}

expr1		:	'+' term expr1
				{
					$$ = t_expr1_plus($2, $3);
				}
			|	'-' term expr1
				{
					$$ = t_expr1_minus($2, $3);
				}
			|	{
					$$ = 0;
				};

term		:	unary term1
				{
					$$ = t_term($1, $2);
				};

term1		:	'*' unary term1
				{
					$$ = t_term1_mul($2, $3);
				}
			|	'/' unary term1
				{
					$$ = t_term1_div($2, $3);
				}
			|	{
					$$ = 0;
				}

unary		:	'!' unary
				{
					$$ = t_unary_not($2);
				}
			|	'-' unary
				{
					$$ = t_unary_neg($2);
				}
			|	factor
				{
					$$ = t_unary_factor($1);
				};

factor		:	'(' bool ')'
				{
					$$ = t_factor_bool($2);
				}
			|	loc
				{
					$$ = t_factor_loc($1);
				}
			|	TOK_NUM
				{
					$$ = t_factor_num(TOK_INT, yylval.treeNum);
				}
			|	TOK_REAL
				{
					$$ = t_factor_real(TOK_FLOAT, yylval.treeReal);
				}
			|	TOK_TRUE
				{
					$$ = t_factor_true();
				}
			|	TOK_FALSE
				{
					$$ = t_factor_false();
				};
