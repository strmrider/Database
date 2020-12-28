#include "stdafx.h"
#include "Selections.h"
using namespace std;
using namespace database;

Selections::Selections() {}
Selections::~Selections() {}
void Selections::add(std::shared_ptr<vector<string>>& values) {
	results.push_back(values);
}
shared_ptr<vector<string>> Selections::operator[](int index) {
	return (results)[index];
}

bool Selections::isEmpty() { return results.size() == 0; }
int Selections::size() { return results.size(); }