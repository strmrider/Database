#pragma once
#ifndef Pair_h
#define Pair_h
#include "Libs.h"

namespace database {
	class Pair {
	public:
		std::string value;
		std::string var;
		int column;
		Pair(int column, std::string value);
		Pair(std::string var, std::string value);
		bool isIndexed() const;
		void setColumn(int column);
		bool operator<(const Pair& pair);
	};
}
#endif  // !Pair_h
