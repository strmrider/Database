#pragma once
#ifndef ColumnsDataFunctor_h
#define ColumnsDataFunctor_h
#include "Libs.h"
#include "Column.h"

namespace database {
	class ColumnsDataFunctor {
	private:
		std::shared_ptr<std::vector<std::shared_ptr<Column>>> columns;

	public:
		ColumnsDataFunctor(std::vector<std::shared_ptr<Column>>& columns);
		~ColumnsDataFunctor();
		int operator()(const std::string& name);
		data_type operator()(int index);
	};
}
#endif