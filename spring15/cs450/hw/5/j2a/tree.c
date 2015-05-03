/**
 * Author: Aaron Woods
 */

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "error.h"
//#include "scanner.h"
#include "bison-parser.h"
#include "yyinput.h"
#include "type.h"

#define ALLOC(t) \
  t v=malloc(sizeof(*v)); \
  if (!v) ERR("malloc() failed")

extern TreeProg t_prog(TreeBlock block)
{
	ALLOC(TreeProg);
	v->code = TOK_BLOCK;
	v->line = line();
	v->col = col();
	v->u.u_block.block = block;
	return v;
}

extern TreeBlock t_block(TreeDecls decls, TreeStmts stmts, int depth)
{
	ALLOC(TreeBlock);
	v->code = TOK_DECLS;
	v->line = line();
	v->col = col();
	v->u.u_block.decls = decls;
	v->u.u_block.stmts = stmts;
	return v;
}

extern TreeDecls t_decls(TreeDecl decl, TreeDecls decls)
{
	ALLOC(TreeDecls);
	v->code = TOK_DECL;
	v->line = line();
	v->col = col();
	v->u.u_decls.decl = decl;
	v->u.u_decls.decls = decls;
	return v;
}

extern TreeDecl t_decl(TreeType type, SymtabEntry entry)
{
	ALLOC(TreeDecl);
	v->code = TOK_TYPE;
	v->line = line();
	v->col = col();
	v->u.u_decl.type = type;
	v->u.u_decl.symtabEntry = entry;
	return v;
}

extern TreeType t_type(TreeBasic basic, TreeType1 type1)
{
	ALLOC(TreeType);
	v->code = TOK_BASIC;
	v->line = line();
	v->col = col();
	v->u.u_type.basic = basic;
	v->u.u_type.type1 = type1;
	return v;
}

extern TreeType1 t_type1(TreeType1 type1, unsigned int length, unsigned int dims)
{
	ALLOC(TreeType1);
	v->code = TOK_NUM;
	v->u.u_type1.type1 = type1;
	v->u.u_type1.length = length;
	v->u.u_type1.dims = dims;
	v->line = line();
	v->col = col();
	return v;
}

extern TreeBasic t_basic_int()
{
	ALLOC(TreeBasic);
	v->code = TOK_INT;
	v->line = line();
	v->col = col();
	return v; 
}

extern TreeBasic t_basic_float()
{
	ALLOC(TreeBasic);
	v->code = TOK_FLOAT;
	v->line = line();
	v->col = col();
	return v;
}

extern TreeStmts t_stmts(TreeStmt stmt, TreeStmts stmts)
{
	ALLOC(TreeStmts);
	v->code = TOK_STMT;
	v->line = line();
	v->col = col();
	v->u.u_stmts.stmt = stmt;
	v->u.u_stmts.stmts = stmts;
	return v;
}

extern TreeStmt t_stmt_assign(TreeLoc loc, TreeBool bool) 
{
 	ALLOC(TreeStmt);
  	v->code = TOK_ID;
	v->line = line();
	v->col = col();
  	v->u.u_assign.loc = loc;
  	v->u.u_assign.bool = bool;
  	return v;
}

extern TreeStmt t_stmt_if(TreeBool bool, TreeStmt stmt)
{
	ALLOC(TreeStmt);
	v->code = TOK_IF;
	v->line = line();
	v->col = col();
	v->u.u_if.bool = bool;
	v->u.u_if.stmt = stmt;
	return v;
}

extern TreeStmt t_stmt_if_else(TreeBool bool, TreeStmt ifStmt, TreeStmt elseStmt)
{
	ALLOC(TreeStmt);
	v->code = TOK_IF_ELSE;
	v->line = line();
	v->col = col();
	v->u.u_if_else.bool = bool;
	v->u.u_if_else.ifStmt = ifStmt;
	v->u.u_if_else.elseStmt = elseStmt;
	return v;
}

extern TreeStmt t_stmt_while(TreeBool bool, TreeStmt stmt) 
{
	ALLOC(TreeStmt);
  	v->code = TOK_WHILE;
	v->line = line();
	v->col = col();
  	v->u.u_while.bool=bool;
  	v->u.u_while.stmt=stmt;
  	return v;
}

extern TreeStmt t_stmt_do(TreeStmt stmt, TreeBool bool)
{
	ALLOC(TreeStmt);
	v->code = TOK_DO;
	v->line = line();
	v->col = col();
	v->u.u_do.stmt = stmt;
	v->u.u_do.bool = bool;
	return v;
}

