/**
 * Author: Aaron Woods
 */

#include <stdlib.h>
#include "type.h"

Type TypeNew(TypeCode code)
{
	Type type = (Type)malloc(sizeof(Type));
	type->code = code;

	return type;
}

Type TypeNewArray(TypeCode code, unsigned int length, unsigned int dims)
{
	Type type = (Type)malloc(sizeof(Type));
	type->code = code;
	
	switch(type->code)
	{
		case TYPE_INT_ARRAY:
		{
			type->u.u_int_array.length = length;
			type->u.u_int_array.dims = dims;
			type->u.u_int_array.type = (Type)malloc(sizeof(Type));
			break;
		}

		case TYPE_FLOAT_ARRAY:
		{
			type->u.u_float_array.length = length;
			type->u.u_float_array.dims = dims;
			type->u.u_float_array.type = (Type)malloc(sizeof(Type));
			break;
		}

		default:
		{
			break;
		}
	}

	return type;
}

void type_or(Type type1, Type type2)
{

}

void type_and(Type type1, Type type2)
{

}
