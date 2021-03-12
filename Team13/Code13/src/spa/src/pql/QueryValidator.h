#include "PQLCustomTypes.h"
#include "QueryRules.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
using namespace std;
#pragma once
class QueryValidator
{
private:
	QueryRules query_rules_;

public:
	QueryValidator();

	bool ValidateDeclaration(string decl, STRING_STRING_MAP entity_map);

	bool ValidateSelect(string token);

	bool ValidateResultClause(string token, STRING_STRING_MAP user_declared_vars);

	bool ValidateSuchthatClause(string token, STRING_STRING_MAP user_declared_vars);

	string GetValidRelRefType(string token, STRING_STRING_MAP user_declared_vars);

	bool ValidatePatternClause(string token, STRING_STRING_MAP user_declared_vars);

	bool ValidateWithClause(string token, STRING_STRING_MAP user_declared_vars);

};

