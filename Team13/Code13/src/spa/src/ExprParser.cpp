#include <iostream>
#include <queue>
#include <stack>
#include <vector>

#include <CustomTypes.h>
#include <ExprParser.h>
#include <ExprValidator.h>
#include <FrontendTypes.h>
#include <pkb/TNode.h>
#include <testUtils/TreeTraverse.h>
#include <Token.h>
#include <TokenType.h>
#include <ExprValidator.h>

using namespace std;

ExprParser::ExprParser(TOKEN_LIST expr_list) {

	token_list_ = expr_list;
}

TNode* ExprParser::Parse() {
	TNode* root_node = ParseQueue(Shunt());
	ExprValidator::Validate(root_node);

	return root_node;
}

TNode* ExprParser::ParseQueue( SHUNTING_QUEUE shunted_queue ) {

	stack<tuple<Token, TNode*>> t_stack;

	tuple<Token, TNode*> tup = shunted_queue.front();
	Token token = get<0>(tup);
	TNode* root_node_ptr = get<1>(tup);

	while (shunted_queue.size() > 0) {

		tup = shunted_queue.front();

		token = get<0>(tup);

		shunted_queue.pop();

		if (token.GetTokenType() == TokenType::TOKEN_TYPE::constant || token.GetTokenType() == TokenType::TOKEN_TYPE::var) {
			t_stack.push(tup);
		}
		//guaranteed to be an expr
		else {

			//CREATE PARENT NODE
			TNode* parent_node_ptr = get<1>(tup);
			root_node_ptr = parent_node_ptr;

			/**
			* Check size of stack, if size == 1, then is unary
			*/
			if (token.is_unary_op_) {

				//CREATE SINGLE CHILD NODE
				TNode* c_node_ptr = get<1>(t_stack.top());
				t_stack.pop();

				//LINK PARENT TO CHILD
				parent_node_ptr->AddChild(c_node_ptr);

			}
			else {

				//CREATE RIGHT CHILD NODE
				TNode* r_node_ptr = get<1>(t_stack.top());
				t_stack.pop();

				//CREATE LEFT CHILD NODE
				TNode* l_node_ptr = get<1>(t_stack.top());
				t_stack.pop();


				parent_node_ptr->AddChild(l_node_ptr);
				parent_node_ptr->AddChild(r_node_ptr);
			}

			t_stack.push(tup);

		}


	}

	if (root_node_ptr != NULL) {
		return root_node_ptr;
	}
	return NULL;

}

queue<tuple<Token, TNode*>> ExprParser::Shunt() {
	queue<tuple<Token, TNode*>> output_queue;
	stack<Token> op_stack;

	int i = 0;

	while( i < token_list_.size()){


		Token token = token_list_.at(i);
		
		TokenType::TOKEN_TYPE token_type = token.GetTokenType();

		if (token_type == TokenType::TOKEN_TYPE::constant || token_type == TokenType::TOKEN_TYPE::var) {
			output_queue.push(make_tuple(token, ConvertTokenToNode(token)));
		}
		else if (token_type == TokenType::TOKEN_TYPE::expr|| token_type == TokenType::TOKEN_TYPE::rel_expr || token_type == TokenType::TOKEN_TYPE::cond_expr) {

			while (op_stack.size() > 0
				&& (
					CompareOpPrecedence(token, op_stack.top()) == -1
					|| (
						CompareOpPrecedence(token, op_stack.top()) == 0
						&& IsLeftAssoc(token)
						)
					)
				&& op_stack.top().GetValue() != TYPE_PUNC_OPEN_PARAN
				) {


				Token op_token = op_stack.top();
				op_stack.pop();
				output_queue.push(make_tuple(op_token, ConvertTokenToNode(op_token)));	
			}

			op_stack.push(token);
		}
		else if ( (token.GetValue() == TYPE_PUNC_OPEN_PARAN) || i >= 1 && token.is_unary_op_)
		{
			op_stack.push(token);
		}
		else if (token.GetValue() == TYPE_PUNC_CLOSED_PARAN) {

			while (op_stack.top().GetValue() != TYPE_PUNC_OPEN_PARAN) {
				if (op_stack.size() == 0) {
					throw logic_error("Mismatched parantheses!");
				}

				Token op_token = op_stack.top();
				op_stack.pop();
				output_queue.push(make_tuple(op_token, ConvertTokenToNode(op_token)));

			}

			if (op_stack.top().GetValue() == TYPE_PUNC_OPEN_PARAN) {
				op_stack.pop();
			}
		}

		i++;
	}

	while (op_stack.size() > 0) {
		Token op_token = op_stack.top();
		op_stack.pop();
		output_queue.push(make_tuple(op_token, ConvertTokenToNode(op_token)));
	}

	return output_queue;
}

