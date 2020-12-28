#ifndef Table_h
#define Table_h
#include "Query.h"
#include "Record.h"
#include "Selections.h"
#include "Disk.h"
#include "Cache.h"
#include "QueryResult.h"
#include "TableData.h"

namespace database {
	class Table {
	private:
		string name;
		vector<shared_ptr<Column>> columns;
		list<std::shared_ptr<Record>> records;
		shared_ptr<ColumnsDataFunctor> columnsFunctor;
		Cache cache;

		std::shared_ptr<Selections> select(const ConditionsPack& conditions,
			std::shared_ptr<vector<int>>& columns);
		int headerToCol(string name);
		TableDiskHeader* createHeader();
		shared_ptr<Error> isColumnValid(Pair& pair);
		shared_ptr<Error> checkPairsValidity(const vector<shared_ptr<Pair>>& pairs);

	public:
		Table(string name, const vector<shared_ptr<Column>>& headers);
		Table(const TableDiskHeader& header);
		Table(string name, vector<shared_ptr<Column>>& columns);
		~Table();
		string getName();
		TableData& getData();
		void insert(const vector<string>& values);
		void insert(vector<shared_ptr<Pair>>& values);
		void remove(const ConditionsPack& conditions);
		void update(vector<shared_ptr<Pair>>& updates,
			const ConditionsPack& conditions);
		std::shared_ptr<Selections> select(const ConditionsPack& conditions);
		std::shared_ptr<Selections> select(const vector<string>& columns,
			const ConditionsPack& conditions);

		void readRecords(Disk& disk, int numOfRecrods);
		void clear();
		void save(Disk& disk);
		void print();
		shared_ptr<QueryResult> execute(shared_ptr<Query> queryptr);

		void recordsSize() {
			list<std::shared_ptr<Record>>::iterator it;
			std::shared_ptr<Record> record;
			for (auto it = records.begin(); it != records.end(); ++it) {
				record = *it;
				cout << record->totalSize() << endl;
			}
		}
	};
}
#endif  // !Table_h
