#pragma once
#ifndef TableData_h
#define TableData_h
#include "Column.h"

namespace database {
	class TableData {
	private:
		std::string name;
		std::vector<std::shared_ptr<Column>> columns;
		int size;

	public:
		TableData(std::string name, int size,
			std::vector<std::shared_ptr<Column>>& columns);
		const std::string& getName();
		std::vector<std::shared_ptr<Column>>& getColumns();
		int getSize();
	};
}
#endif  // !TableData_h
