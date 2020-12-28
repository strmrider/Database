#include "stdafx.h"
#include "TableDiskHeader.h"
using namespace std;
using namespace database;

TableDiskHeader::TableDiskHeader(string name, string colsNames,
	string types, int numOfRecords)
{
	this->name = name;
	this->colsNames = colsNames;
	this->types = types;
	this->numOfRecords = numOfRecords;

	totalHeaderSize =
		4 + name.length() + 4 + colsNames.length() + types.length() + 4;
	colsNamesList = nullptr;
	typesList = nullptr;
}

TableDiskHeader::TableDiskHeader(string name,
	std::shared_ptr<vector<string>>& colsNames,
	std::shared_ptr<vector<char>> types, int numOfRecords)
{
	this->name = name;
	this->colsNamesList = colsNames;
	this->numOfRecords = numOfRecords;
	typesList = new vector<data_type>;
	for (char type : *types)
		typesList->push_back(static_cast<data_type>((int)type));
}

TableDiskHeader::~TableDiskHeader() {}