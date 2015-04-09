/**
 * Author: Aaron Woods
 */

#include <stdio.h>
#include <stdlib.h>
#include "symtab.h"
#include "tree.h"
#include "type.h"
#include "error.h"
#include "generator.h"
#include "break.h"

#define DEBUG 1
//#define DEBUG 0

#warning need to check types

#warning write the documentation for the generator functions

/**
 * Generates the beginning part of the asm code
 */
static void preProcess();

/**
 * Generates the end part of the asm code
 */
static void postProcess();

/**
 * Generates the code for a TreeProg
 *
 * @param prog the TreeProg to generate code for
 */
static void g_prog(TreeProg prog);

/**
 * Generates the code for a TreeBlock
 *
 * @param block the TreeBlock to generate code for
 */
static void g_block(TreeBlock block);

/**
 * Generates the code for a TreeDecls
 *
 * @param decls the TreeDecls to generate code for
 */
static void g_decls(TreeDecls decls);

/**
 * Generates the code for a TreeDecl
 *
 * @param decl the TreeDecl to generate code for
 */
static void g_decl(TreeDecl decl);

/**
 * Generates the code for a TreeType
 *
 * @param type the TreeType to generate code for
 */
static void g_type(TreeType type);

/**
 * Generates the code for a TreeType1
 *
 * @param type1 the TreeType1 to generate code for
 */
static void g_type1(TreeType1 type1);

/**
 * Generates the code for a TreeBasic
 *
 * @param basic the TreeBasic to generate code for
 */
static void g_basic(TreeBasic basic);

/**
 * Generates the code for a TreeStmts
 *
 * @param stmts the TreeStmts to generate code for
 */
static void g_stmts(TreeStmts stmts);

/**
 * Generates the code for a TreeStmt
 *
 * @param stmt the TreeStmt to generate code for
 */
static void g_stmt(TreeStmt stmt);

/**
 * Generates the assignment code for a TreeStmt
 *
 * @param stmt the TreeStmt to generate assignment code for
 */
static void g_stmt_assign(TreeStmt stmt);

/**
 * Generates the if code for a TreeStmt
 *
 * @param stmt the TreeStmt to generate if code for
 */
static void g_stmt_if(TreeStmt stmt);

/**
 * Generates the if else code for a TreeStmt
 *
 * @param stmt the TreeStmt to generate if else code for
 */
static void g_stmt_if_else(TreeStmt stmt);

/**
 * Generates the while code for a TreeStmt
 *
 * @param stmt the TreeStmt to generate while code for
 */
static void g_stmt_while(TreeStmt stmt);

/**
 * Generates the do while code for a TreeStmt
 *
 * @param stmt the TreeStmt to generate do while code for
 */
static void g_stmt_do(TreeStmt stmt);

/**
 * Generates the break code for a TreeStmt
 *
 * @param stmt the TreeStmt to generate break code for
 */
static void g_stmt_break(TreeStmt stmt);

/**
 * Generates the block code for a TreeStmt
 *
 * @param stmt the TreeStmt to generate block code for
 */
static void g_stmt_block(TreeStmt stmt);

/**
 * Generates the read code for a TreeStmt
 *
 * @param stmt the TreeStmt to generate the read code for
 */
static void g_stmt_read(TreeStmt stmt);

/**
 * Generates the write code for a TreeStmt
 *
 * @param stmt the TreeStmt to generate the write code for
 */
static void g_stmt_write(TreeStmt stmt);

/**
 * Generates the code for a TreeLoc
 *
 * @param loc the TreeLoc to generate code for
 */
static void g_loc(TreeLoc loc);

/**
 * Generates the code for a TreeLoc1
 *
 * @param loc1 the TreeLoc1 to generate code for
 */
static void g_loc1(TreeLoc1 loc1);

/**
 * Generates the code for a TreeBool
 *
 * @param bool the TreeBool to generate code for
 */
static void g_bool(TreeBool bool);

/**
 * Generates the code for a TreeBool1
 *
 * @param bool1 the TreeBool1 to generate code for
 */
