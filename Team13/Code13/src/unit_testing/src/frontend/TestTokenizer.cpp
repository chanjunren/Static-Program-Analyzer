
#include <Tokenizer.h>
#include <frontend/Token.h>
#include <frontend/TokenType.h>

#include "catch.hpp"
#include <vector>

TEST_CASE("Tokenizer | Manipulates token string correctly") {

	Tokenizer tokenizer("");

	tokenizer.token_str_ = "procedure";

	SECTION("Resets token string") {
		tokenizer.ResetTokenStr();
		REQUIRE(tokenizer.token_str_.length() == 0);
	}

	SECTION("Adds character to token string") {
		tokenizer.ResetTokenStr();
		tokenizer.AppendCharToTokenStr('x');
		REQUIRE(tokenizer.token_str_ == "x");
	}

	SECTION("Adds string to token string") {
		tokenizer.ResetTokenStr();
		tokenizer.AppendStrToTokenStr("x = a + b");
		REQUIRE(tokenizer.token_str_ == "x = a + b");
	}
}

TEST_CASE("Tokenizer | Manipulates tokens correctly") {

	SECTION("Tokenizes string with one keyword token correctly") {
		Tokenizer tokenizer("procedure");
		tokenizer.Tokenize();
		Token token = tokenizer.GetTokenList().at(0);

		REQUIRE(token.GetValue() == "procedure");
		REQUIRE(TokenType::GetStmtType(token.GetValue()) == TokenType::STMT_TYPE::_procedure);
	}

	SECTION("Tokenizes string with all white spaces correctly") {
		Tokenizer tokenizer("\n \t \r");
		tokenizer.Tokenize();
		REQUIRE(tokenizer.GetTokenList().size() == 0);
	}

	SECTION("Tokenizes arithmetic expression string correctly") {
		Tokenizer tokenizer("x = a + 5");
		tokenizer.Tokenize();
		std::vector<Token> tokenList = tokenizer.GetTokenList();

		int i = 0;
		REQUIRE(tokenList.at(i).GetValue() == "x");
		REQUIRE(tokenList.at(i).GetTokenType() == TokenType::TOKEN_TYPE::var);
		i++;
		REQUIRE(tokenList.at(i).GetValue() == "=");
		REQUIRE(tokenList.at(i).GetTokenType() == TokenType::TOKEN_TYPE::assign);
		i++;
		REQUIRE(tokenList.at(i).GetValue() == "a");
		REQUIRE(tokenList.at(i).GetTokenType() == TokenType::TOKEN_TYPE::var);
		i++;
		REQUIRE(tokenList.at(i).GetValue() == "+");
		REQUIRE(tokenList.at(i).GetTokenType() == TokenType::TOKEN_TYPE::expr);
		i++;
		REQUIRE(tokenList.at(i).GetValue() == "5");
		REQUIRE(tokenList.at(i).GetTokenType() == TokenType::TOKEN_TYPE::constant);

	}

	SECTION("Tokenizes while statement with one condition correctly") {

		Tokenizer tokenizer("while ( 1 ) {");
		tokenizer.Tokenize();
		std::vector<Token> tokenList = tokenizer.GetTokenList();

		int i = 0;
		REQUIRE(tokenList.at(i).GetValue() == "while");
		REQUIRE(tokenList.at(i).GetTokenType() == TokenType::TOKEN_TYPE::stmt);
		REQUIRE(tokenList.at(i).GetStmtType() == TokenType::STMT_TYPE::_while);
		i++;
		REQUIRE(tokenList.at(i).GetValue() == "(");
		REQUIRE(tokenList.at(i).GetTokenType() == TokenType::TOKEN_TYPE::punc);
		i++;
		REQUIRE(tokenList.at(i).GetValue() == "1");
		REQUIRE(tokenList.at(i).GetTokenType() == TokenType::TOKEN_TYPE::constant);
		i++;
		REQUIRE(tokenList.at(i).GetValue() == ")");
		REQUIRE(tokenList.at(i).GetTokenType() == TokenType::TOKEN_TYPE::punc);
		i++;
		REQUIRE(tokenList.at(i).GetValue() == "{");
		REQUIRE(tokenList.at(i).GetTokenType() == TokenType::TOKEN_TYPE::punc);


	}

	SECTION("Tokenizes print statement correctly") {
		Tokenizer tokenizer("print x");
		tokenizer.Tokenize();
		std::vector<Token> tokenList = tokenizer.GetTokenList();

		REQUIRE(tokenList.at(0).GetValue() == "print");
		REQUIRE(tokenList.at(0).GetTokenType() == TokenType::TOKEN_TYPE::stmt);
		REQUIRE(tokenList.at(0).GetStmtType() == TokenType::STMT_TYPE::_print);

		REQUIRE(tokenList.at(1).GetValue() == "x");
		REQUIRE(tokenList.at(1).GetTokenType() == TokenType::TOKEN_TYPE::var);
	}

}
