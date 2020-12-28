#ifndef Selections_h
#define Selections_h
#include "Libs.h"

namespace database {
	class Selections {
	private:
		std::vector<std::shared_ptr<std::vector<std::string>>> results;

	public:
		Selections();
		~Selections();
		void add(std::shared_ptr<std::vector<std::string>>& values);
		std::shared_ptr<std::vector<std::string>> operator[](int index);
		bool isEmpty();
		int size();
	};
}
#endif  // !Selections
