#include <iostream>
#include <queue>
#include <stdexcept>
#include <stack>
#include <vector>

#include <ExprParser.h>
#include <ASTBuilder.h>
#include <TNode.h>
#include <TokenType.h>
#include <Token.h>
#include <FrontendTypes.h>
#include <ExprValidator.h>

using namespace std;

ASTBuilder::ASTBuilder() {}

TNode* ASTBuilder::BuildMainPrgNode(TOKEN_LIST token_list) {
	token_list_ = token_list;
	token_index_ = -1;
	statement_index_ = 0;

	TNode* program_node = new TNode(TNode::NODE_TYPE::program);

	while (token_index_ < (int)token_list_.size() - 1) {
		Token proc_token = GetNextToken();

		if (proc_token.GetValue() != TYPE_PROC) {
			throw "Missing procedure at line ";
		}

		TNode* proc_node = ASTBuilder::BuildProcNode();
		program_node->AddChild(proc_node);
	}

	return program_node;
}

// Returns a statement node
TNode* ASTBuilder::BuildStmtNode() {
	Token first_token = ASTBuilder::GetNextToken();
	statement_index_++;

	switch (first_token.GetTokenType()) {
	case TokenType::TOKEN_TYPE::stmt:
		switch (first_token.GetStmtType()) {
		case TokenType::STMT_TYPE::_read:
			return ASTBuilder::BuildReadNode();
		case TokenType::STMT_TYPE::_print:
			return ASTBuilder::BuildPrintNode();
		case TokenType::STMT_TYPE::_call:
			return ASTBuilder::BuildCallNode();
		case TokenType::STMT_TYPE::_if:
			return ASTBuilder::BuildIfNode();
		case TokenType::STMT_TYPE::_while:
			return ASTBuilder::BuildWhileNode();
		case TokenType::STMT_TYPE::_procedure:
			throw "Cannot define a procedure within a procedure";
		default:
			throw "Validation error thrown in ASTBuilder at line ";
		}
	case TokenType::TOKEN_TYPE::var: // For assignment statements
		return ASTBuilder::BuildAssignNode(first_token);
	default:
		throw "Validation error thrown in ASTBuilder for the token: " + first_token.GetValue();
	}
}

TNode* ASTBuilder::BuildProcNode() {
	Token name_token = ASTBuilder::GetNextToken();

	TNode* proc_node = new TNode(TNode::NODE_TYPE::procedure);
	TNode* proc_name_node = new TNode(TNode::NODE_TYPE::procName, name_token.GetValue());
	TNode* stmt_list_node = ASTBuilder::BuildStmtListNode();

	if (!proc_node->AddChild(proc_name_node) || !proc_node->AddChild((stmt_list_node))) {
		throw logic_error("Null node added to child of proc node");
	}

	return proc_node;
}

TNode* ASTBuilder::BuildReadNode() {

	Token var_token = ASTBuilder::GetNextToken();

	TNode* read_node = new TNode(TNode::NODE_TYPE::readStmt, statement_index_);
	TNode* var_node = new TNode(TNode::NODE_TYPE::varName, var_token.GetValue());

	ASTBuilder::GetNextToken(); // Pop ';' token

	if (!read_node->AddChild(var_node)) {
		throw "Null node added as child of read node";
	}

	return read_node;
}

TNode* ASTBuilder::BuildPrintNode() {
	Token var_token = ASTBuilder::GetNextToken();
	TNode* print_node = new TNode(TNode::NODE_TYPE::printStmt, statement_index_);
	TNode* var_node = new TNode(TNode::NODE_TYPE::varName, var_token.GetValue());

	ASTBuilder::GetNextToken(); // Pop ';' token

	if (!print_node->AddChild(var_node)) {
		throw "Null node added to child of print node at line ";
	}

	return print_node;
}

TNode* ASTBuilder::BuildCallNode() {
	Token proc_name_token = ASTBuilder::GetNextToken();
	TNode* call_node = new TNode(TNode::NODE_TYPE::callStmt, statement_index_);
	TNode* proc_name_node = new TNode(TNode::NODE_TYPE::procName, proc_name_token.GetValue());

	ASTBuilder::GetNextToken(); // Pop ';' token

	if (!call_node->AddChild(proc_name_node)) {
		throw logic_error("Null node added to child of call node");
	}
	return call_node;
}

TNode* ASTBuilder::BuildIfNode() {
	TNode* if_node = new TNode(TNode::NODE_TYPE::ifStmt, statement_index_);
	TNode* expr_node = ASTBuilder::BuildExpressionNode(_FRONTENDTYPES_H_::ExpressionType::_if);
	// Hmm wrong place to put? 
	if (expr_node->GetNodeType() != TNode::NODE_TYPE::relExpr && expr_node->GetNodeType() != TNode::NODE_TYPE::condExpr) {
		throw "Invalid expression at ";
	}
	TNode* thenStmtListNode = ASTBuilder::BuildStmtListNode();

	for (TNode* child : thenStmtListNode->GetChildrenVector()) {
		child->SetParent(if_node);
	}

	if (!if_node->AddChild(expr_node) || !if_node->AddChild(thenStmtListNode)) {
		throw "Null node added to child of if node at line ";
	}

	ASTBuilder::GetNextToken(); // Pop 'else' token
	
	TNode* elseStmtListNode = ASTBuilder::BuildStmtListNode();
	
	for (TNode* child : elseStmtListNode->GetChildrenVector()) {
		child->SetParent(if_node);
	}

	if (!if_node->AddChild(elseStmtListNode)) {
		throw "Null node added to child of if node at line ";
	}

	return if_node;
}

