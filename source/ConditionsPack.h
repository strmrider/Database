#pragma once
#ifndef ConditionsPack_h
#define ConditionsPack_h
#include "Libs.h"
#include "Condition.h"

namespace database {
	class Container {
	public:
		std::shared_ptr<Condition> condition;
		conditions_list_operator _operator;
		condition_pack_element_type type;
		Container(const std::shared_ptr<Condition> condition);
		Container(conditions_list_operator _operator);
		bool operator==(const Container& container);
		bool operator!=(const Container& container);
	};

	class ConditionsPack {
	public:
		std::list<std::shared_ptr<Container>> list;
		ConditionsPack();
		void operator=(const ConditionsPack& pack);
		bool operator==(const ConditionsPack& pack);
	};
}
#endif  // !ConditionsPack_h
