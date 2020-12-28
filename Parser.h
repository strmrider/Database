#pragma once
#ifndef Parser_h
#define Parser_h
#include "Libs.h"
#include "Condition.h"
#include "ConditionsPack.h"
#include "ConditionsQuery.h"
#include "Pair.h"
#include "Column.h"
#include "Query.h"
#include "Utilities.h"

namespace database {
	class Parser {
	private:
		std::shared_ptr<vector<string>> splitQuery(const std::string& query);

		std::shared_ptr<Condition> parseCondition(const std::string& str);

		std::shared_ptr<ConditionsPack> parseConditionsPack(const string& conditions);

		std::shared_ptr<std::vector<std::string>> splitColumns(
			const std::string& str);

		std::shared_ptr<Pair> parsePair(const string& pair);

		std::shared_ptr<std::vector<std::shared_ptr<Pair>>> parsePairs(
			const std::vector<std::string>& tokens);

		std::shared_ptr<std::vector<std::shared_ptr<Column>>> parseColumns(
			std::shared_ptr<std::vector<std::string>> names,
			std::shared_ptr<std::vector<std::string>> types);

	public:
		Parser();
		std::shared_ptr<Query> parse(const std::string& query);
	};
}
#endif  // !Parser_h