extern TreeStmt t_stmt_break() 
{
  	ALLOC(TreeStmt);
  	v->code = TOK_BREAK;
	v->line = line();
	v->col = col();
  	return v;
}

extern TreeStmt t_stmt_block(TreeBlock block)
{
	ALLOC(TreeStmt);
	v->code = TOK_BLOCK;
	v->line = line();
	v->col = col();
	v->u.u_block.block = block;
	return v;
}

extern TreeStmt t_stmt_read(TreeLoc loc)
{
	ALLOC(TreeStmt);
	v->code = TOK_READ;
	v->line = line();
	v->col = col();
	v->u.u_read.loc = loc;
	return v;
}

extern TreeStmt t_stmt_write(TreeBool bool)
{
	ALLOC(TreeStmt);
	v->code = TOK_WRITE;
	v->line = line();
	v->col = col();
	v->u.u_write.bool = bool;
	return v;
}

extern TreeLoc t_loc(SymtabEntry entry, TreeLoc1 loc1)
{
	ALLOC(TreeLoc);
	v->code = TOK_ID;
	v->line = line();
	v->col = col();
	v->u.u_loc.symtabEntry = entry;
	v->u.u_loc.loc1 = loc1;
	return v;
}

extern TreeLoc1 t_loc1(TreeBool bool, TreeLoc1 loc1)
{
	ALLOC(TreeLoc1);
	v->code = TOK_BOOL;
	v->line = line();
	v->col = col();
	v->u.u_loc1.bool = bool;
	v->u.u_loc1.loc1 = loc1;
	return v;
}

extern TreeBool t_bool(TreeJoin join, TreeBool1 bool1)
{
	ALLOC(TreeBool);
	v->code = TOK_JOIN;
	v->line = line();
	v->col = col();
	v->u.u_bool.join = join;
	v->u.u_bool.bool1 = bool1;
	return v;
}

extern TreeBool1 t_bool1(TreeJoin join, TreeBool1 bool1)
{
	ALLOC(TreeBool1);
	v->code = TOK_OR;
	v->line = line();
	v->col = col();
	v->u.u_bool1.join = join;
	v->u.u_bool1.bool1 = bool1;
	return v;
}

extern TreeJoin t_join(TreeEquality equality, TreeJoin1 join1)
{
	ALLOC(TreeJoin);
	v->code = TOK_EQUALITY;
	v->line = line();
	v->col = col();
	v->u.u_join.equality = equality;
	v->u.u_join.join1 = join1;
	return v;
}

extern TreeJoin1 t_join1(TreeEquality equality, TreeJoin1 join1)
{
	ALLOC(TreeJoin1);
	v->code = TOK_AND;
	v->line = line();
	v->col = col();
	v->u.u_join1.equality = equality;
	v->u.u_join1.join1 = join1;
	return v;
}

extern TreeEquality t_equality(TreeRel rel, TreeEquality1 equality1)
{
	ALLOC(TreeEquality);
	v->code = TOK_REL;
	v->line = line();
	v->col = col();
	v->u.u_equality.rel = rel;
	v->u.u_equality.equality1 = equality1;
	return v;
}

extern TreeEquality1 t_equality1_eq(TreeRel rel, TreeEquality1 equality1)
{
	ALLOC(TreeEquality1);
	v->code = TOK_EQ;
	v->line = line();
	v->col = col();
	v->u.u_eq.rel = rel;
	v->u.u_eq.equality1 = equality1;
	return v;
}

extern TreeEquality1 t_equality1_ne(TreeRel rel, TreeEquality1 equality1)
{
	ALLOC(TreeEquality1);
	v->code = TOK_NE;
	v->line = line();
	v->col = col();
	v->u.u_ne.rel = rel;
	v->u.u_ne.equality1 = equality1;
	return v;
}

extern TreeRel t_rel_lt(TreeExpr expr1, TreeExpr expr2)
{
	ALLOC(TreeRel);
	v->code = '<';
	v->line = line();
	v->col = col();
	v->u.u_lt.expr1 = expr1;
	v->u.u_lt.expr2 = expr2;
	return v;
}

extern TreeRel t_rel_lte(TreeExpr expr1, TreeExpr expr2)
{
	ALLOC(TreeRel);
	v->code = TOK_LTE;
	v->line = line();
	v->col = col();
	v->u.u_lte.expr1 = expr1;
	v->u.u_lte.expr2 = expr2;
	return v;
}

extern TreeRel t_rel_gte(TreeExpr expr1, TreeExpr expr2)
{
	ALLOC(TreeRel);
	v->code = TOK_GTE;
	v->line = line();
	v->col = col();
	v->u.u_gte.expr1 = expr1;
	v->u.u_gte.expr2 = expr2;
	return v;
}

