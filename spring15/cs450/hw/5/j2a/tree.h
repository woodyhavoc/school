#ifndef TREE_H
#define TREE_H

#include "symtaben.h"
//#include "token.h"

// Struct definitions
typedef struct TreeProg		*Tree;
typedef struct TreeProg *TreeProg;
typedef struct TreeBlock *TreeBlock;
typedef struct TreeDecls *TreeDecls;
typedef struct TreeDecl *TreeDecl;
typedef struct TreeType *TreeType;
typedef struct TreeType1 *TreeType1;
typedef struct TreeBasic *TreeBasic;
typedef struct TreeStmts *TreeStmts;
typedef struct TreeStmt *TreeStmt;
typedef struct TreeLoc *TreeLoc;
typedef struct TreeLoc1 *TreeLoc1;
typedef struct TreeBool *TreeBool;
typedef struct TreeBool1 *TreeBool1;
typedef struct TreeJoin *TreeJoin;
typedef struct TreeJoin1 *TreeJoin1;
typedef struct TreeEquality *TreeEquality;
typedef struct TreeEquality1 *TreeEquality1;
typedef struct TreeRel *TreeRel;
typedef struct TreeExpr *TreeExpr;
typedef struct TreeExpr1 *TreeExpr1;
typedef struct TreeTerm *TreeTerm;
typedef struct TreeTerm1 *TreeTerm1;
typedef struct TreeUnary *TreeUnary;
typedef struct TreeFactor *TreeFactor;

Tree tree;

// Functions for creating all the tree structures
extern TreeProg t_prog(TreeBlock block);
extern TreeBlock t_block(TreeDecls decls, TreeStmts stmts, int depth);
extern TreeDecls t_decls(TreeDecl decl, TreeDecls decls);
extern TreeDecl t_decl(TreeType type, SymtabEntry entry);
extern TreeType t_type(TreeBasic basic, TreeType1 type1);
extern TreeType1 t_type1(TreeType1 type1, unsigned int length, unsigned int dims);
extern TreeBasic t_basic_int();
extern TreeBasic t_basic_float();
extern TreeStmts t_stmts(TreeStmt stmt, TreeStmts stmts);
extern TreeStmt t_stmt_assign(TreeLoc loc, TreeBool bool); 
extern TreeStmt t_stmt_if(TreeBool bool, TreeStmt stmt);
extern TreeStmt t_stmt_if_else(TreeBool bool, TreeStmt ifStmt, TreeStmt elseStmt);
extern TreeStmt t_stmt_while(TreeBool bool, TreeStmt stmt);
extern TreeStmt t_stmt_do(TreeStmt stmt, TreeBool bool);
extern TreeStmt t_stmt_break();
extern TreeStmt t_stmt_block(TreeBlock block);
extern TreeStmt t_stmt_read(TreeLoc loc);
extern TreeStmt t_stmt_write(TreeBool bool);
extern TreeLoc t_loc(SymtabEntry entry, TreeLoc1 loc1);
extern TreeLoc1 t_loc1(TreeBool bool, TreeLoc1 loc1);
extern TreeBool t_bool(TreeJoin join, TreeBool1 bool1);
extern TreeBool1 t_bool1(TreeJoin join, TreeBool1 bool1);
extern TreeJoin t_join(TreeEquality equality, TreeJoin1 join1);
extern TreeJoin1 t_join1(TreeEquality equality, TreeJoin1 join1);
extern TreeEquality t_equality(TreeRel rel, TreeEquality1 equality1);
extern TreeEquality1 t_equality1_eq(TreeRel rel, TreeEquality1 equality1);
extern TreeEquality1 t_equality1_ne(TreeRel rel, TreeEquality1 equality1);
extern TreeRel t_rel_lt(TreeExpr expr1, TreeExpr expr2);
extern TreeRel t_rel_lte(TreeExpr expr1, TreeExpr expr2);
extern TreeRel t_rel_gte(TreeExpr expr1, TreeExpr expr2);
extern TreeRel t_rel_gt(TreeExpr expr1, TreeExpr expr2);
extern TreeRel t_rel_expr(TreeExpr expr);
extern TreeExpr t_expr(TreeTerm term, TreeExpr1 expr1);
extern TreeExpr1 t_expr1_plus(TreeTerm term, TreeExpr1 expr1);
extern TreeExpr1 t_expr1_minus(TreeTerm term, TreeExpr1 expr1);
extern TreeTerm t_term(TreeUnary unary, TreeTerm1 term1);
extern TreeTerm1 t_term1_mul(TreeUnary unary, TreeTerm1 term1);
extern TreeTerm1 t_term1_div(TreeUnary unary, TreeTerm1 term1);
extern TreeUnary t_unary_not(TreeUnary unary);
extern TreeUnary t_unary_neg(TreeUnary unary);
extern TreeUnary t_unary_factor(TreeFactor factor);
extern TreeFactor t_factor_bool(TreeBool bool);
extern TreeFactor t_factor_loc(TreeLoc loc);
extern TreeFactor t_factor_num();
extern TreeFactor t_factor_real();
extern TreeFactor t_factor_true();
extern TreeFactor t_factor_false();

