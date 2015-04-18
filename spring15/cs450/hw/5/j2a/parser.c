/**
 * Author: Aaron Woods
 */

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "symtab.h"
#include "token.h"
#include "scanner.h"
#include "linkage.h"
#include "type.h"

#define TRUE 1
#define FALSE 0

#define DEBUG 1
//#define DEBUG 0

#warning write the documentation for parser functions

/**
 * Parses a TreeProg
 *
 * @return a parsed TreeProg
 */
static TreeProg p_prog();

/**
 * Parses a TreeBlock
 *
 * @return a parsed TreeBlock
 */
static TreeBlock p_block();

/**
 * Parses a TreeDecls
 *
 * @return a parsed TreeDecls
 */
static TreeDecls p_decls();

/**
 * Parses a TreeDecl
 *
 * @return a parsed TreeDecl
 */
static TreeDecl p_decl();

/**
 * Parses a TreeType
 *
 * @return a parsed TreeType
 */
static TreeType p_type();

/**
 * Parses a TreeType1
 *
 * @return a parsed TreeType1
 */
static TreeType1 p_type1();

/**
 * Parses a TreeBasic
 *
 * @return a parsed TreeBasic
 */
static TreeBasic p_basic();

/**
 * Parses a TreeStmts
 *
 * @return a parsed TreeStmts
 */
static TreeStmts p_stmts();

/**
 * Parses a TreeStmt
 *
 * @return a parsed TreeStmt
 */
static TreeStmt p_stmt();

/**
 * Parses a TreeLoc
 *
 * @return a parsed TreeLoc
 */
static TreeLoc p_loc();

/**
 * Parses a TreeLoc1
 *
 * @return a parsed TreeLoc1
 */
static TreeLoc1 p_loc1();

/**
 * Parses a TreeBool
 *
 * @return a parsed TreeBool
 */
static TreeBool p_bool();

/**
 * Parses a TreeBool1
 *
 * @return a parsed TreeBool1
 */
static TreeBool1 p_bool1();

/**
 * Parses a TreeJoin
 *
 * @return a parsed TreeJoin
 */
static TreeJoin p_join();

/**
 * Parses a TreeJoin1
 *
 * @return a parsed TreeJoin1
 */
static TreeJoin1 p_join1();

/**
 * Parses a TreeEquality
 *
 * @return a parsed TreeEquality
 */
static TreeEquality p_equality();

/**
 * Parses a TreeEquality1
 *
 * @return a parsed TreeEquality1
 */
static TreeEquality1 p_equality1();

/**
 * Parses a TreeRel
 *
 * @return a parsed TreeRel
 */
static TreeRel p_rel();

/**
 * Parses a TreeExpr
 *
 * @return a parsed TreeExpr
 */
static TreeExpr p_expr();

/**
 * Parses a TreeExpr1
 *
 * @return a parsed TreeExpr1
 */
static TreeExpr1 p_expr1();

/**
 * Parses a TreeTerm
 *
 * @return a parsed TreeTerm
 */
static TreeTerm p_term();

/**
 * Parses a TreeTerm1
 *
 * @return a parsed TreeTerm1
 */
static TreeTerm1 p_term1();

/**
 * Parses a TreeUnary
 *
 * @return a parsed TreeUnary
 */
static TreeUnary p_unary();

/**
 * Parses a TreeFactor
 *
 * @return a parsed TreeFactor
 */
static TreeFactor p_factor();

/**
 * Determines the number of dimensions of an integer array, and generates
 * a Type for each dimension
 *
 * @param type1 the TreeType1 which represents the integer array
 * @param type the Type for the first dimension, contains a subsequent Type
 * 			   for continued dimension generation
 * @param dims the total number of dimensions
 */
static void setTypeArrayInt(TreeType1 type1, Type type, unsigned int dims);

/**
 * Determines the number of dimensions of an float array, and generates
 * a Type for each dimension
 *
 * @param type1 the TreeType1 which represents the float array
 * @param type the Type for the first dimension, contains a subsequent Type
 * 			   for continued dimension generation
 * @param dims the total number of dimensions
 */
static void setTypeArrayFloat(TreeType1 type1, Type type, unsigned int dims);

/**
 * Prints an error message when there is an error in parsing
 *
 * @param str the error message to be printed
 */
static void error_parse(char* str);

/**
 * This function generates a parse stack trace
 * as the source file is being parsed.
 *
 * @param func the function that was most recently called
 * @param code the current token code
 */
