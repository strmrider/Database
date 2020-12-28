#ifndef Cache_h
#define Cache_h
#include "Libs.h"
#include "ConditionsPack.h"
#include "Record.h"
#include "Selections.h"

namespace database {
	class Cache {
	private:
		shared_ptr<ConditionsPack> recent;
		vector<shared_ptr<Record>> references;

	public:
		Cache();
		void setQuery(const ConditionsPack& query);
		void addRef(shared_ptr<Record>& record);
		void clear();
		std::shared_ptr<Selections> fetch(vector<int>& columns);
		bool isCached(const ConditionsPack& query);
	};
}
#endif  // !Cache_h