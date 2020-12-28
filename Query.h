#ifndef Query_h
#define Query_h
#include "Libs.h"
#include "Column.h"
#include "ConditionsPack.h"
#include "Pair.h"

namespace database {
	class Query : public enable_shared_from_this<Query> {
	protected:
		query_type action;
		string table;

	public:
		Query();
		virtual ~Query() {};
		Query(query_type action, const string& name);
		query_type getAction();
		string& getTable();
	};

	/****************************************/

	class CreateTableQuery : public Query {
	private:
		shared_ptr<vector<shared_ptr<Column>>> newColumns;

	public:
		CreateTableQuery(const string& name,
			shared_ptr<vector<shared_ptr<Column>>> columns);
		CreateTableQuery(const string& name);
		virtual ~CreateTableQuery() {};
		void addColumn(const string& name, data_type type);
		vector<shared_ptr<Column>>& getColumns();
	};

	/****************************************/

	typedef class ConditionalQuery : virtual public Query {
	protected:
		shared_ptr<ConditionsPack> conditions;

	public:
		ConditionalQuery(query_type action, const string& name,
			shared_ptr<ConditionsPack> conditions);
		ConditionalQuery(const string& name, shared_ptr<ConditionsPack> conditions);
		ConditionsPack& getConditions();
		virtual ~ConditionalQuery() {};
	} DeletionQuery;

	/****************************************/

	typedef class ParityQuery : virtual public Query {
	protected:
		shared_ptr<vector<shared_ptr<Pair>>> pairs;

	public:
		ParityQuery(query_type action, const string& name,
			shared_ptr<vector<shared_ptr<Pair>>> pairs);
		ParityQuery(query_type action, const string& name);
		ParityQuery(const string& name);
		virtual ~ParityQuery() {};
		void addPair(Pair& pair);
		void addField(const string& column, const string& value);
		vector<shared_ptr<Pair>>& getPairs();
	} InsertionQuery;

	/****************************************/

	class UpdateQuery : public ConditionalQuery, public ParityQuery {
	public:
		UpdateQuery(query_type action, const string& name,
			shared_ptr<vector<shared_ptr<Pair>>> pairs,
			shared_ptr<ConditionsPack> conditions);
		UpdateQuery(const string& name, shared_ptr<ConditionsPack> conditions);
		virtual ~UpdateQuery() {};
	};

	/****************************************/
	class SelectQuery : public ConditionalQuery {
	private:
		shared_ptr<vector<string>> columns;

	public:
		SelectQuery(const string& name, shared_ptr<vector<string>> columns,
			shared_ptr<ConditionsPack> conditions);
		SelectQuery(const string& name, shared_ptr<ConditionsPack> conditions);
		virtual ~SelectQuery() {};
		void addColumn(const string& column);
		shared_ptr<vector<string>> getColumns();
	};
}
#endif  // !Query.h