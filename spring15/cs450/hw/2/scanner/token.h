#ifndef TOKEN_H
#define TOKEN_H

typedef enum 
{
  TOK_ERROR=256,
  TOK_ID,
  TOK_NUM,
  TOK_REAL,
  TOK_BREAK,
  TOK_WHILE,
  TOK_AND, // &&
  TOK_LTE, // <=
  TOK_OR, // ||
  // Additions
  TOK_INT,
  TOK_FLOAT,
  TOK_IF,
  TOK_ELSE,
  TOK_DO,
  TOK_READ,
  TOK_WRITE,
  TOK_EQ, // ==
  TOK_NE, // !=
  TOK_GTE, // >=
  TOK_TRUE,
  TOK_FALSE,
  // Token codes for non-terminals to be used
  // by the generator
  TOK_BLOCK,
  TOK_DECLS,
  TOK_DECL,
  TOK_TYPE,
  TOK_BASIC,
  TOK_STMT,
  TOK_LOC,
  TOK_BOOL,
  TOK_JOIN,
  TOK_EQUALITY,
  TOK_REL,
  TOK_EXPR,
  TOK_TERM,
  TOK_UNARY,
  TOK_FACTOR
} TokenCode;

typedef struct Token 
{
  char *lexeme;
  TokenCode code;
} *Token;

#endif
