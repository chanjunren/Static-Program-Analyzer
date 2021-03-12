#include <stdexcept>
#include <string>

#include "FrontendTypes.h"
#include <CustomTypes.h>
#include "TokenType.h"

using namespace std;

TokenType::STMT_TYPE TokenType::GetStmtType(string token_str) {

	if (token_str == TYPE_PROC) {
		return TokenType::STMT_TYPE::_procedure;
	}

	if (token_str == TYPE_STMT_CALL) {
		return TokenType::STMT_TYPE::_call;
	}

	if (token_str == TYPE_STMT_READ) {
		return TokenType::STMT_TYPE::_read;
	}

	if (token_str == TYPE_STMT_WHILE) {
		return TokenType::STMT_TYPE::_while;
	}

	if (token_str == TYPE_STMT_PRINT) {
		return TokenType::STMT_TYPE::_print;
	}

	if (token_str == TYPE_STMT_IF) {
		return TokenType::STMT_TYPE::_if;
	}

	if (token_str == TYPE_STMT_IF_THEN) {
		return TokenType::STMT_TYPE::_then;
	}

	if (token_str == TYPE_STMT_IF_ELSE) {
		return TokenType::STMT_TYPE::_else;
	}

	throw logic_error("No such token type!");

}

