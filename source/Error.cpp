#include "stdafx.h"
#include "Error.h"
using namespace database;

Error::Error() {
	error = NO_ERROR;
	message = "No Errors were detected";
}
Error::Error(error_type type, std::string errorMessage) {
	error = type;
	message = errorMessage;
}

error_type Error::getCode() { return error; }

std::string Error::getMessage() { return message; }