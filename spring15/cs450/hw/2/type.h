#ifndef TYPE_H
#define TYPE_H

enum Type
{
	TYPE_ERR = 1000,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_BOOL,
	TYPE_ID_INT,
	TYPE_ID_FLOAT
};

void type_or(Type type1, Type type2);
void type_and(Type type1, Type type2);

#endif /* TYPE_H */
