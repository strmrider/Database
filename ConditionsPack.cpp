#include "stdafx.h"
#include "ConditionsPack.h"
using namespace std;
using namespace database;

Container::Container(const shared_ptr<Condition> condition) {
	this->condition = condition;
	type = CONDITION;
}
Container::Container(conditions_list_operator _operator) {
	this->_operator = _operator;
	type = OPERATOR;
}

bool Container::operator==(const Container& container) {
	if (type == container.type) {
		if (type == CONDITION)
			return condition == container.condition;
		else
			return condition == container.condition;
	}
	return false;
}

bool Container::operator!=(const Container& container) {
	return !((*this) == container);
}

/****************************************/

ConditionsPack::ConditionsPack() {}

void ConditionsPack::operator=(const ConditionsPack& pack) {
	std::list<shared_ptr<Container>>::const_iterator it;
	std::shared_ptr<Container> container;
	for (auto it = pack.list.begin(); it != pack.list.end(); ++it) {
		container = *it;
		list.push_back(container);
	}
}

bool ConditionsPack::operator==(const ConditionsPack& pack) {
	if (list.size() == pack.list.size()) {
		std::list<shared_ptr<Container>>::const_iterator it;
		std::list<shared_ptr<Container>>::const_iterator it2;
		std::shared_ptr<Container> container1;
		std::shared_ptr<Container> container2;

		it2 = (pack.list).begin();
		for (auto it = list.begin(); it != list.end(); ++it, ++it2) {
			container1 = *it;
			container2 = *it2;
			if (container1 != container2) return false;
		}
	}
	return true;
}
