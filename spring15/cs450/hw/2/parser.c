#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "symtab.h"
#include "scanner/token.h"
#include "scanner/scanner.h"
#include "scanner/linkage.h"

static int depth = -1;
static int offset = -1;

static TreeProg p_prog();
static TreeBlock p_block();
static TreeDecls p_decls();
static TreeDecl p_decl();
static TreeType p_type();
static TreeType1 p_type1();
static TreeBasic p_basic();
static TreeStmts p_stmts();
static TreeStmt p_stmt();
static TreeLoc p_loc();
static TreeLoc1 p_loc1();
static TreeBool p_bool();
static TreeBool1 p_bool1();
static TreeJoin p_join();
static TreeJoin1 p_join1();
static TreeEquality p_equality();
static TreeEquality1 p_equality1();
static TreeRel p_rel();
static TreeExpr p_expr();
static TreeExpr1 p_expr1();
static TreeTerm p_term();
static TreeTerm1 p_term1();
static TreeUnary p_unary();
static TreeFactor p_factor();

static void error_parse(char* str, int line, int col);

TreeProg parse()
{
	return p_prog();
}

/**
 * program : block
 */
TreeProg p_prog()
{
	TreeProg prog = 0;
	TreeBlock block = p_block();
	prog = t_prog(block);

	return prog;
}

/**
 * block : '{' decls stmts '}'
 */
TreeBlock p_block()
{
	TreeBlock block = 0;
	depth++;
	offset = -1;
	TokenCode code = curr()->code;

	switch(code)
	{
		case '{':
		{
			eat('{');
			TreeDecls decls = p_decls();
			TreeStmts stmts = p_stmts();
			eat('}');
			block = t_block(decls, stmts);
			break;
		}

		default:
		{
			error_parse("block", line(), col());
			break;
		}
	}
	depth--;
	return block;
}

/**
 * decls : decl decls
 * 		 |
 */
TreeDecls p_decls()
{
	TreeDecls decls = 0;

	TokenCode code = curr()->code;

	switch (code)
	{
		case TOK_INT:
		case TOK_FLOAT:
		{
			TreeDecl decl = p_decl();
			TreeDecls decls1 = p_decls();
			decls = t_decls(decl, decls1);
			break;
		}

		default: break;
	}

	return decls;
}

/**
 * decl : type id ';'
 */
TreeDecl p_decl()
{
	TreeDecl decl = 0;
	offset++;
	TokenCode code = curr()->code;
	
	switch(code)
	{
		case TOK_INT:
		case TOK_FLOAT:
		{
			TreeType type = p_type();
			SymtabEntry entry = SymtabPut(curr()->lexeme, offset);
			entry->depth = depth;
			entry->offset = offset;
			eat(TOK_ID);
			eat(';');
			decl = t_decl(type);
			break;
		}

		default:
		{
			error_parse("decl", line(), col());
			break;
		}
	}

	return decl;
}

/**
 * type : basic type1
 */
TreeType p_type()
{
	TreeType type = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case TOK_INT:
		case TOK_FLOAT:
		{
			TreeBasic basic = p_basic();
			TreeType1 type1 = p_type1();
			type = t_type(basic, type1);
			break;
		}

		default:
		{
			error_parse("type", line(), col());
			break;
		}
	}

	return type;
}

/**
 * type1 : '[' num ']' type1
 * 		 |
 */
TreeType1 p_type1()
{
	TreeType1 type1 = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case TOK_NUM:
		{
			eat('[');
			eat(TOK_NUM);
			eat(']');
			TreeType1 type2 = p_type1();
			type1 = t_type1(type2);
			break;
		}

		default: break;
	}

	return type1;
}

/**
 * basic : 'int'
 * 		 | 'float'
 */
TreeBasic p_basic()
{
	TreeBasic basic = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case TOK_INT:
		{
			eat(TOK_INT);
			basic = t_basic_int();
			break;
		}

		case TOK_FLOAT:
		{
			eat(TOK_FLOAT);
			basic = t_basic_float();
			break;
		}

		default:
		{
			error_parse("basic", line(), col());
			break;
		}
	}

	return basic;
}

/**
 * stmts : stmt stmts
 * 		 |
 */
