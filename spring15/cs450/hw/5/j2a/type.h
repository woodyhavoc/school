#ifndef TYPE_H
#define TYPE_H

typedef enum
{
	TYPE_ERR = 1000,
	TYPE_INT,
	TYPE_INT_ARRAY,
	TYPE_FLOAT,
	TYPE_FLOAT_ARRAY,
	TYPE_BOOL
} TypeCode;

typedef struct type *Type;

struct type
{
	TypeCode code;
		
	union
	{
		struct
		{
			char* val;
		} u_int;

		struct
		{
			unsigned int length;
			unsigned int dims;
			Type type;
		} u_int_array;

		struct
		{
			char* val;
		} u_float;

		struct
		{
			unsigned int length;
			unsigned int dims;
			Type type;
		} u_float_array;

		struct
		{

		} u_bool;
	} u;
};

/**
 * Creates a new Type and gives it a TypeCode
 *
 * @param code the TypeCode to be stored in the Type
 */
Type TypeNew(TypeCode code);

/**
 * Creates a new Type which is an array type
 *
 * @param code the TypeCode to be stored in the Type
 * @param length the number of elements in this dimension
 * @param dims the total number of dimensions
 */
Type TypeNewArray(TypeCode code, unsigned int length, unsigned int dims);

/**
 * Compares the type of two Types
 *
 * @param type1 the first Type to be compared
 * @param type2 the first Type to be compared
 */
void type_or(Type type1, Type type2);

/**
 * Compares the type of two Types
 *
 * @param type1 the first Type to be compared
 * @param type2 the first Type to be compared
 */
void type_and(Type type1, Type type2);

#endif /* TYPE_H */
