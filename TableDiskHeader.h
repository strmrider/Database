#ifndef TableDiskHeader_h
#define TableDiskHeader_h
#include "Libs.h"
using namespace std;

namespace database {
	class TableDiskHeader {
	public:
		std::string name;
		std::string colsNames;
		std::string types;
		int numOfCols;
		int numOfRecords;
		int totalHeaderSize;

		std::shared_ptr<std::vector<std::string>> colsNamesList;

		std::vector<data_type>* typesList;

		TableDiskHeader(std::string name, std::string colsNames, std::string types,
			int numOfRecords);

		TableDiskHeader(std::string name,
			std::shared_ptr<std::vector<std::string>>& colsNames,
			std::shared_ptr<std::vector<char>> types, int numOfRecords);

		~TableDiskHeader();
	};
}
#endif  // !TableDiskHeader_h
