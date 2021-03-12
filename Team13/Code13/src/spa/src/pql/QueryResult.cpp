#include "QueryResult.h"

QueryResult::QueryResult() {
	this->result = *(new STRINGLIST_SET());
}

STRINGLIST_SET QueryResult::GetResult() {
	return this->result;
}

void QueryResult::SetResult(STRINGLIST_SET result) {
	this->result = *(new STRINGLIST_SET(result));
}