TreeStmts p_stmts()
{
	TreeStmts stmts = 0;

	TokenCode code = curr()->code;

	switch (code)
	{
		case TOK_NUM:
		case TOK_REAL:
		case TOK_TRUE:
		case TOK_FALSE:
		case TOK_IF:
		case TOK_ELSE:
		case TOK_WHILE:
		case TOK_DO:
		case TOK_BREAK:
		case TOK_READ:
		case TOK_WRITE:
		{
			TreeStmt stmt = p_stmt();
			TreeStmts stmts1 = p_stmts();
			stmts = t_stmts(stmt, stmts1);
			break;
		}

		default: break;
	}

	return stmts;
}

/**
 * stmt: loc '=' bool ';'
 * 	   | 'if' '(' bool ')' stmt
 * 	   | 'if' '(' bool ')' stmt 'else' stmt
 * 	   | 'while' '(' bool ')' stmt
 * 	   | 'do' stmt 'while' '(' bool ')' ';'
 * 	   | 'break' ';'
 * 	   | block
 * 	   | 'read' loc ';'
 * 	   | 'write' bool ';'
 */
TreeStmt p_stmt()
{
  	TreeStmt stmt = 0;
  	TokenCode code = curr()->code;
  	switch(code)
  	{
    	case TOK_ID: 
		{			
      		TreeLoc loc=p_loc();
      		eat('=');
      		TreeBool bool=p_bool();
      		eat(';');
      		stmt = t_stmt_assign(loc,bool);
      		break;
    	}

		case TOK_IF:
		{
			eat(TOK_IF);
			eat('(');
			TreeBool bool = p_bool();
			eat(')');
			TreeStmt stmt1 = p_stmt();
			stmt = t_stmt_if(bool, stmt1);
			break;
		}

		case TOK_ELSE:
		{
			eat(TOK_ELSE);
			TreeStmt stmt1 = p_stmt();
			stmt = t_stmt_else(stmt1);
			break;
		}

    	case TOK_WHILE:
		{
      		eat(TOK_WHILE);
      		eat('(');
      		TreeBool bool=p_bool();
      		eat(')');
      		TreeStmt body=p_stmt();
      		stmt=t_stmt_while(bool,body);
      		break;
    	}

		case TOK_DO:
		{
			eat(TOK_DO);
			TreeStmt stmt1 = p_stmt();
			eat(TOK_WHILE);
			eat('(');
			TreeBool bool = p_bool();
			eat(')');
			eat(';');
			stmt = t_stmt_do(stmt1, bool);
			break;	
		}

    	case TOK_BREAK:
		{
      		eat(TOK_BREAK);
      		eat(';');
      		stmt = t_stmt_break();
      		break;
    	}

		case '{':
		{
			TreeBlock block = p_block();
			stmt = t_stmt_block(block);
			break;
		}

		case TOK_READ:
		{		
			eat(TOK_READ);
			TreeLoc loc = p_loc();
			stmt = t_stmt_read(loc);
			eat(';');
			break;
		}

		case TOK_WRITE:
		{
			eat(TOK_WRITE);
			TreeBool bool = p_bool();
			stmt = t_stmt_write(bool);
			eat(';');
			break;
		}

    	default:
		{
      		error_parse("stmt", line(), col());
      		break;
		}
  	}

  return stmt;
}

/**
 * loc : id loc1
 */
TreeLoc p_loc()
{
	TreeLoc loc = 0;
	TreeLoc1 loc1 = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case TOK_ID:
		{
			SymtabEntry entry = SymtabPut(curr()->lexeme, offset);
			entry->depth = depth;
			entry->offset = offset;
			eat(TOK_ID);
			loc1 = p_loc1();
			loc = t_loc(loc1);
			break;
		}

		default:
		{
			error_parse("loc", line(), col());
			break;
		}
	}

	return loc;
}

/**
 * loc1 : '[' bool ']' loc1
 * 		|
 */
TreeLoc1 p_loc1()
{
	TreeLoc1 loc1 = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case '[':
		{
			eat('[');
			TreeBool bool = p_bool();
			eat(']');
			TreeLoc1 loc2 = p_loc1();
			loc1 = t_loc1(bool, loc2);
			break;
		}

		default: break;
	}

	return loc1;
}