static void g_bool1(TreeBool1 bool1);

/**
 * Generates the code for a TreeJoin
 *
 * @param join the TreeJoin to generate code for
 */
static void g_join(TreeJoin join);

/**
 * Generates the code for a TreeJoin1
 *
 * @param join1 the TreeJoin1 to generate code for
 */
static void g_join1(TreeJoin1 join1);

/**
 * Generates the code for a TreeEquality
 *
 * @param equality the TreeEquality to generate code for
 */
static void g_equality(TreeEquality equality);

/**
 * Generatetys the code for a TreeEquality1
 *
 * @param equality1 the TreeEquality1 to generate code for
 */
static void g_equality1(TreeEquality1 equality1);

/**
 * Generates the code for a TreeRel
 *
 * @param rel the TreeRel to generate code for
 */
static void g_rel(TreeRel rel);

/**
 * Generates the code for a TreeExpr
 *
 * @param expr the TreeExpr to generate code for
 */
static void g_expr(TreeExpr expr);

/**
 * Generates the code for a TreeExpr1
 *
 * @param expr1 the TreeExpr1 to generate code for
 */
static void g_expr1(TreeExpr1 expr1);

/**
 * Generates the code for a TreeTerm
 *
 * @param term the TreeTerm to generate code for
 */
static void g_term(TreeTerm term);

/**
 * Generates the code for a TreeTerm1
 *
 * @param term1 the TreeTerm1 to generate code for
 */
static void g_term1(TreeTerm1 term1);

/**
 * Generates the code for a TreeUnary
 *
 * @param unary the TreeUnary to generate code for
 */
static void g_unary(TreeUnary unary);

/**
 * Generates the code for a TreeFactor
 *
 * @param factor the TreeFactor to generate code for
 */
static void g_factor(TreeFactor factor);

// ASM code generation functions
/**
 * Generates a label that can be used as a jump location
 * in the asm code.  The function maintains a static
 * integer varaible to number the generated labels.
 */
static char* gen_label();

/**
 * Emits a label in a special format so it can
 * be understood by the asm machine.
 *
 * @param label the label to be emitted into the asm code
 */
static void emit_label(char* label);

/**
 * Emits a single command that can be understood by
 * the asm machine.
 *
 * @param cmd the single asm command to be emitted
 */
static void emit_ins1(char* cmd);

/**
 * Emits a two-part command that can be understood by
 * the asm machine.
 *
 * @param cmd the first of the two-part command to be emitted
 * @param op the second of the two-part command to be emitted
 */
static void emit_ins2(char* cmd, char* op);

// Helper functions
/**
 * Inserts an asm comment into the asm code.
 * 
 * @param comment the comment to be inserted into the
 * 				  asm code
 */
static void comment(char* comment);

/**
 * This function is used to make a generation stack
 * trace that is written to a file.
 *
 * @param func that last function that was called
 * @param code the current token code
 */
static void g_trace(char* func, int code);

/**
 * This function acccepts an integer value which
 * represents a token code, and returns a string
 * representation of the code.
 *
 * @param code the code to be used to return a meaningful
 * 			   string
 *
 * @return a string representation of the token code
 */
static char* g_code(int code);

// Global variables
static FILE* fp;

int generate(TreeProg prog)
{
	if(!prog)
	{
		return -1;
	}

	fp = fopen("generatorTrace.log", "w");

	if(DEBUG)
	{
		g_trace("g_generate", prog->code);
	}

	preProcess();
		
	g_prog(prog);

	postProcess();

	return prog ? 0 : 1;
}

static void preProcess()
{
	int maxDepth = SymTabMaxDepth();

	comment("program begin execution");

	printf("\tpush\t$start\n");
	printf("\tjmp\n");
	printf("$display:\t0\n");

	while(maxDepth-- > 0)
	{
		printf("\t\t0\n");
	}

	printf("$start:\n");
}

static void postProcess()
{
	comment("program termination");

	printf("\tstop\n");
	printf("\tend\n");
}

