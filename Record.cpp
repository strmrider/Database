#include "stdafx.h"
#include "Record.h"
using namespace std;
using namespace database;

Record::Record(const vector<string>& values,
	const vector<shared_ptr<Column>>& columns,
	shared_ptr<ColumnsDataFunctor>& columnsFunctor) {
	this->columnsFunctor = columnsFunctor;
	std::shared_ptr<Field> field;
	for (int i = 0; i < values.size(); i++) {
		field =
			std::shared_ptr<Field>(new Field((columns)[i]->getType(), values[i]));
		fields.push_back(field);
	}
}

Record::Record(vector<shared_ptr<Pair>>& values,
	const vector<shared_ptr<Column>>& columns,
	shared_ptr<ColumnsDataFunctor>& columnsFunctor) {
	this->columnsFunctor = columnsFunctor;
	shared_ptr<Field> field;
	PairSortFunctor functor(columnsFunctor);
	std::sort(values.begin(), values.end(), functor);

	for (int i = 0; i < values.size(); i++) {
		field = std::shared_ptr<Field>(
			new Field(columns[i]->getType(), values[i]->value));
		fields.push_back(field);
	}
}

Record::~Record() { fields.clear(); }

int Record::numOfFields() const { return fields.size(); }

std::string Record::operator[](int column) const {
	if (column >= 0 && column < fields.size())
		return fields[column]->getValue();
	else {
		return NULL;
	}
}

void Record::setIndexPerNameInPairs(const vector<shared_ptr<Pair>>& pairs) {
	for (shared_ptr<Pair> pair : pairs) {
		if (!pair->isIndexed()) pair->setColumn((*columnsFunctor)(pair->var));
	}
}

void Record::calTopStackConditions(conditions_list_operator _operator,
	stack<bool>& results) const {
	bool first = results.top();
	results.pop();
	bool second = results.top();
	results.pop();
	if (_operator == AND)
		results.push((first && second));
	else if (_operator == OR)
		results.push(first || second);
}

int Record::getConditionColumn(Condition& condition) const {
	int index = condition.getColumn();
	if (index == -1) {
		index = (*columnsFunctor)(condition.getColumnName());
	}
	// still no index
	if (index == -1) {
		throw Error(CONDITION_COLUMN_ERROR, "Column does not exist");
	}
	return index;
}

bool Record::checkConditions(const ConditionsPack& pack) const {
	if (pack.list.size() == 0) return true;

	list<shared_ptr<Container>>::iterator it;
	std::shared_ptr<Container> container;
	stack<bool> results;
	for (auto it = pack.list.begin(); it != pack.list.end(); ++it) {
		container = *it;
		if (container->type == CONDITION) {
			try {
				int index = getConditionColumn(*container->condition);

				bool result = fields[index]->checkCondition(*container->condition);
				results.push(result);
			}
			catch (Error& error) {
				throw error;
			}
		}
		else if (container->type == OPERATOR) {
			calTopStackConditions(container->_operator, results);
		}
	}
	return results.top();
}

std::shared_ptr<vector<string>> Record::select(const ConditionsPack& pack) {
	if (checkConditions(pack)) {
		vector<int> columns;
		return select(columns);
	}

	return NULL;
}

std::shared_ptr<vector<string>> Record::select(vector<int> columns,
	const ConditionsPack& pack) {
	if (checkConditions(pack)) return select(columns);

	return NULL;
}

std::shared_ptr<vector<string>> Record::select(vector<int>& columns) {
	std::shared_ptr<vector<string>> values =
		std::shared_ptr<vector<string>>(new vector<string>);
	if (columns.size() == 0) {
		for (std::shared_ptr<Field> field : fields)
			values->push_back(field->getValue());
	}
	else {
		for (int i = 0; i < columns.size(); i++)
			values->push_back(fields[columns[i]]->getValue());
	}

	return values;
}

void Record::update(vector<shared_ptr<Pair>>& updates,
	const ConditionsPack& pack) {
	int index;
	if (checkConditions(pack)) {
		for
			each(shared_ptr<Pair> updater in updates) {
			index = (*updater).column;
			if (!updater->isIndexed())
				index = (*columnsFunctor)(updater->var);
			fields[index]->setValue(updater->value);
		}
	}
}

int Record::totalSize(bool sizeOnDisk) const {
	int sum = 0;
	for (std::shared_ptr<Field> field : fields) {
		sum += field->size(sizeOnDisk);
	}
	return sum;
}

void Record::print() {
	for (int i = 0; i < fields.size(); i++) {
		cout << fields[i]->getValue();
		if (i + 1 < fields.size())
			cout << "\t";
		else
			cout << endl;
	}
}