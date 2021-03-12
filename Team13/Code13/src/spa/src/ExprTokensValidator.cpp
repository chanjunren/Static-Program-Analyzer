#include <iostream>

#include <CustomTypes.h>
#include <ExprTokensValidator.h>
#include <FrontendTypes.h>

using namespace std;

bool ExprTokensValidator::Validate(TOKEN_LIST token_list) {

	int prev_index = -1;
	int curr_index = 0;

	while (curr_index < token_list.size()) {

		if (prev_index > -1) {
			Token prev_token = token_list.at(prev_index);
			Token curr_token = token_list.at(curr_index);

			if (!ExprTokensValidator::CheckValid(prev_token, curr_token)) {
				return false;
			}
		}

		prev_index++;
		curr_index++;
	}


	return true;

}

bool ExprTokensValidator::CheckValid(Token prev_token, Token curr_token) {

	TokenType::TOKEN_TYPE prev_type = prev_token.GetTokenType();
	TokenType::TOKEN_TYPE curr_type = curr_token.GetTokenType();


	string prev_val = prev_token.GetValue();
	string curr_val = curr_token.GetValue();

	if (prev_type == TokenType::TOKEN_TYPE::expr) {
		if (curr_type != TokenType::TOKEN_TYPE::var && curr_type != TokenType::TOKEN_TYPE::constant && curr_val != TYPE_PUNC_OPEN_PARAN) {
			return false;
		}
	}

	if (prev_type == TokenType::TOKEN_TYPE::rel_expr) {
		if (curr_type != TokenType::TOKEN_TYPE::var && curr_type != TokenType::TOKEN_TYPE::constant && curr_val != TYPE_PUNC_OPEN_PARAN) {
			return false;
		}
	}

	if (prev_type == TokenType::TOKEN_TYPE::cond_expr) {
		if (curr_type != TokenType::TOKEN_TYPE::var && curr_type != TokenType::TOKEN_TYPE::constant && curr_val != TYPE_PUNC_OPEN_PARAN) {
			return false;
		}
	}

	if (prev_val == TYPE_PUNC_OPEN_PARAN) {
		if (curr_type != TokenType::TOKEN_TYPE::var && curr_type != TokenType::TOKEN_TYPE::constant && curr_val != TYPE_COND_EXPR_NOT && curr_val != TYPE_PUNC_OPEN_PARAN) {
			return false;
		}
	}

	if (prev_val == TYPE_PUNC_CLOSED_PARAN) {
		if (curr_type != TokenType::TOKEN_TYPE::cond_expr && curr_val != TYPE_PUNC_CLOSED_PARAN && curr_type != TokenType::TOKEN_TYPE::expr && curr_type != TokenType::TOKEN_TYPE::rel_expr) {
			return false;
		}
	}

	if (prev_type == TokenType::TOKEN_TYPE::constant) {
		if (curr_type != TokenType::TOKEN_TYPE::expr && curr_type != TokenType::TOKEN_TYPE::rel_expr && curr_val != TYPE_PUNC_CLOSED_PARAN) {
			return false;
		}
	}

	if (prev_type == TokenType::TOKEN_TYPE::var) {
		if (curr_type != TokenType::TOKEN_TYPE::expr && curr_type != TokenType::TOKEN_TYPE::rel_expr && curr_val != TYPE_PUNC_CLOSED_PARAN) {
			return false;
		}
	}

	return true;
}