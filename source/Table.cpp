#include "stdafx.h"
#include "Table.h"
using namespace database;

Table::Table(string name, vector<shared_ptr<Column>>& columns) {
	this->name = name;
	for (shared_ptr<Column> column : columns)
		this->columns.push_back(column->copy());
	columnsFunctor =
		shared_ptr<ColumnsDataFunctor>(new ColumnsDataFunctor(this->columns));
}

Table::Table(const TableDiskHeader& header) {
	name = header.name;
	for (int i = 0; i < header.colsNamesList->size(); i++) {
		shared_ptr<Column> c = shared_ptr<Column>(
			new Column((*header.colsNamesList)[i], 0, (*header.typesList)[i]));
		columns.push_back(c->copy());
	}
	columnsFunctor =
		shared_ptr<ColumnsDataFunctor>(new ColumnsDataFunctor(this->columns));
}

Table::~Table() { clear(); }

string Table::getName() { return name; }

TableData& Table::getData() {
	return (*shared_ptr<TableData>(new TableData(name, records.size(), columns)));
}

int Table::headerToCol(string name) {
	for (int i = 0; i < columns.size(); i++) {
		if (columns[i]->getHeader() == name) return i;
	}
	return -1;
}

void Table::insert(const vector<string>& values) {
	records.push_back(
		std::shared_ptr<Record>(new Record(values, columns, columnsFunctor)));
	cache.clear();
}

void Table::insert(vector<shared_ptr<Pair>>& values) {
	records.push_back(
		std::shared_ptr<Record>(new Record(values, columns, columnsFunctor)));
	cache.clear();
}

void Table::remove(const ConditionsPack& conditions) {
	list<std::shared_ptr<Record>>::iterator it;
	std::shared_ptr<Record> record;
	for (auto it = records.begin(); it != records.end();) {
		record = *it;
		if (record->checkConditions(conditions)) {
			it = records.erase(it);
		}
		else
			++it;
	}
	cache.clear();
}

void Table::update(vector<shared_ptr<Pair>>& updates,
	const ConditionsPack& conditions) {
	list<std::shared_ptr<Record>>::iterator it;
	std::shared_ptr<Record> record;
	for (auto it = records.begin(); it != records.end(); ++it) {
		record = *it;
		try {
			record->update(updates, conditions);
		}
		catch (Error& error) {
			throw error;
		}
	}
	cache.clear();
}

std::shared_ptr<Selections> Table::select(const ConditionsPack& conditions) {
	// creates empty indices list
	std::shared_ptr<vector<int>> indices =
		std::shared_ptr<vector<int>>(new vector<int>);
	return select(conditions, indices);
}

std::shared_ptr<Selections> Table::select(const vector<string>& columns,
	const ConditionsPack& conditions) {
	std::shared_ptr<vector<int>> indices =
		std::shared_ptr<vector<int>>(new vector<int>);
	for (string name : columns) {
		indices->push_back(headerToCol(name));
	}

	return select(conditions, indices);
}

std::shared_ptr<Selections> Table::select(
	const ConditionsPack& conditions, std::shared_ptr<vector<int>>& columns) {
	if (cache.isCached(conditions)) {
		return cache.fetch(*columns);
	}

	cache.setQuery(conditions);
	std::shared_ptr<Selections> results =
		std::shared_ptr<Selections>(new Selections());
	std::shared_ptr<vector<string>> currentResult;
	list<std::shared_ptr<Record>>::const_iterator it;
	std::shared_ptr<Record> record;
	for (auto it = records.begin(); it != records.end(); ++it) {
		record = *it;
		if (columns->size() == 0)
			currentResult = record->select(conditions);
		else
			currentResult = record->select(*columns, conditions);
		if (currentResult != NULL) {
			cache.addRef(record);
			results->add(currentResult);
		}
	}
	if (!results->isEmpty())
		return results;
	else
		return NULL;
}

void Table::clear() {
	list<std::shared_ptr<Record>>::iterator it;
	std::shared_ptr<Record> record;
	for (auto it = records.begin(); it != records.end(); ++it) record = *it;

	records.clear();
}

void Table::readRecords(Disk& disk, int numOfRecrods) {
	for (int i = 0; i < numOfRecrods; i++)
		records.push_back(shared_ptr<Record>(
			new Record(*disk.readRecord(columns), columns, columnsFunctor)));
}

