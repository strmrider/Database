#pragma once
#ifndef Libs_H
#define Libs_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <vector>
#include <list>
#include <stack>
#include <map>
#include <string>

enum data_type { INT, LONG, FLOAT, STRING, VOID };
typedef enum {
	EQUAL = 0,
	NOT_EQUAL,
	GREATER,
	LESSER,
	GREATER_EQUAL,
	LESSER_EQUAL,
	NONE
} comparison_type;

enum condition_pack_element_type { CONDITION, OPERATOR };
enum conditions_list_operator { AND = 0, OR, PARENTHESES };
enum query_type { CREATE, DROP, SELECT, INSERT, DELETE, UPDATE, SAVE };

enum error_type {
	NO_ERROR,
	VOLATILE_ERROR,
	ACTION_ERROR,
	CONDITION_COLUMN_ERROR,
	CONDITION_TYPE_ERROR,
	COLUMN_TYPE_ERROR,
	COLUMN_ERROR,
	TABLE_DOES_NOT_EXIST_ERROR,
	TABLE_ALREADY_EXIST,
	COLUMNS_NUMBER_ERROR
};

#endif  // !Libs_H