static void g_prog(TreeProg prog)
{
	if(!prog)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_prog", prog->code);
	}
	
	switch(prog->code)
	{
		case TOK_BLOCK:
		{
			g_block(prog->u.u_block.block);
			break;
		}

		default:
		{
			ERR("bad program");
			break;
		}	
	}
}

static void g_block(TreeBlock block)
{
	if(!block)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_block", block->code);
	}

	switch(block->code)
	{
		case TOK_DECLS:
		{
			
			char* blockNum = (char*)malloc(sizeof(char)*128 + sizeof(int));
			sprintf(blockNum, "%d", block->depth);

			emit_ins2("push", "$display");
			emit_ins1(blockNum);
			emit_ins1("add");
			emit_ins2("push", "$sp");
			emit_ins1("ld");
			emit_ins1("st");

			g_decls(block->u.u_block.decls);
			g_stmts(block->u.u_block.stmts);
			
			emit_ins2("push", "$display");
			emit_ins1(blockNum);
			emit_ins1("add");
			emit_ins2("push", "$sp");
			emit_ins1("ld");
			emit_ins1("st");
			
			break;
		}

		default:
		{
			ERR("bad block");
			break;
		}
	}
}

static void g_decls(TreeDecls decls)
{
	if(!decls)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_decls", decls->code);
	}

	switch(decls->code)
	{
		case TOK_DECL:
		{
			g_decl(decls->u.u_decls.decl);
			g_decls(decls->u.u_decls.decls);
			break;
		}

		default:
		{
			break;
		}
	}
}

#warning complete implementation of g_decl
static void g_decl(TreeDecl decl)
{
	if(!decl)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_decl", decl->code);	
	}

	switch(decl->code)
	{
		case TOK_TYPE:
		{
			g_type(decl->u.u_decl.type);
			break;
		}

		default:
		{
			ERR("bad decl");
			break;
		}
	}
}

#warning complete implementation of g_type
static void g_type(TreeType type)
{
	if(!type)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_type", type->code);
	}

	switch(type->code)
	{
		case TOK_BASIC:
		{
			g_basic(type->u.u_type.basic);
			g_type1(type->u.u_type.type1);
			break;
		}

		default:
		{
			ERR("bad type");
			break;
		}
	}
}

#warning complete implementation of g_type1
static void g_type1(TreeType1 type1)
{
	if(!type1)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_type1", type1->code);
	}

	switch(type1->code)
	{
		case TOK_NUM:
		{
			g_type1(type1->u.u_type1.type1);
			break;
		}

		default:
		{
			break;
		}
	}
}

#warning complete implemenation of g_basic
static void g_basic(TreeBasic basic)
{
	if(!basic)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_basic", basic->code);
	}

	switch(basic->code)
	{
		case TOK_INT:
		{
			// TODO finish this shit
			break;
		}

		case TOK_FLOAT:
		{
			// TODO finish this shit
			break;
		}	

		default:
		{
			ERR("bad basic");
			break;
		}
	}
}

static void g_stmts(TreeStmts stmts)
{
	if(!stmts)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_stmts", stmts->code);
	}

	switch(stmts->code)
	{
		case TOK_STMT:
		{
			g_stmt(stmts->u.u_stmts.stmt);
			g_stmts(stmts->u.u_stmts.stmts);
		}

		default:
		{
			break;
		}
	}
}

static void g_stmt(TreeStmt stmt) 
{
  	if(!stmt)
  	{
	  	return;
  	}
	
	if(DEBUG)
	{
		g_trace("g_stmt", stmt->code);
	}

  	switch (stmt->code)
  	{
   	 	case TOK_ID:
		{
			g_stmt_assign(stmt);
			break;
		}            

		case TOK_IF:
		{
			g_stmt_if(stmt);
			break;
		}

		case TOK_IF_ELSE:
		{
			g_stmt_if_else(stmt);
			break;
		}

    	case TOK_WHILE:
		{
			g_stmt_while(stmt);
			break;
		}
		
		case TOK_DO:
		{
			g_stmt_do(stmt);
			break;
		}

    	case TOK_BREAK:
		{
			g_stmt_break(stmt);
			break;
		}
		
		case TOK_BLOCK:
		{
			g_stmt_block(stmt);
			break;
		}
		
		case TOK_READ:
		{
			g_stmt_read(stmt);
			break;
		}

		case TOK_WRITE:
		{
			g_stmt_write(stmt);
			break;
		}

    	default:
		{
			ERR("bad stmt");
			emit_ins1("error");
			break;
		}                
  	}
}