static void p_trace(char* func, int code);

/**
 * This function accepts an integer and returns
 * a string representation of the corresponding
 * token code.
 *
 * @param code the code to be represented as a token code string
 */
static char* p_code(int code);

static int TYPE1_EXIT = TRUE;
static int offset;
static unsigned int WRITTEN = FALSE;
static const char* FILE_NAME = "parserTrace.log";
static FILE* fp;

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
	TokenCode code = curr()->code;

	if(DEBUG)
	{
		p_trace("p_prog", code);
	}

	switch(code)
	{
		case '{':
		{
			TreeBlock block = p_block();
			prog = t_prog(block);
			break;
		}

		default:
		{
			error_parse("prog");
		}
	}

	return prog;
}

/**
 * block : '{' decls stmts '}'
 */
TreeBlock p_block()
{
	TreeBlock block = 0;
	offset = -1;
	TokenCode code = curr()->code;
	static int blockDepth = -1;
		
	if(DEBUG)
	{
		p_trace("p_block", code);
	}
	
	switch(code)
	{
		case '{':
		{
			eat('{');
			blockDepth++;
			SymTabPush();
			TreeDecls decls = p_decls();
			TreeStmts stmts = p_stmts();
			block = t_block(decls, stmts, blockDepth);
			eat('}');
			blockDepth--;
			SymTabPop();
			break;
		}
		
		default:
		{
			error_parse("block");
			break;
		}
	}
	
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
	
	if(DEBUG)
	{
		p_trace("p_decls", code);
	}
	
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

		default:
		{
			break;
		}
	}

	return decls;
}

/**
 * decl : type id ';'
 */
