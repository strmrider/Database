#pragma once
#ifndef Disk_h
#define Disk_h
#include "Libs.h"
#include "Record.h"
#include "TableDiskHeader.h"
#include "Column.h"

namespace database
{
	class Disk
	{
	private:
		std::string filename;
		std::fstream file;

		void writeSize(int size);
		int bytesToInt(const char* bytes, int begin, int end);
		std::string byteToString(const char* bytes, int begin, int end);
		std::shared_ptr<std::vector<char>> bytesToTypes(
			const char* bytes, int begin, int end);

		std::shared_ptr<std::vector<std::string>> split(
			const std::string& text, char delimiter);

	public:
		Disk(std::string filename);
		~Disk();
		void open();
		void close();
		void writeRecord(const Record& record,
			std::vector<std::shared_ptr<Column>>& columns);
		void writeDatabaseDiskHeader(int size);
		void writeDiskHeader(const TableDiskHeader& props);
		int readDatabaseHeader();
		std::shared_ptr<TableDiskHeader> readHeader();

		std::shared_ptr<vector<string>> readRecord(
			const std::vector<std::shared_ptr<Column>>& columns);

	};
}
#endif // !Disk_h