#pragma once

#include "PQLCustomTypes.h"

class QueryResult
{
private:
	STRINGLIST_SET result;

public:
	QueryResult();

	STRINGLIST_SET GetResult();
	void SetResult(STRINGLIST_SET result);
};
