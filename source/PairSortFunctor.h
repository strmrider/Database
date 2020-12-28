#pragma once
#ifndef PairSortFunctor_h
#define PairSortFunctor_h
#include "ColumnsDataFunctor.h"
#include "Pair.h"

namespace database {
	class PairSortFunctor {
	private:
		std::shared_ptr<ColumnsDataFunctor> columnsData;
		void setIndex(Pair& pair);

	public:
		PairSortFunctor(std::shared_ptr<ColumnsDataFunctor>& functor);
		bool operator()(const std::shared_ptr<Pair>& firstPair,
			const std::shared_ptr<Pair>& secondPair);
	};
}

#endif  // !PairSortFunctor
