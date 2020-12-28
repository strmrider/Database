#include "stdafx.h"
#include "Query.h"
using namespace std;
using namespace database;

Query::Query() {}

Query::Query(query_type action, const string& name) {
	this->action = action;
	this->table = name;
}

query_type Query::getAction() { return action; }

string& Query::getTable() { return table; }

/****************************************/

CreateTableQuery::CreateTableQuery(
	const string& name, shared_ptr<vector<shared_ptr<Column>>> columns)
	: Query(query_type::CREATE, name) {
	newColumns = columns;
}

CreateTableQuery::CreateTableQuery(const string& name)
	: Query(query_type::CREATE, name) {
	newColumns =
		shared_ptr<vector<shared_ptr<Column>>>(new vector<shared_ptr<Column>>);
}

void CreateTableQuery::addColumn(const string& name, data_type type) {
	newColumns->push_back(shared_ptr<Column>(new Column(name, 0, type)));
}

vector<shared_ptr<Column>>& CreateTableQuery::getColumns() {
	return (*newColumns);
}

/****************************************/

ConditionalQuery::ConditionalQuery(query_type action, const string& name,
	shared_ptr<ConditionsPack> conditions)
	: Query(action, name) {
	this->conditions = conditions;
}

ConditionalQuery::ConditionalQuery(const string& name,
	shared_ptr<ConditionsPack> conditions)
	: Query(query_type::DELETE, name) {
	this->conditions = conditions;
}

ConditionsPack& ConditionalQuery::getConditions() { return *conditions; }

/****************************************/

ParityQuery::ParityQuery(query_type action, const string& name,
	shared_ptr<vector<shared_ptr<Pair>>> pairs)
	: Query(action, name) {
	this->pairs = pairs;
}

ParityQuery::ParityQuery(query_type action, const string& name)
	: Query(action, name) {
	this->pairs =
		shared_ptr<vector<shared_ptr<Pair>>>(new vector<shared_ptr<Pair>>);
}

ParityQuery::ParityQuery(const string& name) : Query(query_type::INSERT, name) {
	this->pairs =
		shared_ptr<vector<shared_ptr<Pair>>>(new vector<shared_ptr<Pair>>);
}

void ParityQuery::addPair(Pair& pair) {
	pairs->push_back(shared_ptr<Pair>(&pair));
}

void ParityQuery::addField(const string& column, const string& value) {
	pairs->push_back(shared_ptr<Pair>(new Pair(column, value)));
}

vector<shared_ptr<Pair>>& ParityQuery::getPairs() { return *pairs; }

/****************************************/

UpdateQuery::UpdateQuery(query_type action, const string& name,
	shared_ptr<vector<shared_ptr<Pair>>> pairs,
	shared_ptr<ConditionsPack> conditions)
	: ConditionalQuery(action, name, conditions),
	ParityQuery(action, name, pairs) {}

UpdateQuery::UpdateQuery(const string& name,
	shared_ptr<ConditionsPack> conditions)
	: ConditionalQuery(query_type::UPDATE, name, conditions),
	ParityQuery(query_type::UPDATE, name) {
	this->action = query_type::UPDATE;
	this->table = name;
}

/****************************************/

SelectQuery::SelectQuery(const string& name, shared_ptr<vector<string>> columns,
	shared_ptr<ConditionsPack> conditions)
	: ConditionalQuery(query_type::SELECT, name, conditions) {
	this->action = query_type::SELECT;
	this->table = name;
	this->columns = columns;
	this->conditions = conditions;
}

SelectQuery::SelectQuery(const string& name,
	shared_ptr<ConditionsPack> conditions)
	: ConditionalQuery(query_type::SELECT, name, conditions) {
	this->action = query_type::SELECT;
	this->table = name;
	columns = shared_ptr<vector<string>>(new vector<string>);
	this->conditions = conditions;
}

void SelectQuery::addColumn(const string& column) {
	columns->push_back(column);
}

shared_ptr<vector<string>> SelectQuery::getColumns() { return columns; }