/**
 * bool : join bool1
 */
TreeBool p_bool()
{
	TreeBool bool = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case '!':
		case '-':
		case TOK_NUM:
		case TOK_REAL:
		case TOK_TRUE:
		case TOK_FALSE:
		{
			TreeJoin join = p_join();
			TreeBool1 bool1 = p_bool1();
			bool = t_bool(join, bool1);
			break;
		}

		default:
		{
			error_parse("bool", line(), col());
			break;
		}
	}

	return bool;
}

/**
 * bool1 : '||' join bool1
 * 		 |
 */
TreeBool1 p_bool1()
{
	TreeBool1 bool1 = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case TOK_OR:
		{
			eat(TOK_OR);
			TreeJoin join = p_join();
			TreeBool1 bool2 = p_bool1();
			bool1 = t_bool1(join, bool2);
			break;
		}

		default: break;
	}

	return bool1;
}

/**
 * join : equality join1
 */
TreeJoin p_join()
{
	TreeJoin join = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case '!':
		case '-':
		case TOK_NUM:
		case TOK_REAL:
		case TOK_TRUE:
		case TOK_FALSE:
		{
			TreeEquality equality = p_equality();
			TreeJoin1 join1 = p_join1();
			join = t_join(equality, join1);
			break;
		}
		
		default:
		{
			error_parse("join", line(), col());
			break;
		}
	}

	return join;
}

/**
 * join1 : '&&' equality join1
 * 		 |
 */
TreeJoin1 p_join1()
{
	TreeJoin1 join1 = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case TOK_AND:
		{
			eat(TOK_AND);
			TreeEquality equality = p_equality();
			TreeJoin1 join2 = p_join1();
			join1 = t_join1(equality, join2);
			break;
		}

		default: break;
	}

	return join1;
}

/**
 * equality : rel
 */
TreeEquality p_equality()
{
	TreeEquality equality = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case '!':
		case '-':
		case TOK_NUM:
		case TOK_REAL:
		case TOK_TRUE:
		case TOK_FALSE:
		{
			TreeRel rel = p_rel();
			TreeEquality1 equality1 = p_equality1();
			equality = t_equality(rel, equality1);
			break;
		}

		default:
		{
			error_parse("equality", line(), col());
			break;
		}
	}

	return equality;
}

/**
 * equality1 : '==' rel equality1
 * 			 | '!=' rel equality1
 * 			 |
 */
TreeEquality1 p_equality1()
{
	TreeEquality1 equality1 = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case TOK_EQ:
		{
			eat(TOK_EQ);
			TreeRel rel = p_rel();
			TreeEquality1 equality2 = p_equality1();
			equality1 = t_equality1_eq(rel, equality2);
			break;
		}

		case TOK_NE:
		{
			eat(TOK_NE);
			TreeRel rel = p_rel();
			TreeEquality1 equality2 =  p_equality1();
			equality1 = t_equality1_ne(rel, equality2);
			break;
		}

		default: break;
	}

	return equality1;
}

/**
 * rel : expr '<' expr
 * 	   | expr '<=' expr
 * 	   | expr '>=' expr
 * 	   | expr '>' expr
 * 	   | expr
 */
TreeRel p_rel()
{
	TreeRel rel = 0;
	TreeExpr expr1 = p_expr();
	TokenCode code = curr()->code;

	switch (code)
	{
		case '<':
		{
			eat('<');
			TreeExpr expr2 = p_expr();
			rel = t_rel_lt(expr1, expr2);
			break;
		}

		case TOK_LTE:
		{
			eat(TOK_LTE);
			TreeExpr expr2 = p_expr();
			rel = t_rel_lte(expr1, expr2);
			break;
		}

		case TOK_GTE:
		{
			eat(TOK_GTE);
			TreeExpr expr2 = p_expr();
			rel = t_rel_gte(expr1, expr2);
			break;
		}

		case '>':
		{
			eat('>');
			TreeExpr expr2 = p_expr();
			rel = t_rel_gt(expr1, expr2);
			break;
		}

		default:
		{
			rel = t_rel_expr(expr1);
			break;
		}
	}

	return rel;
}

