#include "stdafx.h"
#include "QueryResult.h"
using namespace std;
using namespace database;

QueryResult::QueryResult() { selections = nullptr; }

QueryResult::QueryResult(Error& error) { this->error = error; }
QueryResult::QueryResult(shared_ptr<Selections>& selections) {
	this->selections = selections;
}

shared_ptr<Selections> QueryResult::getSelections() { return selections; }

Error& QueryResult::getError() { return error; }

bool QueryResult::isError() { return error.getCode() != NO_ERROR; }