#include <iostream>
#include <queue>
#include <stdexcept>
#include <stack>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <stack>
#include <vector>
#include <map>
#include <set>

#include <SimpleValidator.h>
#include <ExprTokensValidator.h>
#include <TokenType.h>
#include <Token.h>
#include <FrontendTypes.h>

using namespace std;

SimpleValidator::SimpleValidator() {}

bool SimpleValidator::IsValid(TOKEN_LIST token_list) {
	token_list_ = token_list;
	token_index_ = -1;
	statement_index_ = 0;
 	if (token_list.size() == 0) {
		throw "Program cannot be empty";
	}

	while (token_index_ < (int)token_list_.size() - 1) {
		if (SimpleValidator::GetNextToken().GetValue() != "procedure") {
			cout << "(Line " << statement_index_ << ")";
			throw "Missing procedure";
		}
		SimpleValidator::IsValidProc();
	}

	map<string, vector<string>>::iterator it;

	for (it = proc_adj_list_.begin(); it != proc_adj_list_.end(); it++) {
		for (int i = 0; i < it->second.size(); i++) {
			if (!proc_adj_list_.count(it->second[i])) {
				// Key does not exist
				cout << "(" << it->second[i] << ") ";
				throw "Undefined procedure ";
			}
		}
	}
	CheckForCyclicCalls();
	return true;
}

bool SimpleValidator::IsValidStmt() {
	if (SHOW_STMT_INDX) {
		cout << endl << "Line " << statement_index_ + 1 << ": ";
	}
	Token first_token = SimpleValidator::GetNextToken();
	statement_index_++;

	switch (first_token.GetTokenType()) {
	case TokenType::TOKEN_TYPE::stmt:
		switch (first_token.GetStmtType()) {
		case TokenType::STMT_TYPE::_read:
			return SimpleValidator::IsValidReadStmt();
		case TokenType::STMT_TYPE::_print:
			return SimpleValidator::IsValidPrintStmt();
		case TokenType::STMT_TYPE::_call:
			return SimpleValidator::IsValidCallStmt();
		case TokenType::STMT_TYPE::_if:
			return SimpleValidator::IsValidIfBlock();
		case TokenType::STMT_TYPE::_while:
			return SimpleValidator::IsValidWhileBlock();
		case TokenType::STMT_TYPE::_procedure:
			throw "Cannot define a procedure within a procedure";
		default:
			throw "Unhandled statement name";
		}
	case TokenType::TOKEN_TYPE::var: // For assignment statements
		return SimpleValidator::IsValidAssignment(first_token);
	default:
		cout << "(" << first_token.GetValue() << ") ";
		throw "Unhandled Token";
	}
}

bool SimpleValidator::IsValidProc() {
	Token name_token = SimpleValidator::GetNextToken();
	if (proc_adj_list_.count(name_token.GetValue()) != 0) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Redefining procedure";
	}
	curr_proc_ = name_token.GetValue();
	proc_adj_list_.insert({name_token.GetValue(), vector<DFS_NODE>()});

	if (name_token.GetTokenType() != TokenType::TOKEN_TYPE::var &&
		name_token.GetTokenType() != TokenType::TOKEN_TYPE::stmt) {
		cout << "(Line: " << statement_index_ << ") ";

		throw "Invalid procedure name";
	}

	return SimpleValidator::IsValidStmtList();
}

bool SimpleValidator::IsValidReadStmt() {
	Token name_token = SimpleValidator::GetNextToken();

	if (name_token.GetTokenType() != TokenType::TOKEN_TYPE::var &&
		name_token.GetTokenType() != TokenType::TOKEN_TYPE::stmt) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Invalid variable name for read statement";
	}

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing ';'";
	}

	return true;
}

bool SimpleValidator::IsValidPrintStmt() {
	Token name_token = SimpleValidator::GetNextToken();

	if (name_token.GetTokenType() != TokenType::TOKEN_TYPE::var &&
		name_token.GetTokenType() != TokenType::TOKEN_TYPE::stmt) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Invalid variable name for print statement";
	}

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing ';'";
	}

	return true;
}

bool SimpleValidator::IsValidCallStmt() {
	Token name_token = SimpleValidator::GetNextToken();
	
	if (name_token.GetTokenType() != TokenType::TOKEN_TYPE::var &&
		name_token.GetTokenType() != TokenType::TOKEN_TYPE::stmt) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Invalid variable name for call statement";
	}

	// Updating proc_adj_list
	vector<string> lst = proc_adj_list_.at(curr_proc_);
	lst.push_back(name_token.GetValue());
	proc_adj_list_.at(curr_proc_) = lst;

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing ';'";
	}

	return true;
}

bool SimpleValidator::IsValidIfBlock() {
	if (!SimpleValidator::IsValidExpression(GetExpressionTokens(ExpressionType::_if))) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Invalid expression in if block";
	}
	
	if (SimpleValidator::GetNextToken().GetValue() != TYPE_STMT_IF_THEN) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing 'then'";
	}

	SimpleValidator::IsValidStmtList();

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_STMT_IF_ELSE) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing 'else'";
	}

	SimpleValidator::IsValidStmtList();
	
	return true;
}

