#include "stdafx.h"
#include "TableData.h"
using namespace std;
using namespace database;

TableData::TableData(string name, int size,
	vector<shared_ptr<Column>>& columns) {
	this->name = name;
	this->size = size;
	for (shared_ptr<Column> column : columns) 
		columns.push_back(column->copy());
}

const string& TableData::getName() { 
	return name; }

vector<shared_ptr<Column>>& TableData::getColumns() { 
	return columns; }

int TableData::getSize() { 
	return size; }