#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "generator.h"

// Static function declarations
static void g_program(TreeProg prog);
static void g_block(TreeBlock block);
static void g_decls(TreeDecls decls);
static void g_decl(TreeDecl decl);
static void g_type(TreeType type);
static void g_type1(TreeType1 type1);
static void g_basic(TreeBasic basic);
static void g_stmts(TreeStmts stmts);
static void g_stmt(TreeStmt stmt);
static void g_stmt_assign(TreeStmt stmt);
static void g_stmt_if(TreeStmt stmt);
static void g_stmt_else(TreeStmt stmt);
static void g_stmt_while(TreeStmt stmt);
static void g_stmt_do(TreeStmt stmt);
static void g_stmt_break(TreeStmt stmt);
static void g_stmt_read(TreeStmt stmt);
static void g_stmt_write(TreeStmt stmt);
static void g_loc(TreeLoc loc);
static void g_loc1(TreeLoc1 loc1);
static void g_bool(TreeBool bool);
static void g_bool1(TreeBool1 bool1);
static void g_join(TreeJoin join);
static void g_join1(TreeJoin1 join1);
static void g_equality(TreeEquality equality);
static void g_equality1(TreeEquality1 equality1);
static void g_rel(TreeRel rel);
static void g_expr(TreeExpr expr);
static void g_expr1(TreeExpr1 expr1);
static void g_term(TreeTerm term);
static void g_term1(TreeTerm1 term1);
static void g_unary(TreeUnary unary);
static void g_factor(TreeFactor factor);

static char* gen_label();
static void emit_label(char* label);
static void emit_ins1(char* cmd);
static void emit_ins2(char* cmd, char* op);

int generate(TreeProg prog)
{
	g_program(prog);

	return prog ? 0 : 1;
}

