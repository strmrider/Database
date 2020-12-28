#include "stdafx.h"
#include "Utilities.h"
using namespace std;
using namespace database;

data_type database::toType(string type) {
	if (type == "str")
		return STRING;
	else if (type == "int")
		return INT;
	else if (type == "long")
		return LONG;
	else if (type == "float")
		return FLOAT;
	else
		return VOID;
}

data_type database::getValueType(const string& value) {
	short NumOfFloatPoint = 0;
	short NumOfMinus = 0;
	int size = value.length();
	for (int i = 0; i < value.length(); i++) {
		if (value[i] == '.') {
			if (NumOfFloatPoint > 1) return STRING;
			NumOfFloatPoint++;
		}
		else if (value[i] == '-') {
			if (NumOfMinus > 1) return STRING;
			NumOfMinus++;
		}
		else if (value[i] < 48 || value[i] > 57)
			return STRING;
	}
	if (((NumOfFloatPoint >= 1) || (NumOfMinus > 1)) && size == 1)
		return STRING;
	else if (NumOfFloatPoint == 1) {
		return FLOAT;
	}
	else
		return INT;
}