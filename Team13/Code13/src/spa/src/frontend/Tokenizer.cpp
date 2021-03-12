#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

#include "frontend/Token.h"
#include "frontend/TokenType.h"
#include "Tokenizer.h"

using namespace std;

Tokenizer::Tokenizer(string input) {
	
	pos_ = -1;
	text_ = input;
	len_ = text_.length();
	token_str_ = "";

	Tokenize();

}

void Tokenizer::Tokenize() {

	Token prev_token;
	Token* curr_token_ptr;

	while (pos_ < len_ - 1) {
		pos_ += 1;

		char c = text_.at(pos_);

		//detect whitespace
		if (IsWhiteSpace(c)) {
			continue;
			//detect punctuation
		}
		else if (IsPunc(c)) {
			AppendCharToTokenStr(c);
			AddToken(TokenType::TOKEN_TYPE::punc);
			ResetTokenStr();
			continue;
			//detect arithmetic operators
		}
		else if (IsExpr(c)) {
			AppendCharToTokenStr(c);
			AddToken(TokenType::TOKEN_TYPE::expr);
			ResetTokenStr();
			continue;
		}
		// if it is > < ! =
		else if (IsRelExprPart(c)) {

			char next_char = text_.at(pos_ + 1);

			string str = "";

			str.push_back(c);
			str.push_back(next_char);

			if (IsRelExpr(str)) {
				AppendStrToTokenStr(str);
				AddToken(TokenType::TOKEN_TYPE::rel_expr);
				ResetTokenStr();
				pos_ += 1;
				continue;
			}
			else {

				string c_str = "";
				c_str.push_back(c);

				if (IsRelExpr(c_str)) {
					AppendStrToTokenStr(c_str);
					AddToken(TokenType::TOKEN_TYPE::rel_expr);
					ResetTokenStr();
					continue;
				}

				if (c == '!') {
					AppendCharToTokenStr(c);
					Token* token_ptr = AddToken(TokenType::TOKEN_TYPE::cond_expr);
					token_ptr->is_unary_op_ = true;
					ResetTokenStr();
					continue;
				}
				else if (c == '=') {
					AppendCharToTokenStr(c);
					AddToken(TokenType::TOKEN_TYPE::assign);
					ResetTokenStr();
					continue;
				}
			}

		}
		else if (IsCondExprPart(c)) {

			char next_char = text_.at(pos_ + 1);

			string str = "";

			str.push_back(c);

			str.push_back(next_char);
			
			if (IsCondExpr(str)) {
				AppendStrToTokenStr(str);
				AddToken(TokenType::TOKEN_TYPE::cond_expr);
				ResetTokenStr();
				pos_ += 1;
				continue;
			}
		}
		else {
			AppendCharToTokenStr(c);
		}


		/**
		We are left with
		1. Integers: DIGIT+
		2. Names: LETTER (LETTER|DIGIT)*
		2a. Keywords: Names that match keywords
		where DIGIT: 0-9 && LETTER: A-Z | a-z
		*/

		/**
		* If we encounter a whitespace on the next char, we evaluate the token
		*/

		if (
			token_str_.length() > 0
			&& (pos_ + 1 >= len_
				|| ((
					IsWhiteSpace(text_.at(pos_ + 1))
					|| IsPunc(text_.at(pos_ + 1))
					|| IsRelExprPart(text_.at(pos_ + 1))
					|| IsCondExprPart(text_.at(pos_ + 1))
					|| IsExpr(text_.at(pos_+1))
					)))) {

			if (IsInteger(token_str_)) {
				AddToken(TokenType::TOKEN_TYPE::constant);
			}
			else {

				if (IsStmtName(token_str_)) {
					Token* stmtToken = AddToken(TokenType::TOKEN_TYPE::stmt);

					stmtToken->stmt_type_ = TokenType::GetStmtType(token_str_);
				}
				else {
					if (IsVar(token_str_)) {
						AddToken(TokenType::TOKEN_TYPE::var);
					}
					else {
						throw logic_error("Invalid token");
					}
				}

			}

			ResetTokenStr();

		}

	}

}

bool Tokenizer::IsStmtName(string stmt_name) {

	int next_pos = pos_ + 1;
	int text_length = text_.length();

	while ( next_pos < text_length && IsWhiteSpace(text_.at(next_pos))) {
		next_pos += 1;
	}

	if (next_pos < text_length && text_.at(next_pos) == '=') {
		return false;
	}

	for (string name : stmt_names_) {
		if (stmt_name == name) {
			return true;
		}
	}
	
	return false;

}

bool Tokenizer::IsInteger(string integer) {

	bool result = false;

	regex expr("^0$|^[1-9][0-9]*$");

	if (regex_match(integer, expr)) {
		result = true;
	}

	return result;
}

bool Tokenizer::IsVar(string var_name) {

	return regex_match(var_name, regex("^[a-zA-Z][a-zA-Z0-9]*$"));

}


bool Tokenizer::IsWhiteSpace(char c) {

	string s = string(1, c);

	return IsMatch(s, white_spaces_);
}

bool Tokenizer::IsPunc(char c) {
	string s = string(1, c);

	return IsMatch(s, punctuations_);
}

bool Tokenizer::IsExpr(char c) {
	string s = string(1, c);

	return IsMatch(s, expressions_);
}

bool Tokenizer::IsRelExprPart(char c) {
	string s = string(1, c);
	return IsMatch(s, rel_expressions_part_);
}

bool Tokenizer::IsRelExpr(string str) {
	return IsMatch(str, rel_expressions_);
}

bool Tokenizer::IsCondExprPart(char c) {
	string s = string(1, c);
	return IsMatch(s, cond_expressions_part_);
}

bool Tokenizer::IsCondExpr(string str) {
	return IsMatch(str, cond_expressions_);
}

bool Tokenizer::IsMatch(string s, STR_LIST v) {

	for (string str : v) {
		if (s == str) {
			return true;
		}
	}

	return false;
}

void Tokenizer::AppendCharToTokenStr(char c) {
	string str = "";
	str += c;

	AppendStrToTokenStr(str);
}

void Tokenizer::AppendStrToTokenStr(string str) {
	token_str_ += str;
}


Token* Tokenizer::AddToken(TokenType::TOKEN_TYPE token_type) {

	Token token_obj = Token(token_str_, token_type);

	token_list_.push_back(token_obj);

	return &token_list_.at(token_list_.size()-1);
}

void Tokenizer::ResetTokenStr() {
	token_str_ = "";
}

vector<Token> Tokenizer::GetTokenList() {
	return token_list_;
}