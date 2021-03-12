#include <iostream>
#include <string>

#include "Token.h"
#include "TokenType.h"

using namespace std;

Token::Token(string tokenVal, TokenType::TOKEN_TYPE token_type) {
	value_ = tokenVal;
	type_ = token_type;
	is_unary_op_ = false;
}

Token::Token(string tokenVal, TokenType::TOKEN_TYPE token_type, bool is_unary_op ) {
	value_ = tokenVal;
	type_ = token_type;
	is_unary_op_ = is_unary_op;
}

Token::Token(){}


string Token::GetValue() {
	return value_;
}

TokenType::TOKEN_TYPE Token::GetTokenType() {
	return type_;
}

void Token::SetTokenType(TokenType::TOKEN_TYPE token_type) {
	type_ = token_type;
}


TokenType::STMT_TYPE Token::GetStmtType() {
	return stmt_type_;
}

string Token::GetTokenTypeStr() {

	switch (type_) {
	case TokenType::TOKEN_TYPE::expr:
		return TOKEN_TYPE_EXPR;
	case TokenType::TOKEN_TYPE::rel_expr:
		return TOKEN_TYPE_REL_EXPR;
	case TokenType::TOKEN_TYPE::cond_expr:
		return TOKEN_TYPE_COND_EXPR;
	case TokenType::TOKEN_TYPE::punc:
		return TOKEN_TYPE_PUNC;
	case TokenType::TOKEN_TYPE::constant:
		return TOKEN_TYPE_CONSTANT;
	case TokenType::TOKEN_TYPE::var:
		return TOKEN_TYPE_VAR;
	case TokenType::TOKEN_TYPE::stmt:
		return TOKEN_TYPE_STMT;
	case TokenType::TOKEN_TYPE::assign:
		return TOKEN_TYPE_ASSIGN;
	default:
		return "";
	}

}