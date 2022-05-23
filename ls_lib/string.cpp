


#include "pch.h"
#include "string.h"

void set(char* Dist, const char* Source)
{
	strcpy(Dist, Source);
}

void SetStringSymbol(char* str, char symbol, int id)
{
	int length = strlen(str);
	if (id > length) {
		char* arr = str;
		str = new char[id];
		for (int i = 0; i < id; i++) {
			if (i < length)
				str[i] = arr[i];
			else
				str[i] = ' ';
			if (i == id - 1)
				str[i] = symbol;
		}
	}else
		str[id] = (char)symbol;
}

