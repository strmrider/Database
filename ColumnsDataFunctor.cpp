#include "stdafx.h"
#include "ColumnsDataFunctor.h"
using namespace std;
using namespace database;

ColumnsDataFunctor::ColumnsDataFunctor(vector<shared_ptr<Column>>& columns) {
	this->columns =
		shared_ptr<vector<shared_ptr<Column>>>(new vector<shared_ptr<Column>>());
	for (shared_ptr<Column> column : columns)
		this->columns->push_back(column->copy());
}
ColumnsDataFunctor::~ColumnsDataFunctor() {}

int ColumnsDataFunctor::operator()(const string& name) {
	for (int i = 0; i < (columns)->size(); i++) {
		if ((*columns)[i]->getHeader() == name) return i;
	}
	return -1;
}

data_type ColumnsDataFunctor::operator()(int index) {
	return (*columns)[index]->getType();
}