#warning complete implementation of g_stmt_assign
static void g_stmt_assign(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_stmt_assign", stmt->code);
	}

	switch(stmt->code)
	{
		case TOK_ID:
		{
			g_loc(stmt->u.u_assign.loc);
			g_bool(stmt->u.u_assign.bool);
			break;
		}

		default:
		{
			ERR("bad assign");
			break;
		}
	}
}

static void g_stmt_if(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_stmt_if", stmt->code);
	}
	
	switch(stmt->code)
	{
		case TOK_IF:
		{
			g_bool(stmt->u.u_if.bool);

			comment("generating code for if statement");
			
			char* done = gen_label();
			
			emit_ins2("push", done);
			emit_ins1("jz");
			
			g_stmt(stmt->u.u_if.stmt);
			
			emit_label(done);

			break;
		}

		default:
		{
			ERR("bad if");
		}
	}
}

static void g_stmt_if_else(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_stmt_if_else", stmt->code);
	}

	switch(stmt->code)
	{
		case TOK_IF_ELSE:
		{
			g_bool(stmt->u.u_if_else.bool);

			comment("generating code for if else statement");

			char* done = gen_label();
			char* elseLab = gen_label();

			emit_ins2("push", elseLab);
			emit_ins1("jz");

			g_stmt(stmt->u.u_if_else.ifStmt);

			emit_ins2("push", done);
			emit_ins1("jmp");
			emit_label(elseLab);

			g_stmt(stmt->u.u_if_else.elseStmt);

			emit_label(done);

			break;
		}

		default:
		{
			ERR("bad else");
			break;
		}
	}
}

static void g_stmt_while(TreeStmt stmt) 
{
	if(!stmt)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_stmt_while", stmt->code);
	}

	Type bool;
  	
	comment("generating while loop code");
	
	char* top = gen_label();
  	char* end = gen_label();
  
  	breakPush(end);
  	
	emit_label(top);
  	
	g_bool(stmt->u.u_while.bool);
  	
	type_or(bool, bool);
  	
	emit_ins2("push",end);
  	emit_ins1("jz");
  	
	g_stmt(stmt->u.u_while.stmt);
  	
	emit_ins2("push",top);
  	emit_ins1("jmp");
  	
	breakPop();

	emit_label(end);
}

static void g_stmt_do(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_stmt_do", stmt->code);
	}

	char* top = gen_label();
	char* end = gen_label();

	breakPush(end);
	
	emit_label(top);

	g_stmt(stmt->u.u_do.stmt);
	
	g_bool(stmt->u.u_do.bool);
	
	emit_ins2("push", end);
	emit_ins1("jz");
	emit_ins2("push", top);
	emit_ins1("jmp");

	breakPop();
		
	emit_label(end);
}

static void g_stmt_break(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_stmt_break", stmt->code);
	}

	switch(stmt->code)
	{
		case TOK_BREAK:
		{
			comment("generating break statement");
			emit_ins2("push", breakCurr());
			emit_ins1("jmp");
			break;
		}

		default:
		{
			ERR("bad break");
			break;
		}
	}
}

static void g_stmt_block(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_stmt_block", stmt->code);
	}

	switch(stmt->code)
	{
		case TOK_BLOCK:
		{
			g_block(stmt->u.u_block.block);
			break;
		}

		default:
		{
			ERR("bad g_stmt_block");
		}
	}
}