void Table::save(Disk& disk) {
	TableDiskHeader* header = createHeader();
	disk.writeDiskHeader(*header);
	list<std::shared_ptr<Record>>::iterator it;
	std::shared_ptr<Record> record;
	for (auto it = records.begin(); it != records.end(); ++it) {
		record = *it;
		disk.writeRecord(*record, columns);
	}
	delete header;
}

void Table::print() {
	list<std::shared_ptr<Record>>::iterator it;
	std::shared_ptr<Record> record;
	for (auto it = records.begin(); it != records.end(); ++it) {
		record = *it;
		record->print();
	}
}

TableDiskHeader* Table::createHeader() {
	string columnNames = "";
	string columnTypes = "";
	for (int i = 0; i < columns.size(); i++) {
		columnNames += columns[i]->getHeader();
		if (i + 1 < columns.size()) columnNames += ",";
		columnTypes += (char)columns[i]->getType();
	}
	return new TableDiskHeader(name, columnNames, columnTypes, records.size());
}

shared_ptr<Error> Table::isColumnValid(Pair& pair) {
	int index = pair.column;
	if (!pair.isIndexed()) {
		index = (*columnsFunctor)(pair.var);
		pair.setColumn(index);
	}

	if (index >= 0) {
		shared_ptr<Error> error = shared_ptr<Error>(new Error());
		return error;
	}
	else {
		string msg = "Column's name " + pair.var + " does not exist";
		shared_ptr<Error> error = shared_ptr<Error>(new Error(COLUMN_ERROR, msg));
		return error;
	}
}

shared_ptr<Error> Table::checkPairsValidity(
const vector<shared_ptr<Pair>>& pairs) {
	for (shared_ptr<Pair> pair : pairs) {
		shared_ptr<Error> error = isColumnValid(*pair);
		if (error->getCode() != NO_ERROR) return error;
		if (columns[pair->column]->getType() != STRING &&
			(getValueType(pair->value) != columns[pair->column]->getType())) {
			string msg =
				"Value '" + pair->value + "' not compatible with Column's type";
			shared_ptr<Error> error =
				shared_ptr<Error>(new Error(COLUMN_TYPE_ERROR, msg));
			return error;
		}
	}
	return shared_ptr<Error>(new Error());
}

shared_ptr<QueryResult> Table::execute(shared_ptr<Query> queryptr) {
	shared_ptr<Query> query = queryptr;
	try {
		if (query->getAction() == INSERT || query->getAction() == UPDATE) {
			shared_ptr<ParityQuery> parityQueryPtr =
				dynamic_pointer_cast<ParityQuery>(query);
			shared_ptr<Error> error = checkPairsValidity(parityQueryPtr->getPairs());
			if (error->getCode() == NO_ERROR) {
				if (query->getAction() == INSERT) {
					shared_ptr<InsertionQuery> insertion =
						dynamic_pointer_cast<InsertionQuery>(query);
					if (insertion->getPairs().size() != columns.size()) {
						string msg = "Number of columns does not fit tables columns";
						return shared_ptr<QueryResult>(
							new QueryResult(*new Error(COLUMNS_NUMBER_ERROR, msg)));
					}
					else
						insert(insertion->getPairs());
				}
				else if (query->getAction() == UPDATE) {
					shared_ptr<UpdateQuery> updateQuery =
						dynamic_pointer_cast<UpdateQuery>(query);
					update(updateQuery->getPairs(), updateQuery->getConditions());
				}
			}
			else {
				return shared_ptr<QueryResult>(new QueryResult(*error));
			}
		}
		else if (query->getAction() == SELECT) {
			shared_ptr<SelectQuery> updateQuery =
				dynamic_pointer_cast<SelectQuery>(query);
			shared_ptr<Selections> selections;
			if (updateQuery->getColumns()->size() > 0)
				selections = select(updateQuery->getConditions());
			else
				selections =
				select(*updateQuery->getColumns(), updateQuery->getConditions());

			return shared_ptr<QueryResult>(new QueryResult(selections));
		}
		else if (query->getAction() == DELETE) {
			shared_ptr<DeletionQuery> deletionQuery =
				dynamic_pointer_cast<DeletionQuery>(query);
			remove(deletionQuery->getConditions());
		}
	}
	catch (Error& error) {
		return shared_ptr<QueryResult>(new QueryResult(error));
	}
	// no errors
	return shared_ptr<QueryResult>(new QueryResult());
}