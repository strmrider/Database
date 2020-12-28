#include "stdafx.h"
#include "PairSortFunctor.h"
using namespace std;
using namespace database;

void PairSortFunctor::setIndex(Pair& pair) {
	if (!pair.isIndexed()) pair.setColumn((*columnsData)(pair.var));
}

PairSortFunctor::PairSortFunctor(shared_ptr<ColumnsDataFunctor>& functor) {
	columnsData = functor;
}

bool PairSortFunctor::operator()(const shared_ptr<Pair>& firstPair,
	const shared_ptr<Pair>& secondPair) {
	setIndex(*firstPair);
	setIndex(*secondPair);
	return (firstPair->column < secondPair->column);
}