static void g_stmt_read(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_stmt_read", stmt->code);
	}

	switch(stmt->code)
	{
		case TOK_READ:
		{
			g_loc(stmt->u.u_read.loc);
			emit_ins1("rd");
			break;
		}

		default:
		{
			ERR("bad read");
			break;
		}
	}
}

static void g_stmt_write(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_stmt_write", stmt->code);
	}

	switch(stmt->code)
	{
		case TOK_WRITE:
		{
			g_bool(stmt->u.u_write.bool);
			emit_ins1("wr");
			break;
		}

		default:
		{
			ERR("bad write");
			break;
		}
	}
}

/**
 * This has an ID, so likely additional logic will need
 * to be implemented
 * 
 */
#warning complete implementation of g_loc
static void g_loc(TreeLoc loc)
{
	// TODO Verify this shit
	if(!loc)
	{
		return;
	}
	
	if(DEBUG)
	{
		g_trace("g_loc", loc->code);
	}

	switch(loc->code)
	{
		case TOK_ID:
		{
			// TODO logic for ID
			g_loc1(loc->u.u_loc.loc1);
			break;
		}

		default:
		{
			ERR("bad loc");
			break;
		}
	}

}

#warning complete implementation of g_loc1
static void g_loc1(TreeLoc1 loc1)
{
	if(!loc1)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_loc1", loc1->code);
	}

	switch(loc1->code)
	{
		case TOK_BOOL:
		{
			g_bool(loc1->u.u_loc1.bool);
			g_loc1(loc1->u.u_loc1.loc1);
		}

		default:
		{
			break;
		}
	}
	
}

#warning complete implementation of g_bool
static void g_bool(TreeBool bool)
{
	if(!bool)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_bool", bool->code);
	}
	
	switch(bool->code)
	{
		case TOK_JOIN:
		{
			g_join(bool->u.u_bool.join);
			g_bool1(bool->u.u_bool.bool1);
			break;
		}

		default:
		{
			ERR("bad bool");
			break;
		}
	}
}

static void g_bool1(TreeBool1 bool1)
{
	if(!bool1)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_bool1", bool1->code);
	}

	switch(bool1->code)
	{
		case TOK_OR:
		{
			g_join(bool1->u.u_bool1.join);
			
			comment("generating code for ||");

			char* t = gen_label();
			char* f = gen_label();
			char* done = gen_label();

			emit_ins1("sub");
			emit_ins2("push", f);
			emit_ins1("jz");
			emit_ins2("push", t);
			emit_ins1("jmp");
			emit_label(f);
			emit_ins2("push", "0");
			emit_ins2("push", done);
			emit_ins1("jmp");
			emit_label(t);
			emit_ins2("push", "1");
			emit_ins2("push", done);
			emit_ins1("jmp");
			emit_label(done);

			g_bool1(bool1->u.u_bool1.bool1);
			break;
		}

		default:
		{
			ERR("bad bool1");
		}
	}
}

#warning complete implementation of g_join
static void g_join(TreeJoin join)
{
	if(!join)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_join", join->code);
	}

	switch(join->code)
	{
		case TOK_EQUALITY:
		{
			g_equality(join->u.u_join.equality);
			g_join1(join->u.u_join.join1);
			break;
		}

		default:
		{
			ERR("bad join");
			break;
		}
	}
}

#warning complete implementation of g_join1
static void g_join1(TreeJoin1 join1)
{
	if(!join1)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_join1", join1->code);
	}

	switch(join1->code)
	{
		case TOK_AND:
		{
			g_equality(join1->u.u_join1.equality);
			
			comment("generating code for &&");
			
			char* t = gen_label();
			char* f = gen_label();
			char* done = gen_label();

			emit_ins1("sub");
			emit_ins2("push", t);
			emit_ins1("jz");
			emit_ins2("push", f);
			emit_ins1("jmp");
			emit_label(t);
			emit_ins2("push", "1");
			emit_ins2("push", done);
			emit_ins1("jmp");
			emit_label(f);
			emit_ins2("push", "0");
			emit_ins2("push", done);
			emit_ins1("jmp");
			emit_label(done);
			g_join1(join1->u.u_join1.join1);
			break;
		}

		default:
		{
			break;
		}
	}
}

