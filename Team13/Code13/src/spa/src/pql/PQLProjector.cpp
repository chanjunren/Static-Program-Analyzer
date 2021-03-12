#include "PQLProjector.h"

#include <sstream>

STRING_SET PQLProjector::Project(QueryResult query_result) {
	STRINGLIST_SET result = query_result.GetResult();
	STRING_SET final_result = *(new STRING_SET());

	for (auto f = result.cbegin(); f != result.cend(); f++) {
		STRING_LIST values = *(*f);
		string converted_value = "";

		for (string v : values) {
			converted_value.append(v).append(" ");
		}

		converted_value = converted_value.substr(0, converted_value.length() - 1);
		final_result.insert(converted_value);
	}

	return final_result;
}
