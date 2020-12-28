#include "stdafx.h"
#include "Condition.h"
using namespace std;
using namespace database;

Condition::Condition(int col) : column(col) {};

int Condition::getColumn() { return column; }

/****************************************/

ComparisonCondition::ComparisonCondition(int column, std::string value,
	comparison_type type)
	: Condition(column) {
	_value = value;
	compType = type;
};

ComparisonCondition::ComparisonCondition(string column, std::string value,
	comparison_type type)
	: Condition(-1) {
	columnName = column;
	_value = value;
	compType = type;
}

ComparisonCondition::ComparisonCondition(int column, std::string _column,
	std::string value,
	comparison_type type)
	: Condition(column) {
	columnName = _column;
	_value = value;
	compType = type;
}

string ComparisonCondition::getVal() const { return _value; };

comparison_type ComparisonCondition::getType() { return compType; }

string ComparisonCondition::getColumnName() { return columnName; }

bool ComparisonCondition::operator==(const std::string value) const {
	return _value == value;
}

bool ComparisonCondition::operator!=(const std::string value) const {
	return _value != value;
}

bool ComparisonCondition::applyCondition(std::string value,
	data_type type) const {
	if (compType == EQUAL)
		return (*this == value);
	else if (compType == NOT_EQUAL)
		return (*this != value);
	else if (type == STRING) {
		int res = strncmp(value.c_str(), _value.c_str(), value.length());
		if (compType == GREATER)
			return res >= 0;
		else if (compType == LESSER)
			return res <= 0;
		else if (compType == GREATER_EQUAL || compType == LESSER_EQUAL)
			return res == 0;
	}
	else {
		switch (type) {
		case INT:
			return relativeCmp<int>(stoi(value), stoi(_value));
		case LONG:
			return relativeCmp<int>(stol(value), stol(_value));
		case FLOAT:
			return relativeCmp<int>(stof(value), stof(_value));
		default:
			return false;
		}
	}
}

bool ComparisonCondition::operator==(
	const std::shared_ptr<Condition>& condition) const {
	ComparisonCondition* secondCondition =
		dynamic_cast<ComparisonCondition*>(condition.get());
	return (compType == secondCondition->getType() &&
		_value == secondCondition->getVal());
}