struct TreeProg
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeBlock block;
		} u_block;
	} u;
};

struct TreeBlock
{
	int code;
	int line;
	int col;
	int depth;
	union
	{
		struct
		{
			TreeDecls decls;
			TreeStmts stmts;
		} u_block;
	} u;
};

struct TreeDecls
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeDecl decl;
			TreeDecls decls;
		} u_decls;
	} u;
};

struct TreeDecl
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeType type;
			SymtabEntry symtabEntry;
		} u_decl;
	} u;
};

struct TreeType
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeBasic basic;
			TreeType1 type1;
		} u_type;
	} u;
};

struct TreeType1
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeType1 type1;
			unsigned int length;
			unsigned int dims;
		} u_type1;
	} u;
};

struct TreeBasic
{
	int code;
	int line;
	int col;
};

struct TreeStmts
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeStmt stmt;
			TreeStmts stmts;
		} u_stmts;
	} u;
};

struct TreeStmt
{
	int code;
	int line;
	int col;
  	union
  	{
   		struct
		{
      		TreeLoc loc;
      		TreeBool bool;
    	} u_assign;
	 
		struct
		{
			TreeBool bool;
			TreeStmt stmt;
		} u_if;

		struct
		{
			TreeBool bool;
			TreeStmt ifStmt;
			TreeStmt elseStmt;
		} u_if_else;
		
		struct
		{
      		TreeBool bool;
      		TreeStmt stmt;
    	} u_while;

		struct
		{
			TreeStmt stmt;
			TreeBool bool;
		} u_do;

		struct
		{
			TreeBlock block;
		} u_block;

		struct
		{
			TreeLoc loc;
		} u_read;

		struct
		{
			TreeBool bool;
		} u_write;
  	} u;
};

struct TreeLoc
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			SymtabEntry symtabEntry;
			TreeLoc1 loc1;
		} u_loc;
	} u;
};

struct TreeLoc1
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeBool bool;
			TreeLoc1 loc1;
		} u_loc1;
	} u;
};

struct TreeBool
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeJoin join;
			TreeBool1 bool1;
		} u_bool;
	} u;
};

struct TreeBool1
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeJoin join;
			TreeBool1 bool1;
		} u_bool1;
	} u;
};

struct TreeJoin
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeEquality equality;
			TreeJoin1 join1;
		} u_join;
	} u;
};

struct TreeJoin1
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeEquality equality;
			TreeJoin1 join1;
		} u_join1;
	} u;
};

struct TreeEquality
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeRel rel;
			TreeEquality1 equality1;
		} u_equality;
	} u;
};

struct TreeEquality1
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeRel rel;
			TreeEquality1 equality1;
		} u_eq;

		struct
		{
			TreeRel rel;
			TreeEquality1 equality1;
		} u_ne;
	} u;
};

struct TreeRel
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeExpr expr1;
			TreeExpr expr2;
			Type type;
		} u_lt;

		struct
		{
			TreeExpr expr1;
			TreeExpr expr2;
			Type type;
		} u_lte;

		struct
		{
			TreeExpr expr1;
			TreeExpr expr2;
			Type type;
		} u_gte;

		struct
		{
			TreeExpr expr1;
			TreeExpr expr2;
			Type type;
		} u_gt;

		struct
		{
			TreeExpr expr;
		} u_expr;
	} u;
};

struct TreeExpr
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeTerm term;
			TreeExpr1 expr1;
		} u_expr;
	} u;
};

struct TreeExpr1
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeTerm term;
			TreeExpr1 expr1;
		} u_plus;
		
		struct
		{
			TreeTerm term;
			TreeExpr1 expr1;
		} u_minus;
	} u;
};

struct TreeTerm
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeUnary unary;
			TreeTerm1 term1;
		} u_term;
	} u;
};

struct TreeTerm1
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeUnary unary;
			TreeTerm1 term1;
		} u_mul;

		struct
		{
			TreeUnary unary;
			TreeTerm1 term1;
		} u_div;
	} u;
};

struct TreeUnary
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeUnary unary;
		} u_not;

		struct
		{
			TreeUnary unary;
		} u_neg;

		struct
		{
			TreeFactor factor;
		} u_factor;
	} u;
};

struct TreeFactor
{
	int code;
	int line;
	int col;
	union
	{
		struct
		{
			TreeBool bool;
		} u_bool;

		struct
		{
			TreeLoc loc;
		} u_loc;

		struct
		{
			Type type;
			char* lex;
		} u_num;

		struct
		{
			Type type;
			char* lex;
		} u_real;
	} u;
};

#endif /* TREE_H */
