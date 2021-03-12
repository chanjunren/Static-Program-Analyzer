#include "PQLCustomTypes.h"
#include "QueryValidator.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <regex>
#include <sstream>
#include <iostream>
using namespace std;

QueryValidator::QueryValidator() {
	QueryRules query_rules;
	this->query_rules_ = query_rules;
}

bool QueryValidator::ValidateDeclaration(string token, STRING_STRING_MAP entity_map) {
	return this->query_rules_.IsDeclaration(token, entity_map);
}

bool QueryValidator::ValidateSelect(string token) {
	return this->query_rules_.IsSelect(token);
}

bool QueryValidator::ValidateResultClause(string token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsResultClause(token, user_declared_vars);
}

bool QueryValidator::ValidateSuchthatClause(string token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsSuchThatClause(token, user_declared_vars);
}

string QueryValidator::GetValidRelRefType(string token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.GetRelRefType(token, user_declared_vars);
}

bool QueryValidator::ValidatePatternClause(string token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsPatternClause(token, user_declared_vars);
}

bool QueryValidator::ValidateWithClause(string token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsWithClause(token, user_declared_vars);
}