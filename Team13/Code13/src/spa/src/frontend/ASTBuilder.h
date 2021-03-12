#ifndef _ASTBUILDER_H_
#define _ASTBUILDER_H_

#include <vector>

#include <FrontendTypes.h>
#include <TNode.h>
#include "Token.h"

using namespace std;

class ASTBuilder {
public:

	ASTBuilder();

	int statement_index_;
	int token_index_;
	TOKEN_LIST token_list_;

	TNode* BuildMainPrgNode(TOKEN_LIST token_list);

private:
	TNode* BuildStmtNode();
	TNode* BuildProcNode();
	TNode* BuildReadNode();
	TNode* BuildPrintNode();
	TNode* BuildIfNode();
	TNode* BuildWhileNode();
	TNode* BuildCallNode();
	TNode* BuildAssignNode(Token name_token);
	TNode* BuildStmtListNode();
	TNode* BuildExpressionNode(ExpressionType expr_type);

	vector<Token> GetExpressionTokens(ExpressionType expr_type);
	int GetEndIndxOfExpression(ExpressionType expr_type);

	Token GetNextToken();
	Token PeekNextToken();
	int GetEndIndxOfStatementList();
};

#endif