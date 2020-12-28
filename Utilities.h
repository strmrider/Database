#pragma once
#ifndef Utilities_h
#define Utilities_h
#include "Libs.h"

namespace database {
	data_type toType(std::string type);
	data_type getValueType(const std::string& value);
}
#endif  // !Utilities_h
