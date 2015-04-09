/**
 * Author: Aaron Woods
 */

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "generator.h"

int main(int argc, char** argv)
{
	return generate(parse());
}
