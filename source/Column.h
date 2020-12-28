#pragma once
#ifndef Column_H
#define Column_H
#include "Libs.h"

namespace database {
	class Column {
	private:
		std::string header;
		int size;
		data_type type;

	public:
		Column(std::string _header, int _size, data_type _type);
		~Column();
		data_type getType() const;
		std::string getHeader();
		std::shared_ptr<Column> copy();
	};
}

#endif  // !Column_H
