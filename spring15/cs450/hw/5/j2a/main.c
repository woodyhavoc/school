/**
 * Author: Aaron Woods
 */

#include <stdio.h>
#include <stdlib.h>
//#include "parser.h"
#include "tree.h"
#include "bison-parser.h"
#include "yyinput.h"
#include "generator.h"

extern Tree tree;

int main(int argc, char** argv)
{
	yyparse();
	return generate(tree);
}
