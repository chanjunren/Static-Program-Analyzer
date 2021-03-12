#pragma once

#include <unordered_map>

#include "PQLCustomTypes.h"

using namespace std;

class QueryInfo
{
private:
	STRING_STRINGLISTLIST_MAP st_map_;
	STRING_STRINGLISTLIST_MAP pattern_map_;
	STRINGPAIR_SET with_map_;
	STRING_STRING_MAP entity_map_;
	STRING_LIST output_list_;

	bool is_valid_ = true;
	bool is_invalid_due_to_semantics_ = false;

public:
	/* getters */
	STRING_STRINGLISTLIST_MAP GetStMap();
	STRING_STRINGLISTLIST_MAP GetPatternMap();
	STRINGPAIR_SET GetWithMap();
	STRING_STRING_MAP GetEntityMap();
	STRING_LIST GetOutputList();
	STRINGLIST_SET GetFalseResult();
	bool IsQueryInfoValid();
	bool IsBooleanOutput();
	bool IsSemanticsInvalid();

	/* setters */
	void SetStMap(STRING_STRINGLISTLIST_MAP st_map);
	void SetPatternMap(STRING_STRINGLISTLIST_MAP pattern_map);
	void SetWithMap(STRINGPAIR_SET with_map);
	void SetEntityMap(STRING_STRING_MAP entity_map);
	void SetOutputList(STRING_LIST output_list);
	void SetValidToFalse();
	void SetInvalidDueToSemanticsTrue();
	void SetInvalidDueToSemanticsFalse();

	/* printing */
	void PrintOutputList();
	void PrintEntityMap();
	void PrintClausesMap();
	void PrintStMap();
	void PrintPatternMap();
	void PrintWithMap();
};
