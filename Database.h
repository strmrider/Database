#ifndef Database_h
#define Database_h
#include "Table.h"
#include "Disk.h"
#include "QueryResult.h"
#include "Query.h"

namespace database {
	class Database {
	private:
		std::string path;
		std::map<std::string, std::shared_ptr<Table>> tables;
		std::shared_ptr<Disk> disk;
		bool isVolatile;
		void save();
		void readTablesFromDisk();

	public:
		void load();
		Database();
		Database(std::string path);
		~Database();
		void clear();
		std::shared_ptr<QueryResult> query(std::shared_ptr<Query> query);
		std::shared_ptr<Table> getTable(std::string name);
	};
}
#endif  // !Database_h
