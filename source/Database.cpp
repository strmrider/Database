#include "stdafx.h"
#include "Database.h"
using namespace std;
using namespace database;

Database::Database() { isVolatile = true; }

Database::Database(string path) {
	this->path = path;
	disk = shared_ptr<Disk>(new Disk(path));
	isVolatile = false;
}

Database::~Database() { tables.clear(); }

shared_ptr<Table> Database::getTable(string name) {
	if (tables.count(name) > 0)
		return tables[name];
	else
		return nullptr;
}

void Database::save() {
	disk->open();
	disk->writeDatabaseDiskHeader(tables.size());
	for (const auto& mappedTable : tables) {
		mappedTable.second->save(*disk);
	}
	disk->close();
}

void Database::clear() {}

void Database::readTablesFromDisk() {
	shared_ptr<TableDiskHeader> header;
	shared_ptr<Table> table;
	disk->open();
	int size = disk->readDatabaseHeader();
	for (int i = 0; i < size; i++) {
		header = disk->readHeader();
		table = shared_ptr<Table>(new Table(*header));
		table->readRecords(*disk, header->numOfRecords);
		tables.insert(pair<string, shared_ptr<Table>>(table->getName(), table));
	}
	disk->close();
}

void Database::load() {
	if (!isVolatile) readTablesFromDisk();
}

shared_ptr<QueryResult> Database::query(shared_ptr<Query> query) {
	shared_ptr<Query> queryPtr = query;
	if (query->getAction() == CREATE) {
		shared_ptr<CreateTableQuery> newTableQuery =
			dynamic_pointer_cast<CreateTableQuery>(queryPtr);
		shared_ptr<Table> table = shared_ptr<Table>(
			new Table(newTableQuery->getTable(), newTableQuery->getColumns()));
		tables.insert(
			pair<string, shared_ptr<Table>>(newTableQuery->getTable(), table));
	}
	else if (tables.count(query->getTable()) > 0) {
		if (query->getAction() == SAVE) {
			if (!isVolatile)
				save();
			else {
				string msg = "Can't save. Database is volatile";
				Error error(TABLE_DOES_NOT_EXIST_ERROR, msg);
				return *new shared_ptr<QueryResult>(new QueryResult(error));
			}

		}
		else if (query->getAction() == DROP)
			tables.erase(query->getTable());
		else
			return tables[query->getTable()]->execute(query);
	}
	else {
		string msg = "Table '" + query->getTable() + "' does not exist in database";
		Error error(TABLE_DOES_NOT_EXIST_ERROR, msg);
		return *new shared_ptr<QueryResult>(new QueryResult(error));
	}

	return *new shared_ptr<QueryResult>(new QueryResult());
}
