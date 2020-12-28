#ifndef Error_h
#define Error_h
#include "Libs.h"

namespace database {
	class Error {
	private:
		error_type error;
		std::string message;

	public:
		Error();
		Error(error_type type, std::string errorMessage);
		error_type getCode();
		std::string getMessage();
	};
}
#endif  // !Error_h
