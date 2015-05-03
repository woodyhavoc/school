/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_BISON_PARSER_H_INCLUDED
# define YY_YY_BISON_PARSER_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_BREAK = 258,
     TOK_WHILE = 259,
     TOK_AND = 260,
     TOK_LTE = 261,
     TOK_OR = 262,
     TOK_IF = 263,
     TOK_ELSE = 264,
     TOK_DO = 265,
     TOK_READ = 266,
     TOK_WRITE = 267,
     TOK_EQ = 268,
     TOK_NE = 269,
     TOK_GTE = 270,
     TOK_INT = 271,
     TOK_FLOAT = 272,
     TOK_TRUE = 273,
     TOK_FALSE = 274,
     TOK_IF_ELSE = 275,
     TOK_ERROR = 276,
     TOK_ID = 277,
     TOK_NUM = 278,
     TOK_REAL = 279,
     TOK_BLOCK = 280,
     TOK_DECLS = 281,
     TOK_DECL = 282,
     TOK_TYPE = 283,
     TOK_TYPE1 = 284,
     TOK_BASIC = 285,
     TOK_STMT = 286,
     TOK_LOC = 287,
     TOK_LOC1 = 288,
     TOK_BOOL = 289,
     TOK_BOOL1 = 290,
     TOK_JOIN = 291,
     TOK_JOIN1 = 292,
     TOK_EQUALITY = 293,
     TOK_EQUALITY1 = 294,
     TOK_REL = 295,
     TOK_EXPR = 296,
     TOK_EXPR1 = 297,
     TOK_TERM = 298,
     TOK_TERM1 = 299,
     TOK_UNARY = 300,
     TOK_FACTOR = 301
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 19 "bison-parser.y"

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


/* Line 2058 of yacc.c  */
#line 136 "bison-parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_BISON_PARSER_H_INCLUDED  */