#warning complete implementation of g_equality
static void g_equality(TreeEquality equality)
{
	if(!equality)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_equality", equality->code);
	}

	switch(equality->code)
	{
		case TOK_REL:
		{
			g_rel(equality->u.u_equality.rel);
			g_equality1(equality->u.u_equality.equality1);
			break;
		}

		default:
		{
			ERR("bad equality");
		}
	}
}

#warning complete implementation of g_equality1
static void g_equality1(TreeEquality1 equality1)
{
	if(!equality1)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_equality1", equality1->code);
	}

	switch(equality1->code)
	{
		case TOK_EQ:
		{
			g_rel(equality1->u.u_eq.rel);
			g_equality1(equality1->u.u_eq.equality1);
			break;
		}

		case TOK_NE:
		{
			g_rel(equality1->u.u_ne.rel);
			g_equality1(equality1->u.u_ne.equality1);
			break;
		}

		default:
		{
			break;
		}
	}
}

static void g_rel(TreeRel rel)
{
	if(!rel)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_rel", rel->code);
	}

	switch(rel->code)
	{
		case '<':
		{
			g_expr(rel->u.u_lt.expr1);
			g_expr(rel->u.u_lt.expr2);
		
			comment("generating code for < operator");
				
			char* t = gen_label();
			char* f = gen_label();
			char* done = gen_label();

			emit_ins1("sub");
			emit_ins2("push", t);
			emit_ins1("jn");
			emit_ins2("push", f);
			emit_ins1("jmp");
			emit_label(t);
			emit_ins2("push", "1");
			emit_ins2("push", done);
			emit_ins1("jmp");
			emit_label(f);
			emit_ins2("push", "0");
			emit_ins2("push", done);
			emit_ins1("jmp");
			emit_label(done);

			break;
		}

		case TOK_LTE:
		{
			g_expr(rel->u.u_lte.expr1);
			g_expr(rel->u.u_lte.expr2);
		
			comment("generating code for <= operator");
			
			char* t = gen_label();
			char* f = gen_label();
			char* done = gen_label();

			emit_ins1("sub");
			emit_ins2("push", f);
			emit_ins1("jp");
			emit_ins2("push", t);
			emit_ins1("jmp");
			emit_label(f);
			emit_ins2("push", "0");
			emit_ins2("push", done);
			emit_ins1("jmp");
			emit_label(t);
			emit_ins2("push", "1");
			emit_ins2("push", done);
			emit_ins1("jmp");
			emit_label(done);

			break;
		}

		case TOK_GTE:
		{
			g_expr(rel->u.u_gte.expr1);
			g_expr(rel->u.u_gte.expr2);

			comment("generating code for >= operator");

			char* t = gen_label();
			char* f = gen_label();
			char* done = gen_label();

			emit_ins1("sub");
			emit_ins2("push", f);
			emit_ins1("jn");
			emit_ins2("push", t);
			emit_ins1("jmp");
			emit_label(f);
			emit_ins2("push", "0");
			emit_ins2("push", done);
			emit_ins1("jmp");
			emit_label(t);
			emit_ins2("push", "1");
			emit_ins2("push", done);
			emit_ins1("jmp");
			emit_label(done);

			break;
		}

		case '>':
		{
			g_expr(rel->u.u_gt.expr1);
			g_expr(rel->u.u_gt.expr2);

			comment("generating code for > operator");

			char* t = gen_label();
			char* f = gen_label();
			char* done = gen_label();

			emit_ins1("sub");
			emit_ins2("push", t);
			emit_ins1("jp");
			emit_ins2("push", f);
			emit_ins1("jmp");
			emit_label(t);
			emit_ins2("push", "1");
			emit_ins2("push", done);
			emit_ins1("jmp");
			emit_label(f);
			emit_ins2("push", "0");
			emit_ins2("push", done);
			emit_ins1("jmp");
			emit_label(done);

			break;
		}

		case TOK_EXPR:
		{
			g_expr(rel->u.u_expr.expr);
			break;
		}
	}
}

