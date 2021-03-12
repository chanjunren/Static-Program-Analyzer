#include "PQLCustomTypes.h"

#pragma once

/*
The QuerySyntacticValidator only validates if a given query is valid syntactically and does not check for Semantics.

*/
class QuerySyntacticValidator
{
public:
	QuerySyntacticValidator(); 

	bool ValidateDeclaration(string token);
	bool ValidateSuchThatClause(string token);

	bool ValidatePatternClause(string token, STRING_STRING_MAP declared_var_names);

	bool ValidateWithClause(string token);

	bool IsRelRef(string token);
	bool IsValidRelRefArgument(string token);

	bool IsValidStmtRef(string token);
	bool IsValidEntRef(string token);

	bool IsIdent(string token);
	bool IsName(string token);
	bool IsInteger(string token);
	bool IsSynonym(string token);

	bool IsAttrRef(string token);
	bool IsRef(string token);

	bool IsExpr(string token);
	bool IsOperator(char c);
	bool IsExpressionSpec(string token);
};

