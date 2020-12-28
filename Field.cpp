#include "stdafx.h"
#include "Field.h"
using namespace database;
using namespace std;

Field::Field(data_type _type, std::string _value)
	: type(_type), value(_value) {}

Field::~Field() {}

data_type Field::getType() const { return type; }

std::string Field::getValue() const { return value; }

bool Field::isSameType(const Field& field) { return type == field.getType(); }

void Field::setValue(std::string _value) { value = _value; }

bool Field::operator>(const Field& field) {
	if (isSameType(field)) {
		if (type != STRING) return comparePerType(field.getValue());
	}

	return false;
}

bool Field::operator<(const Field& field) {
	return (isSameType(field) && !(*this > field) && (*this != field));
}

bool Field::operator==(const Field& field) {
	return (isSameType(field) && value == field.getValue());
}

bool Field::operator!=(const Field& field) {
	return (isSameType(field) && value != field.getValue());
}

std::string Field::toString() {
	std::ostringstream ss;
	ss << value;
	return ss.str();
}

bool Field::comparePerType(std::string secondValue) {
	switch (type) {
	case INT:
		return stoi(value) > stoi(secondValue);
	case LONG:
		return stol(value) > stol(secondValue);
	case FLOAT:
		return stof(value) > stof(secondValue);
	default:
		return false;
	}
}

bool Field::checkCondition(const Condition& condition) const {
	if (type != STRING && getValueType(condition.getVal()) != type) {
		string msg = "Value '" + condition.getVal() + "' not compatible with type";
		throw Error(CONDITION_TYPE_ERROR, msg);
	}
	return condition.applyCondition(value, type);
}

int Field::size(bool sizeOnDisk) {
	switch (type) {
	case STRING: {
		int size = value.length();
		if (sizeOnDisk) size += 4;
		return size;
	}
	case INT:
		return 4;
	case LONG:
		return 8;
	case FLOAT:
		return 4;
	default:
		return 0;
	}
}
