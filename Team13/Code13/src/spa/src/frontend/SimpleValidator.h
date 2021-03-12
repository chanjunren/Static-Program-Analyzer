#ifndef _SIMPLEVALIDATOR_H_
#define _SIMPLEVALIDATOR_H_

#include <vector>
#include "FrontendTypes.h"

#define SHOW_STMT_INDX false

using namespace std;

class SimpleValidator {
public:
	SimpleValidator();

	int token_index_;
	int statement_index_;

	string curr_proc_;
	TOKEN_LIST token_list_;
	PROC_ADJ_LIST proc_adj_list_;

	bool IsValid(TOKEN_LIST token_list);

private:
	bool IsValidStmt();
	bool IsValidProc();
	bool IsValidReadStmt();
	bool IsValidPrintStmt();
	bool IsValidIfBlock();
	bool IsValidWhileBlock();
	bool IsValidCallStmt();
	bool IsValidAssignment(Token name_token);
	bool IsValidStmtList();
	bool IsValidExpression(TOKEN_LIST expr_list);

	vector<Token> GetExpressionTokens(ExpressionType expr_type);
	int GetEndIndxOfExpression(ExpressionType expr_type);

	Token GetNextToken();
	Token PeekNextToken();
	int GetEndIndxOfStatementList();

	void CheckForCyclicCalls();
};

#endif