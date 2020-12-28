#include "stdafx.h"
#include "Column.h"
using namespace std;
using namespace database;

Column::Column(std::string _header, int _size, data_type _type)
	: header(_header), size(_size), type(_type) {}

Column::~Column() {}

data_type Column::getType() const { return type; }

std::string Column::getHeader() { return header; }

std::shared_ptr<Column> Column::copy() {
	return std::shared_ptr<Column>(new Column(header, size, type));
}