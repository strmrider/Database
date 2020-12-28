#pragma once
#ifndef ConditionsQuery_h
#define ConditionsQuery_h
#include "Libs.h"
#include "Condition.h"
#include "ConditionsPack.h"

namespace database
{
	class ConditionsQuery
	{
	private:
		std::stack<conditions_list_operator> operators;
		std::shared_ptr<ConditionsPack> pack;
		void pushTopOperator();
	public:
		ConditionsQuery();
		void condition(const std::shared_ptr<Condition>& condition);
		void setOperator(conditions_list_operator _operator);
		void parentheses(bool open);
		void done();
		void reset();
		std::shared_ptr<ConditionsPack> get();
	};
}
#endif // !ConditionsQuery_h