bool SimpleValidator::IsValidWhileBlock() {
	if (!SimpleValidator::IsValidExpression(GetExpressionTokens(ExpressionType::_while))) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Invalid expression in while block";
	}

	SimpleValidator::IsValidStmtList();
	return true;
}

bool SimpleValidator::IsValidAssignment(Token name_token) {
	if (SimpleValidator::GetNextToken().GetValue() != TYPE_EQUALS) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing '='";
	}

	if (!SimpleValidator::IsValidExpression(GetExpressionTokens(_assign))) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Invalid expression in assignment";
	}

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing ';'";
	}
	return true;
}


bool SimpleValidator::IsValidStmtList() {
	int end_index = SimpleValidator::GetEndIndxOfStatementList();
	int initial_indx = statement_index_;
	while ((token_index_) < end_index) {
		if (SimpleValidator::PeekNextToken().GetValue() == TYPE_PUNC_OPEN_BRACKET ||
			SimpleValidator::PeekNextToken().GetValue() == TYPE_PUNC_CLOSED_BRACKET) {
			SimpleValidator::GetNextToken();
			continue;
		}
		SimpleValidator::IsValidStmt();
	}
	if (initial_indx == statement_index_) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Empty statement block detected";
	}
	return true;
}

bool SimpleValidator::IsValidExpression(TOKEN_LIST expr_list) {
	if (!ExprTokensValidator::Validate(expr_list)) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Invalid expression";
	}
	return true;
}

// Returns the number of tokens inside a statement list
int SimpleValidator::GetEndIndxOfStatementList() {
	stack <Token> bracket_matcher;
	Token first_token = SimpleValidator::PeekNextToken();
	int counter = 1;

	if (first_token.GetValue() == TYPE_PUNC_OPEN_BRACKET) {
		bracket_matcher.push(first_token);
		while (!bracket_matcher.empty()) {
			if (token_index_ + 1 + counter > (int)token_list_.size()) {
				// throw Exception for incomplete statement block
				throw "Incomplete statement block";
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

Token SimpleValidator::GetNextToken() {
	token_index_++;

	if (token_index_ < token_list_.size()) {
		if (SHOW_STMT_INDX) {
			cout << token_list_[token_index_].GetValue() << " ";
		}
		return token_list_[token_index_];
	}
	else {
		throw "Out of range.";
	}
}

Token SimpleValidator::PeekNextToken() {
	if (token_index_ + 1 < token_list_.size()) {
		return token_list_[token_index_ + 1];
	}
	else {
		throw "Out of range.";
	}
}

vector<Token> SimpleValidator::GetExpressionTokens(ExpressionType expr_type) {
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

int SimpleValidator::GetEndIndxOfExpression(ExpressionType expr_type) {
	int end_index = token_index_;
	string delimiter;
	if (expr_type == ExpressionType::_if) {
		// End index is before 'then' token
		delimiter = "then";
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
		if (end_index + 1 < token_list_.size()) {
			end_index++; 
		}
		else {
			throw "Error when GetEndIndxOfExpression";
		}
	}
	return end_index;
}

void SimpleValidator::CheckForCyclicCalls() {
	int node_count = 0;
	vector<DFS_NODE> white_set; // Univisited
	vector<DFS_NODE> gray_set; // Visiting
	vector<DFS_NODE> black_set; // Visited

	map<DFS_NODE, DFS_NODE> parent_map;

	for (map<string, vector<string>>::iterator it = proc_adj_list_.begin(); 
		it != proc_adj_list_.end(); ++it ) { // Adding all nodes to white set
		white_set.push_back(it->first);
		node_count++;
	}

	bool cycleDetected = false;
	DFS_NODE curr_node;
	curr_node = *white_set.begin();
	white_set.erase(white_set.begin()); // Popping first item from set
	gray_set.push_back(curr_node);
	parent_map.insert({ curr_node, "-1" });
	while ((black_set.size() != node_count) && !cycleDetected) {
		vector<DFS_NODE> adj_list = proc_adj_list_.at(curr_node);
		bool node_visited = false;
		for (DFS_NODE node : adj_list) {
			if (find(white_set.begin(), white_set.end(), node) != white_set.end()) { // Node has not been visited
				parent_map.insert({node, curr_node}); // callee, caller
				curr_node = node;
				white_set.erase(find(white_set.begin(), white_set.end(), node));
				gray_set.push_back(node);
				node_visited = true;
				break;
			}
			else if (find(gray_set.begin(), gray_set.end(), node) != gray_set.end()) {
				cycleDetected = true;
				break;
			}
			// Last case is if it's in the black_set, and dunnid to handle so just ignore
		}
		if (!node_visited) {
			// Node has been fully processed, add to black set
			black_set.push_back(curr_node);
			if (black_set.size() == node_count) {
				break;
			}
			gray_set.erase(find(gray_set.begin(), gray_set.end(), curr_node));
			if (gray_set.empty()) {
				parent_map.clear();
				curr_node = *white_set.begin();
				white_set.erase(white_set.begin()); // Popping first item from set		
				gray_set.push_back(curr_node);
			}
			else {
				curr_node = *(gray_set.rbegin());
			}
		} 
	}
	if (cycleDetected) {
		throw "Recursive call detected";
	}
}