TreeDecl p_decl()
{
	TreeDecl decl = 0;
	TokenCode code = curr()->code;
	
	if(DEBUG)
	{
		p_trace("p_decl", code);
	}
	
	switch(code)
	{
		case TOK_INT:
		{
			Type varType;
			TreeType type = p_type();
			
			if(type->u.u_type.type1)
			{
				unsigned int length = type->u.u_type.type1->u.u_type1.length;
				unsigned int dims = type->u.u_type.type1->u.u_type1.dims;
				varType = TypeNewArray(TYPE_INT_ARRAY, length, dims);
				setTypeArrayInt(type->u.u_type.type1, varType, dims);
				printf("Num dims: %d\n", type->u.u_type.type1->u.u_type1.dims);
			}
			else
			{
				varType = TypeNew(TYPE_INT); 
			}
			
			SymtabEntry entry;	

			if((entry = SymTabFind(curr()->lexeme)))
			{
				error_parse("multiple id declaration");
				printf("%s\n", entry->id);
			}
			else
			{
				entry = SymTabPut(curr()->lexeme, SymTabOffset(++offset));
				entry->type = varType;	
			}

			eat(TOK_ID);
			eat(';');
			decl = t_decl(type, entry);
			break;

		}

		case TOK_FLOAT:
		{
			Type varType;
			TreeType type = p_type();

			if(type->u.u_type.type1)
			{
				unsigned int length = type->u.u_type.type1->u.u_type1.length;
				unsigned int dims = type->u.u_type.type1->u.u_type1.dims;
				varType = TypeNewArray(TYPE_FLOAT_ARRAY, length, dims);
				setTypeArrayFloat(type->u.u_type.type1, varType, dims);
				printf("Num dims: %d\n", type->u.u_type.type1->u.u_type1.dims);
			}
			else
			{
				varType = TypeNew(TYPE_FLOAT);
			}

			SymtabEntry entry;	

			if((entry = SymTabFind(curr()->lexeme)))
			{
				error_parse("multiple id declaration");
				printf("%s\n", entry->id);
			}
			else
			{
				entry = SymTabPut(curr()->lexeme, SymTabOffset(++offset));
				entry->type = varType;	
			}

			eat(TOK_ID);
			eat(';');
			decl = t_decl(type, entry);
			break;
		}
		
		default:
		{
			error_parse("decl");
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
	
	if(DEBUG)
	{
		p_trace("p_type", code);
	}

	switch (code)
	{
		case TOK_INT:
		case TOK_FLOAT:
		{
			TreeBasic basic = p_basic();
			TreeType1 type1 = p_type1();
			TYPE1_EXIT = TRUE;
			type = t_type(basic, type1);
			break;
		}
		
		default:
		{
			error_parse("type");
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
	static unsigned int dims = 0;
	
	if(TYPE1_EXIT)
	{
		TYPE1_EXIT = FALSE;
		dims = 0;
	}
	
	if(DEBUG)
	{
		p_trace("p_type1", code);
	}
	
	switch (code)
	{
		case '[':
		{
			eat('[');
			dims++;
			unsigned int length = atoi(curr()->lexeme);
			eat(TOK_NUM);
			eat(']');
			TreeType1 type2 = p_type1();
			type1 = t_type1(type2, length, dims);
			break;
		}
		
		default:
		{
			break;
		}
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
	
	if(DEBUG)
	{
		p_trace("p_basic", code);
	}

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
			error_parse("basic");
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
	
	if(DEBUG)
	{
		p_trace("p_stmts", code);
	}

	switch (code)
	{
		case TOK_ID:
		case TOK_NUM:
		case TOK_REAL:
		case TOK_TRUE:
		case TOK_FALSE:
		case TOK_IF:
		case TOK_WHILE:
		case TOK_DO:
		case TOK_BREAK:
		case '{':
		case TOK_READ:
		case TOK_WRITE:
		{
			TreeStmt stmt = p_stmt();
			TreeStmts stmts1 = p_stmts();
			stmts = t_stmts(stmt, stmts1);
			break;
		}
		
		default:
		{
			break;
		}
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
	
	if(DEBUG)
	{
		p_trace("p_stmt", code);
	}
	
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
			TreeStmt ifStmt = p_stmt();
			
			if(curr()->code == TOK_ELSE)
			{
				eat(TOK_ELSE);
				TreeStmt elseStmt = p_stmt();
				stmt = t_stmt_if_else(bool, ifStmt, elseStmt);
			}
			else
			{
				stmt = t_stmt_if(bool, ifStmt);
			}

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
      		error_parse("stmt");
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
		
	if(DEBUG)
	{
		p_trace("p_loc", code);
	}

	switch (code)
	{
		case TOK_ID:
		{
			SymtabEntry entry = SymTabFind(curr()->lexeme);
			eat(TOK_ID);
			loc1 = p_loc1();
			loc = t_loc(entry, loc1);
			break;
		}
		
		default:
		{
			error_parse("loc");
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
		
	if(DEBUG)
	{
		p_trace("p_loc1", code);
	}

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

		default:
		{
			break;
		}
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
		
	if(DEBUG)
	{
		p_trace("p_bool", code);
	}

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
			error_parse("bool");
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
		
	if(DEBUG)
	{
		p_trace("p_bool1", code);
	}

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
		
		default:
		{
			break;
		}
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
		
	if(DEBUG)
	{
		p_trace("p_join", code);
	}


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
			error_parse("join");
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
		
	if(DEBUG)
	{
		p_trace("p_join1", code);
	}


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
		
		default:
		{
			break;
		}
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
		
	if(DEBUG)
	{
		p_trace("p_equality", code);
	}

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
			error_parse("equality");
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
		
	if(DEBUG)
	{
		p_trace("p_equality1", code);
	}

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
		
		default:
		{
			break;
		}
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
		
	if(DEBUG)
	{
		p_trace("p_rel", code);
	}

	switch (code)
	{
		case '<':
		{
			eat('<');
			TreeExpr expr2 = p_expr();
			rel = t_rel_lt(expr1, expr2);
			break;
		}
		
		case '>':
		{
			eat('>');
			TreeExpr expr2 = p_expr();
			rel = t_rel_gt(expr1, expr2);
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
		
	if(DEBUG)
	{
		p_trace("p_expr1", code);
	}


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
			error_parse("expr");
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
		
	if(DEBUG)
	{
		p_trace("p_expr1", code);
	}


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

		default:
		{
			break;
		}
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
		
	if(DEBUG)
	{
		p_trace("p_term", code);
	}


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
		
		default:
		{
			break;
		}
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
		
	if(DEBUG)
	{
		p_trace("p_term1", code);
	}


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
		
		default:
		{
			break;
		}
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
		
	if(DEBUG)
	{
		p_trace("p_unary", code);
	}


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

		default:
		{
			break;
		}
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
		
	if(DEBUG)
	{
		p_trace("p_factor", code);
	}

	switch (code)
	{
		case '(':
		{
			eat('(');
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
			factor = t_factor_num(TypeNew(TYPE_INT), curr()->lexeme);
			eat(TOK_NUM);
			break;
		}

		case TOK_REAL:
		{
			factor = t_factor_real(TypeNew(TYPE_FLOAT), curr()->lexeme);
			eat(TOK_REAL);
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
			error_parse("factor");
			break;
		}
	}

	return factor;
}

static void setTypeArrayInt(TreeType1 type1, Type type, unsigned int dims)
{
	TreeType1 type1_tmp = type1;
	Type type_tmp = type;
	unsigned int length = 0;
	
	do
	{
		length = type1_tmp->u.u_type1.length;
		type_tmp->u.u_int_array.type = TypeNewArray(TYPE_INT_ARRAY, length, dims);
		type_tmp = type_tmp->u.u_int_array.type;
		type1_tmp = type1_tmp->u.u_type1.type1;
	} while(type1_tmp);
}

static void setTypeArrayFloat(TreeType1 type1, Type type, unsigned int dims)
{
	TreeType1 type1_tmp = type1;
	Type type_tmp = type;
	unsigned int length;
	
	do
	{
		length = type1_tmp->u.u_type1.length;
		type_tmp->u.u_int_array.type = TypeNewArray(TYPE_FLOAT_ARRAY, length, dims);
		type_tmp = type_tmp->u.u_float_array.type;
		type1_tmp = type1_tmp->u.u_type1.type1;
	} while(type1_tmp);	
}

static void error_parse(char* str)
{
	fprintf(stderr, "Parsing error, function: %s, line no.: %d, col. no.: %d\n", 
			str, line(), col());
}

static void p_trace(char* func, int code)
{

	fp = WRITTEN ? fopen(FILE_NAME, "a") : fopen(FILE_NAME, "w");

	fprintf(fp, "Entered Function: %10s\tToken Code: %s\n", func, p_code(code));
	fclose(fp);
	
	WRITTEN = WRITTEN ? WRITTEN : TRUE;
}

static char* p_code(int code)
{
	switch(code)
	{
		case TOK_ERROR: 	return "TOK_ERROR";
		case TOK_ID:		return "TOK_ID";
		case TOK_NUM:		return "TOK_NUM";
		case TOK_REAL:		return "TOK_REAL";
		case TOK_BREAK:		return "TOK_BREAK";
		case TOK_WHILE:		return "TOK_WHILE";
		case TOK_AND:		return "TOK_AND";
		case TOK_LTE:		return "TOK_LTE";
		case TOK_OR:		return "TOK_OR";
		case TOK_INT:		return "TOK_INT";
		case TOK_FLOAT:		return "TOK_FLOAT";
		case TOK_IF:		return "TOK_IF";
		case TOK_ELSE:		return "TOK_ELSE";
		case TOK_IF_ELSE:	return "TOK_IF_ELSE";
		case TOK_DO:		return "TOK_DO";
		case TOK_READ:		return "TOK_READ";
		case TOK_WRITE:		return "TOK_WRITE";
		case TOK_EQ:		return "TOK_EQ";
		case TOK_NE:		return "TOK_NE";
		case TOK_GTE:		return "TOK_GTE";
		case TOK_TRUE:		return "TOK_TRUE";
		case TOK_FALSE:		return "TOK_FALSE";
		case TOK_BLOCK:		return "TOK_BLOCK";
		case TOK_DECLS:		return "TOK_DECLS";
		case TOK_DECL:		return "TOK_DECL";
		case TOK_TYPE:		return "TOK_TYPE";
		case TOK_BASIC:		return "TOK_BASIC";
		case TOK_STMT:		return "TOK_STMT";
		case TOK_LOC:		return "TOK_LOC";
		case TOK_BOOL:		return "TOK_BOOL";
		case TOK_JOIN:		return "TOK_JOIN";
		case TOK_EQUALITY:	return "TOK_EQUALITY";
		case TOK_REL:		return "TOK_REL";
		case TOK_EXPR:		return "TOK_EXPR";
		case TOK_TERM:		return "TOK_TERM";
		case TOK_UNARY:		return "TOK_UNARY";
		case TOK_FACTOR:	return "TOK_FACTOR";
		case '<':			return "<";
		case '>':			return ">";
		case '+':			return "+";
		case '-':			return "-";
		case '*':			return "*";
		case '/':			return "/";
		case '&':			return "&";
		case '|':			return "|";
		case '{':			return "{";
		case '}':			return "}";
		case '[':			return "[";
		case ']':			return "]";
		case '(':			return "(";
		case ')':			return ")";
		case ';':			return ";";
		case '=':			return "=";
		default:
		{
			char *str;
			sprintf(str, "Invalid Token Code: %d", code);
			return str;		
		}
	}
}
