#include "stdafx.h"
#include "Parser.h"
using namespace std;
using namespace database;

Parser::Parser() {}

shared_ptr<vector<string>> Parser::splitQuery(const string& query) {
	shared_ptr<vector<string>> tokens =
		shared_ptr<vector<string>>(new vector<string>());
	string token;
	bool apostrophe = false;
	for (int i = 0; i < query.length(); i++) {
		if (query[i] == '\'') apostrophe = !apostrophe;

		if ((query[i] != ':' && query[i] != '{' && query[i] != '}') || apostrophe) {
			if ((query[i] != ' ' && query[i] != '\t') || apostrophe)
				token += query[i];
		}

		if (((query[i] == ':' || query[i] == '{' || query[i] == '}') &&
			!apostrophe) ||
			i + 1 == query.length()) {
			if (token.length() > 0) {
				tokens->push_back(token);
				token = "";
			}
		}
	}

	return tokens;
}

shared_ptr<Condition> Parser::parseCondition(const string& str) {
	shared_ptr<Condition> condition;
	string token = "";
	string column = "";
	string value = "";
	comparison_type type = NONE;
	bool apostrophe = false;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == '\'') apostrophe = !apostrophe;

		if ((str[i] == '>' || str[i] == '<' || str[i] == '=' || str[i] == '!') &&
			!apostrophe) {
			column = token;
			token = "";
			if (str[i] == '>') {
				if (str[i + 1] == '=') {
					type = GREATER_EQUAL;
					i++;
				}
				else
					type = GREATER;
			}
			else if (str[i] == '<') {
				if (str[i + 1] == '=') {
					type = LESSER_EQUAL;
					i++;
				}
				else
					type = LESSER;
			}
			else if (str[i] == '!') {
				if (str[i + 1] == '=') {
					type = NOT_EQUAL;
					i++;
				}
			}
			else if (str[i] == '=')
				type = EQUAL;
		}
		else {
			if (str[i] != '\'') token += str[i];
		}
		if (i + 1 == str.length()) {
			value += token;
		}
	}
	condition =
		shared_ptr<Condition>(new ComparisonCondition(-1, column, value, type));
	return condition;
}

shared_ptr<ConditionsPack> Parser::parseConditionsPack(
	const string& expression) {
	ConditionsQuery query;
	bool apostrophe = false;
	string condition = "";
	for (int i = 0; i < expression.length(); i++) {
		if (expression[i] == '\'') apostrophe = !apostrophe;

		if (((expression[i] == '&' || expression[i] == '|') ||
			(expression[i] == '(' || expression[i] == ')') && !apostrophe) ||
			i + 1 == expression.length()) {
			if (expression[i] != '(') {
				query.condition(parseCondition(condition));
				condition = "";
			}

			if (expression[i] == '&')
				query.setOperator(AND);
			else if (expression[i] == '|')
				query.setOperator(OR);
			else if (expression[i] == '(')
				query.parentheses(true);
			else if (expression[i] == ')')
				query.parentheses(false);

		}
		else if (expression[i] != ' ' || apostrophe) {
			condition += expression[i];
		}
	}

	query.done();
	return query.get();
}

shared_ptr<vector<string>> Parser::splitColumns(const string& str) {
	shared_ptr<vector<string>> tokens =
		shared_ptr<vector<string>>(new vector<string>);
	if (str == "*") return tokens;
	string value = "";
	bool apostrophe = false;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == '\'') apostrophe = !apostrophe;

		if ((str[i] != ',') || apostrophe) value += str[i];

		if ((str[i] == ',' && !apostrophe) || i + 1 == str.length()) {
			tokens->push_back(value);
			value = "";
		}
	}
	return tokens;
}

shared_ptr<Pair> Parser::parsePair(const string& pair) {
	shared_ptr<Pair> updater;
	string name = "";
	string value = "";
	string token = "";
	bool apostrophe = false;
	for (int i = 0; i < pair.length(); i++) {
		if (pair[i] == '\'') apostrophe = !apostrophe;

		if ((pair[i] != '=' || apostrophe) && pair[i] != '\'') {
			token += pair[i];
		}

		if (pair[i] == '=' && !apostrophe) {
			name = token;
			token = "";
		}
		else if (i + 1 == pair.length()) {
			value = token;
		}
	}

	updater = shared_ptr<Pair>(new Pair(name, value));
	return updater;
}

shared_ptr<vector<shared_ptr<Pair>>> Parser::parsePairs(
	const vector<string>& tokens) {
	shared_ptr<vector<shared_ptr<Pair>>> updaters =
		shared_ptr<vector<shared_ptr<Pair>>>(new vector<shared_ptr<Pair>>);

	for (string token : tokens) {
		updaters->push_back(parsePair(token));
	}

	return updaters;
}

shared_ptr<vector<shared_ptr<Column>>> Parser::parseColumns(
	shared_ptr<vector<string>> names, shared_ptr<vector<string>> types) {
	shared_ptr<vector<shared_ptr<Column>>> columns =
		shared_ptr<vector<shared_ptr<Column>>>(new vector<shared_ptr<Column>>);
	shared_ptr<Column> column;
	string name;
	data_type type;
	for (int i = 0; i < names->size(); i++) {
		name = (*names)[i];
		type = toType((*types)[i]);
		column = shared_ptr<Column>(new Column(name, 0, type));
		columns->push_back(column);
	}

	return columns;
}

shared_ptr<Query> Parser::parse(const string& query) {
	shared_ptr<vector<string>> tokens = splitQuery(query);
	if ((*tokens)[1] == "create") {
		return shared_ptr<Query>(new CreateTableQuery(
			(*tokens)[0],
			parseColumns(splitColumns((*tokens)[2]), splitColumns((*tokens)[3]))));

	}
	else if ((*tokens)[1] == "drop") {
		return shared_ptr<Query>(new Query(query_type::DROP, (*tokens)[0]));
	}
	else if ((*tokens)[1] == "save") {
		return shared_ptr<Query>(new Query(query_type::SAVE, (*tokens)[0]));
	}
	else if ((*tokens)[1] == "insert") {
		return shared_ptr<Query>(
			new InsertionQuery(query_type::INSERT, (*tokens)[0],
				parsePairs(*splitColumns((*tokens)[2]))));
	}
	else if ((*tokens)[1] == "select") {
		return shared_ptr<Query>(
			new SelectQuery((*tokens)[0], splitColumns((*tokens)[2]),
				parseConditionsPack((*tokens)[3])));
	}
	else if ((*tokens)[1] == "delete") {
		return shared_ptr<Query>(new DeletionQuery(
			query_type::DELETE, (*tokens)[0], parseConditionsPack((*tokens)[2])));
	}
	else if ((*tokens)[1] == "update") {
		return shared_ptr<Query>(
			new UpdateQuery(query_type::UPDATE, (*tokens)[0],
				parsePairs(*splitColumns((*tokens)[2])),
				parseConditionsPack((*tokens)[3])));
	}
	return nullptr;
}