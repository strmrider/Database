#include "stdafx.h"
#include "Pair.h"
using namespace std;
using namespace database;

Pair::Pair(int column, string value) {
	this->column = column;
	this->value = value;
}

Pair::Pair(string var, string value) {
	this->var = var;
	this->value = value;
	column = -1;
}

bool Pair::isIndexed() const { return column >= 0; }

void Pair::setColumn(int column) { this->column = column; }

bool Pair::operator<(const Pair& pair) { return (column < pair.column); }