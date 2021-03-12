#ifndef _EXPRPARSER_H
#define _EXPRPARSER_H

#include <queue>
#include <unordered_map>
#include <vector>

#include "FrontendTypes.h"
#include <pkb/TNode.h>
#include "Token.h"

using namespace std;

#define PRECEDENCE_PARAN 7
#define PRECEDENCE_UNARY 6
#define PRECEDENCE_TIMES_DIVIDE_MOD 5
#define PRECEDENCE_PLUS_MINUS 4
#define PRECEDENCE_GT_LT_GTE_LTE 3
#define PRECEDENCE_EQ_NEQ 2
#define PRECEDENCE_AND 1
#define PRECEDENCE_OR 0

typedef unordered_map<string, int> PRECEDENCE_MAP;
typedef queue<tuple<Token, TNode*>> SHUNTING_QUEUE;

class ExprParser {
public:
	ExprParser(TOKEN_LIST expr_list);

	TNode* Parse();

	PRECEDENCE_MAP precedence_map_ = {
	{TYPE_PUNC_OPEN_PARAN, PRECEDENCE_PARAN},
	{TYPE_PUNC_CLOSED_PARAN, PRECEDENCE_PARAN},
	{TYPE_COND_EXPR_NOT, PRECEDENCE_UNARY},
	{TYPE_EXPR_TIMES, PRECEDENCE_TIMES_DIVIDE_MOD},
	{TYPE_EXPR_DIVIDE, PRECEDENCE_TIMES_DIVIDE_MOD},
	{TYPE_EXPR_MOD, PRECEDENCE_TIMES_DIVIDE_MOD},
	{TYPE_EXPR_PLUS, PRECEDENCE_PLUS_MINUS},
	{TYPE_EXPR_MINUS, PRECEDENCE_PLUS_MINUS},
	{TYPE_REL_EXPR_GT, PRECEDENCE_GT_LT_GTE_LTE},
	{TYPE_REL_EXPR_LT, PRECEDENCE_GT_LT_GTE_LTE},
	{TYPE_REL_EXPR_LTE, PRECEDENCE_GT_LT_GTE_LTE},
	{TYPE_REL_EXPR_GTE, PRECEDENCE_GT_LT_GTE_LTE},
	{TYPE_REL_EXPR_EQ, PRECEDENCE_EQ_NEQ},
	{TYPE_REL_EXPR_NEQ, PRECEDENCE_EQ_NEQ},
	{TYPE_COND_EXPR_AND, PRECEDENCE_AND},
	{TYPE_COND_EXPR_OR, PRECEDENCE_OR},
	};

	STR_LIST non_left_assoc_list = {
		TYPE_PUNC_OPEN_PARAN,
		TYPE_PUNC_CLOSED_PARAN,
		TYPE_REL_EXPR_GT,
		TYPE_REL_EXPR_GTE,
		TYPE_REL_EXPR_LT,
		TYPE_REL_EXPR_LTE,
		TYPE_COND_EXPR_NOT
	};

private:
	TOKEN_LIST token_list_;
	
	int CompareOpPrecedence(Token a, Token b);
	TNode* ConvertTokenToNode(Token t);
	TNode* ParseQueue(SHUNTING_QUEUE shunted_queue);
	TNode::OPERATOR GetOperator(string op_str);
	int GetPrecedence(Token t);
	bool IsLeftAssoc(Token t);
	SHUNTING_QUEUE Shunt();
};

#endif