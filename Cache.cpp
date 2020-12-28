#include "stdafx.h"
#include "Cache.h"
using namespace std;
using namespace database;

Cache::Cache() { recent = nullptr; }

void Cache::setQuery(const ConditionsPack& query) {
	clear();
	recent = shared_ptr<ConditionsPack>(new ConditionsPack());
	(*recent) = query;
}

void Cache::addRef(shared_ptr<Record>& record) {
	recent = nullptr;
	references.push_back(record);
}

void Cache::clear() { references.clear(); }

std::shared_ptr<Selections> Cache::fetch(vector<int>& columns) {
	std::shared_ptr<Selections> selections =
		std::shared_ptr<Selections>(new Selections());
	for (shared_ptr<Record> record : references)
		selections->add(record->select(columns));

	return selections;
}

bool Cache::isCached(const ConditionsPack& query) {
	if (recent != nullptr) {
		return *recent == query;
	}
	return false;
}