/**
 * expr : term expr1
 */
TreeExpr p_expr()
{
	TreeExpr expr = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case '!':
		case '-':
		case TOK_NUM:
		case TOK_REAL:
		case TOK_TRUE:
		case TOK_FALSE:
		{
			TreeTerm term = p_term();
			TreeExpr1 expr1 = p_expr1();
			expr = t_expr(term, expr1);
			break;
		}

		default:
		{
			error_parse("expr", line(), col());
			break;
		}
	}

	return expr;
}

/**
 * expr1 : '+' term expr1
 * 		 | '-' term expr1
 * 		 |
 */
TreeExpr1 p_expr1()
{
	TreeExpr1 expr1 = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case '+':
		{
			eat('+');
			TreeTerm term = p_term();
			TreeExpr1 expr2 = p_expr1();
			expr1 = t_expr1_plus(term, expr2);
			break;
		}

		case '-':
		{
			eat('-');
			TreeTerm term = p_term();
			TreeExpr1 expr2 = p_expr1();
			expr1 = t_expr1_minus(term, expr2);
			break;
		}

		default: break;
	}

	return expr1;
}

/**
 * term : unary term1
 */
TreeTerm p_term()
{
	TreeTerm term = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case '!':
		case '-':
		case TOK_NUM:
		case TOK_REAL:
		case TOK_TRUE:
		case TOK_FALSE:
		{
			TreeUnary unary = p_unary();
			TreeTerm1 term1 = p_term1();
			term = t_term(unary, term1);
			break;
		}

		default: break;
	}

	return term;
}

/**
 * term1 : '*' unary term1
 * 		 | '/' unary term1
 * 		 |
 */
TreeTerm1 p_term1()
{
	TreeTerm1 term1 = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case '*':
		{
			eat('*');
			TreeUnary unary = p_unary();
			TreeTerm1 term2 = p_term1();
			term1 = t_term1_mul(unary, term2);
			break;
		}

		case '/':
		{
			eat('/');
			TreeUnary unary = p_unary();
			TreeTerm1 term2 = p_term1();
			term1 = t_term1_div(unary, term2);
			break;
		}

		default: break;
	}

	return term1;
}

/**
 * unary : '!' unary
 * 		 | '-' unary
 * 		 | factor
 */
TreeUnary p_unary()
{
	TreeUnary unary = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case '!':
		{
			eat('!');
			TreeUnary unary1 = p_unary();
			unary = t_unary_not(unary1);
			break;
		}

		case '-':
		{
			eat('-');
			TreeUnary unary1 = p_unary();
			unary = t_unary_neg(unary1);
			break;
		}

		case TOK_NUM:
		case TOK_REAL:
		case TOK_TRUE:
		case TOK_FALSE:
		{
			TreeFactor factor = p_factor();
			unary = t_unary_factor(factor);
			break;
		}

		default: break;
	}

	return unary;
}

/**
 * factor : '(' bool ')'
 * 		  | loc
 * 		  | num
 * 		  | real
 * 		  | 'true'
 * 		  | 'false'
 */
TreeFactor p_factor()
{
	TreeFactor factor = 0;
	TokenCode code = curr()->code;

	switch (code)
	{
		case '(':
		{
			eat(')');
			TreeBool bool = p_bool();
			eat(')');
			factor = t_factor_bool(bool);
			break;
		}

		case TOK_ID:
		{
			TreeLoc loc = p_loc();
			factor = t_factor_loc(loc);
			break;
		}

		case TOK_NUM:
		{
			eat(TOK_NUM);
			factor = t_factor_num();
			break;
		}

		case TOK_REAL:
		{
			eat(TOK_REAL);
			factor = t_factor_real();
			break;
		}

		case TOK_TRUE:
		{
			eat(TOK_TRUE);
			factor = t_factor_true();
			break;
		}

		case TOK_FALSE:
		{
			eat(TOK_FALSE);
			factor = t_factor_false();
			break;
		}

		default:
		{
			error_parse("factor", line(), col());
			break;
		}
	}

	return factor;
}

static void error_parse(char* str, int line, int col)
{
	fprintf(stderr, "Parsing error, function: %s, line no.: %d, col. no.: %d\n", 
			str, line, col);
}
