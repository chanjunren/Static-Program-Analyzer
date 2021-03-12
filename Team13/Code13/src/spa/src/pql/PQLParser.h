#include <string>
using namespace std;
#include "QueryInfo.h"
#include "QueryValidator.h"
#include "QuerySyntacticValidator.h"
#include "WhitespaceHandler.h"
#include "PQLTokenizer.h"
#include <vector>
#include <unordered_set>
#include <sstream>
#include <iostream>
#include <regex>
#include <string>

#pragma once
class PQLParser
{
public:
	QueryInfo Parse(string query);

	STRING_STRING_MAP ParseDeclaration(string decl);

	void ParseResultClauseElem(string token, STRING_LIST* output_list);

	void ParseResultClauseTuple(string token, STRING_LIST* output_list);

	STRING_LIST ExtractArguments(string token);

	bool IsPatternPartial(string token);

};