extern TreeRel t_rel_gt(TreeExpr expr1, TreeExpr expr2)
{
	ALLOC(TreeRel);
	v->code = '>';
	v->line = line();
	v->col = col();
	v->u.u_gt.expr1 = expr1;
	v->u.u_gt.expr2 = expr2;
	return v;
}

extern TreeRel t_rel_expr(TreeExpr expr)
{
	ALLOC(TreeRel);
	v->code = TOK_EXPR;
	v->line = line();
	v->col = col();
	v->u.u_expr.expr = expr;
	return v;
}

extern TreeExpr t_expr(TreeTerm term, TreeExpr1 expr1)
{
	ALLOC(TreeExpr);
	v->code = TOK_TERM;
	v->line = line();
	v->col = col();
	v->u.u_expr.term = term;
	v->u.u_expr.expr1 = expr1;
	return v;
}

extern TreeExpr1 t_expr1_plus(TreeTerm term, TreeExpr1 expr1)
{
	ALLOC(TreeExpr1);
	v->code = '+';
	v->line = line();
	v->col = col();
	v->u.u_plus.term = term;
	v->u.u_plus.expr1 = expr1;
	return v;
}

extern TreeExpr1 t_expr1_minus(TreeTerm term, TreeExpr1 expr1)
{
	ALLOC(TreeExpr1);
	v->code = '-';
	v->line = line();
	v->col = col();
	v->u.u_minus.term = term;
	v->u.u_minus.expr1 = expr1;
	return v;
}

extern TreeTerm t_term(TreeUnary unary, TreeTerm1 term1)
{
	ALLOC(TreeTerm);
	v->code = TOK_UNARY;
	v->line = line();
	v->col = col();
	v->u.u_term.unary = unary;
	v->u.u_term.term1 = term1;
	return v;
}

extern TreeTerm1 t_term1_mul(TreeUnary unary, TreeTerm1 term1)
{
	ALLOC(TreeTerm1);
	v->code = '*';
	v->line = line();
	v->col = col();
	v->u.u_mul.unary = unary;
	v->u.u_mul.term1 = term1;
	return v;
}

extern TreeTerm1 t_term1_div(TreeUnary unary, TreeTerm1 term1)
{
	ALLOC(TreeTerm1);
	v->code = '/';
	v->line = line();
	v->col = col();
	v->u.u_div.unary = unary;
	v->u.u_div.term1 = term1;
	return v;
}

extern TreeUnary t_unary_not(TreeUnary unary)
{
	ALLOC(TreeUnary);
	v->code = '!';
	v->line = line();
	v->col = col();
	v->u.u_not.unary = unary;
	return v;
}

extern TreeUnary t_unary_neg(TreeUnary unary)
{
	ALLOC(TreeUnary);
	v->code = '-';
	v->line = line();
	v->col = col();
	v->u.u_neg.unary = unary;
	return v;
}

extern TreeUnary t_unary_factor(TreeFactor factor)
{
	ALLOC(TreeUnary);
	v->code = TOK_FACTOR;
	v->line = line();
	v->col = col();
	v->u.u_factor.factor = factor;
	return v;
}

extern TreeFactor t_factor_bool(TreeBool bool)
{
	ALLOC(TreeFactor);
	v->code = TOK_BOOL;
	v->line = line();
	v->col = col();
	v->u.u_bool.bool = bool;
	return v;
}

extern TreeFactor t_factor_loc(TreeLoc loc)
{
	ALLOC(TreeFactor);
	v->code = TOK_LOC;
	v->line = line();
	v->col = col();
	v->u.u_loc.loc = loc;
	return v;
}

extern TreeFactor t_factor_num(Type type, char* lex)
{
	ALLOC(TreeFactor);
	v->code = TOK_NUM;
	v->u.u_num.type = type;
	v->u.u_num.lex = lex;
	v->line = line();
	v->col = col();
	return v;
}

extern TreeFactor t_factor_real(Type type, char* lex)
{
	ALLOC(TreeFactor);
	v->code = TOK_REAL;
	v->u.u_real.type = type;
	v->u.u_real.lex = lex;
	v->line = line();
	v->col = col();
	return v;
}

extern TreeFactor t_factor_true()
{
	ALLOC(TreeFactor);
	v->code = TOK_TRUE;
	v->line = line();
	v->col = col();
	return v;
}

extern TreeFactor t_factor_false()
{
	ALLOC(TreeFactor);
	v->code = TOK_FALSE;
	v->line = line();
	v->col = col();
	return v;
}
