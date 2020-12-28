#pragma once
#ifndef Record_h
#define Record_h
#include "Libs.h"
#include "Condition.h"
#include "ColumnsDataFunctor.h"
#include "Field.h"
#include "PairSortFunctor.h"
#include "ConditionsPack.h"

namespace database {
	class Record {
	private:
		std::vector<shared_ptr<Field>> fields;

		std::shared_ptr<ColumnsDataFunctor> columnsFunctor;

		void calTopStackConditions(conditions_list_operator _operator,
			std::stack<bool>& results) const;

		void setIndexPerNameInPairs(const std::vector<std::shared_ptr<Pair>>& pairs);

		int getConditionColumn(Condition& condition) const;

	public:
		Record(const std::vector<std::string>& values,
			const std::vector<std::shared_ptr<Column>>& columns,
			std::shared_ptr<ColumnsDataFunctor>& columnsFunctor);

		Record(std::vector<std::shared_ptr<Pair>>& values,
			const std::vector<std::shared_ptr<Column>>& columns,
			std::shared_ptr<ColumnsDataFunctor>& columnsFunctor);

		~Record();

		int numOfFields() const;

		std::string operator[](int column) const;

		bool checkConditions(const ConditionsPack& pack) const;

		std::shared_ptr<std::vector<std::string>> select(const ConditionsPack& pack);

		std::shared_ptr<std::vector<std::string>> select(std::vector<int> columns,
			const ConditionsPack& pack);

		std::shared_ptr<vector<string>> select(vector<int>& columns);

		void update(std::vector<std::shared_ptr<Pair>>& updates,
			const ConditionsPack& pack);

		int totalSize(bool sizeOnDisk = false) const;
		void print();
	};
}

#endif  // !Record_h