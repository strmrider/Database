#ifndef QueryResult_h
#define QueryResult_h
#include "Error.h"
#include "Selections.h"

namespace database {
	class QueryResult {
	private:
		Error error;
		std::shared_ptr<Selections> selections;

	public:
		QueryResult();
		QueryResult(Error& error);
		QueryResult(std::shared_ptr<Selections>& selections);
		std::shared_ptr<Selections> getSelections();
		Error& getError();
		bool isError();
	};
}
#endif  // !QueryResult_h