TNode* ASTBuilder::BuildWhileNode() {
	TNode* whleNode = new TNode(TNode::NODE_TYPE::whileStmt, statement_index_);
	TNode* expr_node = ASTBuilder::BuildExpressionNode(_FRONTENDTYPES_H_::ExpressionType::_while);
	TNode* stmt_list_node = ASTBuilder::BuildStmtListNode();

	if (expr_node->GetNodeType() != TNode::NODE_TYPE::relExpr && expr_node->GetNodeType() != TNode::NODE_TYPE::condExpr) {
		throw "Invalid expression at line ";
	}

	for (TNode* child : stmt_list_node->GetChildrenVector()) {
		child->SetParent(whleNode);
	}

	if (!whleNode->AddChild(expr_node) || !whleNode->AddChild(stmt_list_node)) {
		throw "Null node added to child of while node at line ";
	}

	return whleNode;
}


TNode* ASTBuilder::BuildAssignNode(Token name_token) {
	TNode* assign_node = new TNode(TNode::NODE_TYPE::assignStmt, statement_index_);
	TNode* name_node = new TNode(TNode::NODE_TYPE::varName, name_token.GetValue());
	
	ASTBuilder::GetNextToken(); // Pop '=' token

	TNode* expr_node = ASTBuilder::BuildExpressionNode(_FRONTENDTYPES_H_::ExpressionType::_assign);

	ASTBuilder::GetNextToken(); // Pop ';' token
	
	if (!assign_node->AddChild(name_node) || !assign_node->AddChild(expr_node)) {
		throw logic_error("Null node added to child of assign node");
	}

	return assign_node;
}


// Returns a stmtList node
TNode* ASTBuilder::BuildStmtListNode() {
	int end_index = ASTBuilder::GetEndIndxOfStatementList();
	
 	TNode* stmt_list_node = new TNode(TNode::NODE_TYPE::stmtList);
	// Parse the whole statement block
	
	while ((token_index_) < end_index) {

		if (PeekNextToken().GetValue() == TYPE_PUNC_OPEN_BRACKET ||
			PeekNextToken().GetValue() == TYPE_PUNC_CLOSED_BRACKET) {
			GetNextToken();
			continue;
		}
	
		stmt_list_node->AddChild(ASTBuilder::BuildStmtNode());
	}

	return stmt_list_node;
}

TNode* ASTBuilder::BuildExpressionNode(ExpressionType expr_type) {
	TOKEN_LIST expr_tokens = GetExpressionTokens(expr_type);

	ExprParser expr_parser(expr_tokens);

	return expr_parser.Parse();
}

// Returns the number of tokens inside a statement list
int ASTBuilder::GetEndIndxOfStatementList() {
	stack <Token> bracket_matcher;
	Token first_token = ASTBuilder::PeekNextToken();
	int counter = 1;

	if (first_token.GetValue() == TYPE_PUNC_OPEN_BRACKET) {
		bracket_matcher.push(first_token);
		while (!bracket_matcher.empty()) {
			if (token_index_ + 1 + counter > (int)token_list_.size()) {
				// throw Exception for incomplete statement block
				throw logic_error("Incomplete statement block");
			}
			Token next_token = token_list_[token_index_ + 1 + counter];
			if (next_token.GetValue() == TYPE_PUNC_OPEN_BRACKET) {
				bracket_matcher.push(next_token);
			}
			if (next_token.GetValue() == TYPE_PUNC_CLOSED_BRACKET) {
				bracket_matcher.pop();
			}
			counter++;
		}
	}
	return token_index_ + counter;
}
Token ASTBuilder::GetNextToken() {
	token_index_++;

	if (token_index_ < token_list_.size()) {
		return token_list_[token_index_];
	}
	else {
		throw out_of_range("Out of range.");
	}
}

Token ASTBuilder::PeekNextToken() {
	if (token_index_ + 1 < token_list_.size()) {
		return token_list_[token_index_ + 1];
	}
	else {
		throw out_of_range("Out of range.");
	}
}

vector<Token> ASTBuilder::GetExpressionTokens(ExpressionType expr_type) {
	vector<Token> expr_list;
	int EndIndexOfTokens = GetEndIndxOfExpression(expr_type);
	while (token_index_ < EndIndexOfTokens) {
		Token next_token = GetNextToken();
		if (next_token.GetTokenType() == TokenType::TOKEN_TYPE::stmt) {
			next_token.SetTokenType(TokenType::TOKEN_TYPE::var);
		}
		expr_list.push_back(next_token);
	}
	return expr_list;
}

int ASTBuilder::GetEndIndxOfExpression(ExpressionType expr_type) {
	int end_index = token_index_;
	string delimiter;
	if (expr_type == ExpressionType::_if) {
		// End index is before 'then' token
		delimiter = "{";
	}
	else if (expr_type == ExpressionType::_assign) {
		// End index is before ';' token
		delimiter = ";";
	}
	else if (expr_type == ExpressionType::_while) {
		// End index is before '{' token
		delimiter = "{";
	}
	else {
		throw "Unhandled delimiter in SimpleValidator";
	}
	while (token_list_[end_index + 1].GetValue() != delimiter) {
		end_index++;
	}
	return end_index;
}
