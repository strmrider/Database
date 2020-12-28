#pragma once
#ifndef FIELD_H
#define FIELD_H
#include "Libs.h"
#include "Utilities.h"
#include "Error.h"
#include "Pair.h"
#include "Condition.h"

namespace database {
	class Field {
		data_type type;
		std::string value;

		bool comparePerType(std::string value);

	public:
		Field() {}
		Field(data_type _type, std::string _value);
		~Field();

		data_type getType() const;
		std::string getValue() const;
		void setValue(std::string _value);

		bool isSameType(const Field& field);
		bool operator>(const Field& field);
		bool operator<(const Field& field);
		bool operator==(const Field& field);
		bool operator!=(const Field& field);

		std::string toString();
		int size(bool sizeOnDisk = false);
		bool checkCondition(const Condition& condition) const;
	};
}

#endif  // !FIELD_H
