%option yylineno
%option noyywrap

%x COMMENT

%{
#include "tree.h"
#include "bison-parser.h"
#include "yyinput.h"
#include "strtab.h"
#include <readline/readline.h>

void yyerror(const char* s) {
	fprintf(stderr, "%s\n", s);
}
%}

digit		[0-9]
num		{digit}+
real	{digit}+\.?{digit}+
id		[a-z|A-Z|_]+	

%%

"/*"		BEGIN(COMMENT);
<COMMENT>"*/"	BEGIN(INITIAL);
<COMMENT>.
<COMMENT>\n
"//".*\n

"break" return TOK_BREAK;
"while" return TOK_WHILE;
"&&"	return TOK_AND;
"<="	return TOK_LTE;
"||"	return TOK_OR;
"if"	return TOK_IF;
"else"	return TOK_ELSE;
"do"	return TOK_DO;
"read"	return TOK_READ;
"write"	return TOK_WRITE;
"=="	return TOK_EQ;
"!="	return TOK_NE;
">="	return TOK_GTE;
"int"	return TOK_INT;
"float"	return TOK_FLOAT;
"true"	return TOK_TRUE;
"false"	return TOK_FALSE;
	
{num} {
  yylval.treeNum = strtabget(yytext, yyleng);
  return TOK_NUM;
}

{real} {
	yylval.treeReal = strtabget(yytext, yyleng);
	return TOK_REAL;
}

{id} {
	yylval.treeId = strtabget(yytext, yyleng);
	return TOK_ID;
}

[ \t\n]

. error_scan(*yytext);