#warning complete implementation of g_expr
static void g_expr(TreeExpr expr)
{
	if(!expr)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_expr", expr->code);
	}

	switch(expr->code)
	{
		case TOK_TERM:
		{
			g_term(expr->u.u_expr.term);
			g_expr1(expr->u.u_expr.expr1);
			break;
		}

		default:
		{
			ERR("bad expr");
			break;
		}
	}
}

#warning complete implemenation of g_expr
static void g_expr1(TreeExpr1 expr1)
{	
	if(!expr1)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_expr1", expr1->code);
	}

	switch(expr1->code)
	{
		case '+':
		{
			g_term(expr1->u.u_plus.term);
			g_expr1(expr1->u.u_plus.expr1);
			break;
		}

		case '-':
		{
			g_term(expr1->u.u_minus.term);
			g_expr1(expr1->u.u_minus.expr1);
			break;
		}

		default:
		{
			break;
		}
	}
}

#warning complete implemenation of g_term
static void g_term(TreeTerm term)
{
	if(!term)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_term", term->code);
	}

	switch(term->code)
	{
		case TOK_UNARY:
		{
			g_unary(term->u.u_term.unary);
			g_term1(term->u.u_term.term1);
			break;
		}

		default:
		{
			ERR("bad term");
			break;
		}
	}
}

#warning complete implementation of g_term1
static void g_term1(TreeTerm1 term1)
{
	if(!term1)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_term1", term1->code);
	}

	switch(term1->code)
	{
		case '*':
		{
			g_unary(term1->u.u_mul.unary);
			g_term1(term1->u.u_mul.term1);
			break;
		}

		case '/':
		{
			g_unary(term1->u.u_div.unary);
			g_term1(term1->u.u_div.term1);
			break;
		}

		default:
		{
			break;
		}
	}
}

#warning complete implementation of g_unary
static void g_unary(TreeUnary unary)
{
	if(!unary)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_unary", unary->code);
	}

	switch(unary->code)
	{
		case '!':
		{
			g_unary(unary->u.u_not.unary);
			break;
		}

		case '-':
		{
			g_unary(unary->u.u_neg.unary);
			break;
		}

		case TOK_FACTOR:
		{
			g_factor(unary->u.u_factor.factor);
			break;
		}

		default:
		{
			ERR("bad unary");
		}
	}
}

#warning complete implementation of g_factor
static void g_factor(TreeFactor factor)
{
	if(!factor)
	{
		return;
	}

	if(DEBUG)
	{
		g_trace("g_factor", factor->code);
	}

	switch(factor->code)
	{
		case TOK_BOOL:
		{
			g_bool(factor->u.u_bool.bool);
			break;
		}

		case TOK_LOC:
		{
			g_loc(factor->u.u_loc.loc);
			break;
		}

		case TOK_NUM:
		{
			break;
		}

		case TOK_REAL:
		{
			break;
		}

		case TOK_TRUE:
		{
			emit_ins2("push", "1");
			break;
		}

		case TOK_FALSE:
		{
			emit_ins2("push", "0");
			break;
		}

		default:
		{
			ERR("bad factor");
		}
	}
}

static char* gen_label()
{
	static int labelNum = 0;

	char* label = (char*)malloc((sizeof(char)*10)+sizeof(int));

	sprintf(label, "$label%d", labelNum);
	
	labelNum++;

	return label;
}

static void emit_label(char* label)
{
	printf("\n%s:\n", label);
}

static void emit_ins1(char* cmd)
{
	printf("\t%s\n", cmd);
}

static void emit_ins2(char* cmd, char* op)
{
	printf("\t%s\t%s\n", cmd, op);
}

static void comment(char* comment)
{
	printf("\n# %s\n", comment);
}

static void g_trace(char* func, int code)
{
	fprintf(fp, "Entered Function: %10s\tToken Code: %s\n", func, g_code(code));
}

static char* g_code(int code)
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
		default:			return "Invalid Token Code";
	}
}
