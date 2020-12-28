#include "stdafx.h"
#include "Disk.h"
using namespace std;
using namespace database;

Disk::Disk(string filename)
{
	this->filename = filename;
}

Disk::~Disk() {}

void Disk::open()
{
	file.open(filename, ios::in | ios::out | std::ios::binary);
	if (file.fail())
	{
		file.open(filename, ios::in | ios::out | std::ios::binary | ios::app);
		file.close();
		file.open(filename, ios::in | ios::out | std::ios::binary);
	}
}

void Disk::close()
{
	file.close();
}

void Disk::writeSize(int size)
{
	file.write(reinterpret_cast<char*>(&size), sizeof(int));
}

int Disk::bytesToInt(const char* bytes, int begin, int end)
{
	int integer = 0;
	for (int i = end; i >= begin; i--)
		integer = (integer << 8) + (unsigned char)bytes[i];

	return integer;
}

string Disk::byteToString(const char* bytes, int begin, int end)
{
	string str = "";
	for (int i = begin; i <= end; i++)
		str += (char)bytes[i];
	return str;
}

std::shared_ptr<vector<string>> Disk::split(const string& text, char delimiter)
{
	std::shared_ptr<vector<string>> splitted =
		std::shared_ptr<vector<string>>(new vector<string>);
	string current = "";
	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] != delimiter)
			current += text[i];

		if (text[i] == delimiter || i + 1 == text.length())
		{
			splitted->push_back(current);
			current.clear();
		}
	}
	return splitted;
}

std::shared_ptr<vector<char>> Disk::bytesToTypes(const char* bytes, int begin, int end)
{
	std::shared_ptr<vector<char>> types =
		std::shared_ptr<vector<char>>(new vector<char>);
	for (int i = begin; i <= end; i++)
		types->push_back((char)bytes[i]);
	return types;
}

void Disk::writeRecord(const Record& record,
	vector<shared_ptr<Column>>& columns)
{
	writeSize(record.totalSize(true));
	for (int i = 0; i < columns.size(); i++)
	{
		if (columns[i]->getType() == STRING)
			writeSize(record[i].length());
		switch (columns[i]->getType())
		{
		case STRING:
		{
			file.write(record[i].c_str(), record[i].length());
			break;
		}
		case INT:
		{
			int num = stoi(record[i]);
			file.write(reinterpret_cast<const char *>(&num), sizeof(int));
			break;
		}
		case LONG:
		{
			long num = stol(record[i]);
			file.write((char*)&num, sizeof(long));
			break;
		}
		case FLOAT:
		{
			float num = stof(record[i]);
			file.write((char*)&num, sizeof(float));
			break;
		}
		default:
			break;
		}
	}
}

void Disk::writeDiskHeader(const TableDiskHeader& props)
{
	writeSize(props.totalHeaderSize);
	writeSize(props.name.length());
	file.write(props.name.c_str(), props.name.length());
	writeSize(props.colsNames.length());
	file.write(props.colsNames.c_str(), props.colsNames.length());
	file.write(props.types.c_str(), props.types.length());
	writeSize(props.numOfRecords);
}

std::shared_ptr<TableDiskHeader> Disk::readHeader()
{
	int size;
	file.read(reinterpret_cast<char*>(&size), sizeof(int));
	char* header = new char[size];
	file.read(header, size);
	int nameLen = bytesToInt(header, 0, 3);
	string name = byteToString(header, 4, nameLen + 3);
	int colsSize = bytesToInt(header, nameLen + 4, nameLen + 8);
	string cols =
		byteToString(header, nameLen + 8, nameLen + 8 + (colsSize - 1));
	std::shared_ptr<vector<string>> colsNames = split(cols, ',');
	int colsTypesSize = colsNames->size();
	std::shared_ptr<vector<char>> types =
		bytesToTypes(header, nameLen + 8 + colsSize, nameLen + 8 + colsSize + colsTypesSize - 1);
	int numOfRecords =
		bytesToInt(header, nameLen + 4 + 4 + colsSize + colsTypesSize, size - 1);
	delete header;
	std::shared_ptr<TableDiskHeader> properties =
		std::shared_ptr<TableDiskHeader>(
			new TableDiskHeader(name, colsNames, types, numOfRecords));
	return properties;
}

std::shared_ptr<vector<string>> Disk::readRecord(
	const vector<shared_ptr<Column>>& columns)
{
	std::shared_ptr<vector<string>> values =
		std::shared_ptr<vector<string>>(new vector<string>);
	int size;
	file.read(reinterpret_cast<char*>(&size), sizeof(int));
	char* bytes = new char[size];
	file.read(bytes, size);
	int lastIndex = 0;
	for (shared_ptr<Column> column : columns)
	{
		std::ostringstream value;
		if (column->getType() == INT)
		{
			value << (bytesToInt(bytes, lastIndex, lastIndex + 3));
			lastIndex += 4;
		}
		else if (column->getType() == FLOAT)
		{
			value << (bytesToInt(bytes, lastIndex, lastIndex + 3));
			lastIndex += 4;
		}
		else if (column->getType() == LONG)
		{
			value << (bytesToInt(bytes, lastIndex, lastIndex + 7));
			lastIndex += 8;
		}
		else if (column->getType() == STRING)
		{
			int s = (bytesToInt(bytes, lastIndex, lastIndex + 3));
			lastIndex += 4;
			value << byteToString(bytes, lastIndex, lastIndex + s - 1);
			lastIndex += s;
		}
		values->push_back(value.str());
	}
	return values;
}

void Disk::writeDatabaseDiskHeader(int size)
{
	writeSize(size);
}

int Disk::readDatabaseHeader()
{
	int size;
	file.read(reinterpret_cast<char*>(&size), sizeof(int));
	return size;
}