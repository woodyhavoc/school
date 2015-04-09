#include "parser.h"
#include "generator.h"

int main(int argc, char** argv)
{
	return generate(parse());
}