bool ExprParser::IsLeftAssoc(Token t) {

	string val = t.GetValue();

	for (string s : non_left_assoc_list) {
		if (s == val) {
			return false;
		}
	}

	return true;

}

int ExprParser::CompareOpPrecedence(Token a, Token b) {

	int a_precedence = GetPrecedence(a);
	int b_precedence = GetPrecedence(b);

	if (a_precedence == b_precedence) {
		return 0;
	}
	else if (a_precedence < b_precedence) {
		return -1;
	}
	else {
		return 1;
	}
}

int ExprParser::GetPrecedence(Token t) {
	string val = t.GetValue();

	if (t.is_unary_op_) {
		return PRECEDENCE_UNARY;
	}

	return precedence_map_[val];
}

TNode::OPERATOR ExprParser::GetOperator(string op_str) {

	if (op_str == TYPE_COND_EXPR_NOT) {
		return TNode::OPERATOR::notOp;
	}
	else if (op_str == TYPE_COND_EXPR_AND) {
		return TNode::OPERATOR::andOp;
	}
	else if (op_str == TYPE_COND_EXPR_OR) {
		return TNode::OPERATOR::orOp;
	}
	else if (op_str == TYPE_REL_EXPR_GT) {
		return TNode::OPERATOR::greater;
	}
	else if (op_str == TYPE_REL_EXPR_GTE) {
		return TNode::OPERATOR::geq;
	}
	else if (op_str == TYPE_REL_EXPR_LT) {
		return TNode::OPERATOR::lesser;
	}
	else if (op_str == TYPE_REL_EXPR_LTE) {
		return TNode::OPERATOR::notOp;
	}
	else if (op_str == TYPE_REL_EXPR_EQ) {
		return TNode::OPERATOR::equal;
	}
	else if (op_str == TYPE_REL_EXPR_NEQ) {
		return TNode::OPERATOR::unequal;
	}
	else if (op_str == TYPE_EXPR_PLUS) {
		return TNode::OPERATOR::plus;
	}
	else if (op_str == TYPE_EXPR_MINUS) {
		return TNode::OPERATOR::minus;
	}
	else if (op_str == TYPE_EXPR_TIMES) {
		return TNode::OPERATOR::times;
	}
	else if (op_str == TYPE_EXPR_DIVIDE) {
		return TNode::OPERATOR::divide;
	}
	else if (op_str == TYPE_EXPR_MOD) {
		return TNode::OPERATOR::mod;
	}
	else {
		throw logic_error("Invalid token heree" );
	}

}

TNode* ExprParser::ConvertTokenToNode(Token t) {

	//token can only be expr (+, -, * etc), rel_expr (&&, || etc), variable, constant

	if (t.GetValue() == TYPE_PUNC_OPEN_PARAN || t.GetValue() == TYPE_PUNC_CLOSED_PARAN) {
		return new TNode();
	}
	if (t.GetTokenType() == TokenType::TOKEN_TYPE::expr) {
		return new TNode(TNode::NODE_TYPE::expr, GetOperator(t.GetValue()));
	}
	else if (t.GetTokenType() == TokenType::TOKEN_TYPE::rel_expr) {
		return new TNode(TNode::NODE_TYPE::relExpr, GetOperator(t.GetValue()));
	}
	else if (t.GetTokenType() == TokenType::TOKEN_TYPE::var) {
		return new TNode(TNode::NODE_TYPE::varName, t.GetValue());
	}
	else if (t.GetTokenType() == TokenType::TOKEN_TYPE::constant) {
		return new TNode(TNode::NODE_TYPE::constValue, stod(t.GetValue()));
	}
	else if (t.GetTokenType() == TokenType::TOKEN_TYPE::cond_expr) {
		return new TNode(TNode::NODE_TYPE::condExpr, GetOperator(t.GetValue()));
	}
	else {
		throw logic_error("Invalid token laala ");
	}

};
