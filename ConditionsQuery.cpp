#include "stdafx.h"
#include "ConditionsQuery.h"
using namespace std;
using namespace database;

ConditionsQuery::ConditionsQuery() {
	pack = shared_ptr<ConditionsPack>(new ConditionsPack());
}

void ConditionsQuery::condition(const shared_ptr<Condition>& condition) {
	shared_ptr<Container> container =
		shared_ptr<Container>(new Container(condition));
	pack->list.push_back(container);
}

void ConditionsQuery::pushTopOperator() {
	conditions_list_operator o = operators.top();
	operators.pop();
	shared_ptr<Container> container = shared_ptr<Container>(new Container(o));
	pack->list.push_back(container);
}

void ConditionsQuery::setOperator(conditions_list_operator _operator) {
	operators.push(_operator);
}

void ConditionsQuery::parentheses(bool open) {
	if (open)
		operators.push(PARENTHESES);
	else {
		while (!operators.empty() && operators.top() != PARENTHESES) {
			pushTopOperator();
		}
		if (operators.top() == PARENTHESES) {
			conditions_list_operator o = operators.top();
			operators.pop();
		}
	}
}

void ConditionsQuery::done() {
	while (!operators.empty()) {
		pushTopOperator();
	}
}

void ConditionsQuery::reset() {
	operators = stack<conditions_list_operator>();
	pack = shared_ptr<ConditionsPack>(new ConditionsPack());
}

shared_ptr<ConditionsPack> ConditionsQuery::get() {
	shared_ptr<ConditionsPack> current = pack;
	// reset();
	return current;
}