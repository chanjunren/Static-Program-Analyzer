#pragma once

#include "PQLCustomTypes.h"

class QueryRules
{
	/*
		Class to Define all Query Rules. Provides all the helper functions for the QuerySyntaxValidator.
		Provides all the functions to check each token.
	*/
public:
	// Lexical Tokens
	bool IsIdent(string token);
	bool IsName(string token);
	bool IsInteger(string token);
	bool IsSynonym(string token);
	bool IsStmtRef(string token, string synonym_type);
	bool IsEntRef(string token, string synonym_type);
	bool IsLineRef(string token, string synonym_type);
	bool IsElem(string token, STRING_STRING_MAP declared_var_names);
	bool IsAttrName(string token);
	bool IsDesignEntity(string token);

	// Grammar Rules
	bool IsSelect(string token);
	bool IsDeclaration(string token, STRING_STRING_MAP entity_map);
	bool IsResultClause(string token, STRING_STRING_MAP declared_var_names);
	bool IsTuple(string token, STRING_STRING_MAP declared_var_names);
	bool IsWithClause(string token, STRING_STRING_MAP declared_var_names);
	bool IsSuchThatClause(string token, STRING_STRING_MAP declared_var_names);
	bool IsPatternClause(string token, STRING_STRING_MAP declared_var_names);

	bool IsAttrCond(string token, STRING_STRING_MAP declared_var_names);
	bool IsAttrCompare(string token, STRING_STRING_MAP declared_var_names);
	bool IsRef(string token, string synonym_type);
	bool IsAttrRef(string token, string synonym_type);

	//bool IsAttrCond(string token, STRING_STRING_MAP declared_var_names);
	//bool IsAttrCompare(string token, STRING_STRING_MAP declared_var_names);
	//bool IsRef(string token, string synonym_type);
	//bool IsAttrRef(string token);

	bool IsRelCond(string token, STRING_STRING_MAP declared_var_names);
	bool IsRelRef(string token, STRING_STRING_MAP declared_var_names);
	string GetRelRefType(string token, STRING_STRING_MAP declared_var_names);

	bool IsModifiesP(string token, STRING_STRING_MAP declared_var_names);
	bool IsModifiesS(string token, STRING_STRING_MAP declared_var_names);
	bool IsUsesP(string token, STRING_STRING_MAP declared_var_names);
	bool IsUsesS(string token, STRING_STRING_MAP declared_var_names);
	bool IsCalls(string token, STRING_STRING_MAP declared_var_names);
	bool IsCallsT(string token, STRING_STRING_MAP declared_var_names);
	bool IsParent(string token, STRING_STRING_MAP declared_var_names);
	bool IsParentT(string token, STRING_STRING_MAP declared_var_names);
	bool IsFollows(string token, STRING_STRING_MAP declared_var_names);
	bool IsFollowsT(string token, STRING_STRING_MAP declared_var_names);
	bool IsNext(string token, STRING_STRING_MAP declared_var_names);
	bool IsNextT(string token, STRING_STRING_MAP declared_var_names);
	bool IsAffects(string token, STRING_STRING_MAP declared_var_names);
	bool IsAffectsT(string token, STRING_STRING_MAP declared_var_names);

	bool IsPatternCond(string token, STRING_STRING_MAP declared_var_names);
	bool IsPattern(string token, STRING_STRING_MAP declared_var_names);
	bool IsAssign(string token, STRING_STRING_MAP declared_var_names);
	bool IsExpressionSpec(string token);
	bool IsOperator(char c);
	bool IsExpr(string token);
	bool IsTerm(string token);
	bool IsFactor(string token);
	bool IsVarName(string token);
	bool IsConstValue(string token);
	bool IsSynIf(string token, STRING_STRING_MAP declared_var_names);
	bool IsSynWhile(string token, STRING_STRING_MAP declared_var_names);
};
