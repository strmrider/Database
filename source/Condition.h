#pragma once
#ifndef Condition_h
#define Condition_h
#include "Libs.h"
using namespace std;

namespace database {
	class Condition {
	private:
		int column;

	public:
		Condition(int col);
		int getColumn();
		virtual bool applyCondition(std::string value, data_type type) const = 0;
		virtual bool operator==(
			const std::shared_ptr<Condition>& condition) const = 0;
		virtual void print() = 0;
		virtual std::string getColumnName() = 0;
		virtual std::string getVal() const = 0;
	};

	class ComparisonCondition : public Condition {
	private:
		std::string _value;
		comparison_type compType;
		std::string columnName;
		bool operator==(const std::string value) const;
		bool operator!=(const std::string value) const;
		template <class T>
		bool relativeCmp(const T value1, const T value2) const;

	public:
		std::string getVal() const;
		comparison_type getType();
		ComparisonCondition(int column, std::string value, comparison_type type);

		ComparisonCondition(std::string column, std::string value,
			comparison_type type);

		ComparisonCondition(int column, std::string _column, std::string value,
			comparison_type type);

		bool applyCondition(std::string value, data_type type) const;
		bool operator==(const std::shared_ptr<Condition>& condition) const;
		virtual std::string getColumnName();

		void print() { cout << columnName << "\t" << compType << "\t" << _value; }
	};

	template <class T>
	bool ComparisonCondition::relativeCmp(const T value1, const T value2) const {
		switch (compType) {
		case GREATER:
			return value1 > value2;
		case LESSER:
			return value1 < value2;
		case GREATER_EQUAL:
			return value1 >= value2;
		case LESSER_EQUAL:
			return value1 <= value2;
		default:
			return false;
		}
	}
}
#endif  // !Condition_h