static void g_program(TreeProg prog)
{
	if(!prog)
	{
		return;
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

	switch(block->code)
	{
		case TOK_DECLS:
		{
			g_decls(block->u.u_block.decls);
			g_stmts(block->u.u_block.stmts);
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

	switch(decls->code)
	{
		case TOK_DECL:
		{
			g_decl(decls->u.u_decls.decl);
			g_decls(decls->u.u_decls.decls);
		}
	}
}

static void g_decl(TreeDecl decl)
{
	if(!decl)
	{
		return;
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

static void g_type(TreeType type)
{
	if(!type)
	{
		return;
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

static void g_type1(TreeType1 type1)
{
	if(!type1)
	{
		return;
	}

	switch(type1->code)
	{
		case TOK_NUM:
		{
			// TODO finish this shit
			g_type1(type1->u.u_type1.type1);
			break;
		}

		default:
		{
			break;
		}
	}
}

static void g_basic(TreeBasic basic)
{
	if(!basic)
	{
		return;
	}

	switch(basic->code)
	{
		case TOK_INT:
		{
			// TODO finish this shit
		}

		case TOK_FLOAT:
		{
			// TODO finish this shit
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

		case TOK_ELSE:
		{
			g_stmt_else(stmt);
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
			break;
		}                
  	}
}

static void g_stmt_assign(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	switch(stmt->code)
	{
		case TOK_LOC:
		{
			g_loc(stmt->u.u_assign.loc);
			g_bool(stmt->u.u_assign.bool);
		}

		default:
		{
			ERR("bad assign");
		}
	}
}

static void g_stmt_if(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	switch(stmt->code)
	{
		case TOK_IF:
		{
			g_bool(stmt->u.u_if.bool);
			g_stmt(stmt->u.u_if.stmt);
			break;
		}

		default:
		{
			ERR("bad if");
		}
	}
}

static void g_stmt_else(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	switch(stmt->code)
	{
		case TOK_ELSE:
		{
			g_stmt(stmt->u.u_else.stmt);
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

	Type bool;
  	
	char *ltop = gen_label();
  	char *lend = gen_label();
  	
	emit_label(ltop);
  	
	g_bool(stmt->u.u_while.bool);
  	
	type_or(&bool, &bool);
  	
	emit_ins2("push",lend);
  	emit_ins1("jz");
  	
	g_stmt(stmt->u.u_while.stmt);
  	
	emit_ins2("push",ltop);
  	emit_ins1("jmp");
  	
	emit_label(lend);
}

static void g_stmt_do(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	g_stmt(stmt->u.u_do.stmt);

	Type bool;

	char* ltop = gen_label();
	char* lend = gen_label();

	emit_label(ltop);

	g_bool(stmt->u.u_do.bool);

	type_or(&bool, &bool);

	emit_ins2("push", lend);
	emit_ins1("jz");

	g_stmt(stmt->u.u_do.stmt);

	emit_ins2("push", ltop);
	emit_ins1("jmp");

	emit_label(lend);
}

static void g_stmt_break(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	switch(stmt->code)
	{
		case TOK_BREAK:
		{
			// TODO figure out what needs to go here
		}

		default:
		{
			ERR("bad break");
			break;
		}
	}
}

static void g_stmt_read(TreeStmt stmt)
{
	if(!stmt)
	{
		return;
	}

	switch(stmt->code)
	{
		case TOK_READ:
		{
			g_loc(stmt->u.u_read.loc);
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

	switch(stmt->code)
	{
		case TOK_WRITE:
		{
			g_bool(stmt->u.u_write.bool);
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
static void g_loc(TreeLoc loc)
{
	// TODO Verify this shit
	if(!loc)
	{
		return;
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

static void g_loc1(TreeLoc1 loc1)
{
	if(!loc1)
	{
		return;
	}

	switch(loc1->code)
	{
		case TOK_BOOL:
		{
			g_bool(loc1->u.u_loc1.bool);

			// TODO Handle some logic here

			g_loc1(loc1->u.u_loc1.loc1);
		}

		default:
		{
			break;
		}
	}
	
}

static void g_bool(TreeBool bool)
{
	if(!bool)
	{
		return;
	}

	switch(bool->code)
	{
		case TOK_BOOL:
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

	switch(bool1->code)
	{
		case TOK_OR:
		{
			g_join(bool1->u.u_bool1.join);
			g_bool1(bool1->u.u_bool1.bool1);
			break;
		}

		default:
		{
			ERR("bad bool1");
		}
	}
}

static void g_join(TreeJoin join)
{
	if(!join)
	{
		return;
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

static void g_join1(TreeJoin1 join1)
{
	if(!join1)
	{
		return;
	}

	switch(join1->code)
	{
		case TOK_AND:
		{
			g_equality(join1->u.u_join1.equality);
			g_join1(join1->u.u_join1.join1);
			break;
		}

		default:
		{
			break;
		}
	}
}

static void g_equality(TreeEquality equality)
{
	if(!equality)
	{
		return;
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

static void g_equality1(TreeEquality1 equality1)
{
	if(!equality1)
	{
		return;
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

	switch(rel->code)
	{
		case '<':
		{
			g_expr(rel->u.u_lt.expr1);
			g_expr(rel->u.u_lt.expr2);
			break;
		}

		case TOK_LTE:
		{
			g_expr(rel->u.u_lte.expr1);
			g_expr(rel->u.u_lte.expr2);
			break;
		}

		case TOK_GTE:
		{
			g_expr(rel->u.u_gte.expr1);
			g_expr(rel->u.u_gte.expr2);
			break;
		}

		case '>':
		{
			g_expr(rel->u.u_gt.expr1);
			g_expr(rel->u.u_gt.expr2);
			break;
		}

		case TOK_EXPR:
		{
			g_expr(rel->u.u_expr.expr);
			break;
		}
	}
}

static void g_expr(TreeExpr expr)
{
	if(!expr)
	{
		return;
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

static void g_expr1(TreeExpr1 expr1)
{	
	if(!expr1)
	{
		return;
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

static void g_term(TreeTerm term)
{
	if(!term)
	{
		return;
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

static void g_term1(TreeTerm1 term1)
{
	if(!term1)
	{
		return;
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

static void g_unary(TreeUnary unary)
{
	if(!unary)
	{
		return;
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

static void g_factor(TreeFactor factor)
{
	if(!factor)
	{
		return;
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
			break;
		}

		case TOK_FALSE:
		{
			break;
		}
	}
}

static char* gen_label()
{
	static int labelNum = 0;

	char* label = (char*)malloc((sizeof(char)*10)+sizeof(int));

	sprintf(label, "$label%d:", labelNum);

	return label;
}

static void emit_label(char* label)
{
	printf("%s\n", label);
}

static void emit_ins1(char* cmd)
{
	printf("\t%s\n", cmd);
}

static void emit_ins2(char* cmd, char* op)
{
	printf("\t%s\t%s\n", cmd, op);
}
