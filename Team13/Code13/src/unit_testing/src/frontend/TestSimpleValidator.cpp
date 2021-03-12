#include <SimpleValidator.h>
#include "ValidatorTestCases.h"
#include <Tokenizer.h>
#include <catch.hpp>

using namespace std;

TEST_CASE("Testing if SimpleValidator catches cyclic call A->B->C->A") {
	Tokenizer tokenizer(*(CyclicTestCase1.SourceProgram));
	SimpleValidator simple_validator;
	CHECK_THROWS(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Testing if SimpleValidator catches cyclc call A->A") {
	Tokenizer tokenizer(*(CyclicTestCase2.SourceProgram));
	SimpleValidator simple_validator;
	CHECK_THROWS(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Testing if SimpleValidator catches cyclic call for more complex graph") {
	Tokenizer tokenizer(*(CyclicTestCase3.SourceProgram));
	SimpleValidator simple_validator;
	CHECK_THROWS(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing assignment statement for variable to simple expression") {
	Tokenizer tokenizer(*(AssgnTest1.SourceProgram));
	SimpleValidator simple_validator;
	REQUIRE(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing assignment statement for variable to constant") {
	Tokenizer tokenizer(*(AssgnTest2.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	REQUIRE(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing assignment statement for variable to variable") {
	Tokenizer tokenizer(*(AssgnTest3.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	REQUIRE(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing assignment statement for variable to more complex experssion") {
	Tokenizer tokenizer(*(AssgnTest4.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	REQUIRE(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing regular if/else program") {
	Tokenizer tokenizer(*(IfTestCase1.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	REQUIRE(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing If statement without valid expression") {
	Tokenizer tokenizer(*(IfTestCase2.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	CHECK_THROWS(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing if statement without else block") {
	Tokenizer tokenizer(*(InvalidIfTestCase1.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	CHECK_THROWS(simple_validator.IsValid(tokenizer.GetTokenList()));
}

/*TEST_CASE("Parsing if statement with invalid expression") {
	try {
		Tokenizer tokenizer(*(InvalidIfTestCase2.SourceProgram));
		tokenizer.Tokenize();
		SimpleValidator simple_validator;
		(simple_validator.IsValid(tokenizer.GetTokenList()));
	}
	catch (exception e) {
		REQUIRE(true);
	}
}*/

TEST_CASE("Parsing regular if statement with complex expression program") {
	Tokenizer tokenizer(*(IfTestCase3.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	REQUIRE(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing regular while statement") {
	Tokenizer tokenizer(*(WhileTestCase1.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	REQUIRE(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing while statement with invalid expression") {
	Tokenizer tokenizer(*(InvalidWhileTestCase1.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	CHECK_THROWS(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing invalid while statement with extra tokens") {
	Tokenizer tokenizer(*(InvalidWhileTestCase2.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	CHECK_THROWS(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing regular while statement with complex expression program") {
	Tokenizer tokenizer(*(WhileTestCase2.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	REQUIRE(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing program with extra ; token") {
	Tokenizer tokenizer(*(ExtraTokensCase1.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	CHECK_THROWS(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing program with extra } token") {
	Tokenizer tokenizer(*(ExtraTokensCase1.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	CHECK_THROWS(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing program with extra token in definition of procedure") {
	Tokenizer tokenizer(*(ExtraTokensCase1.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	CHECK_THROWS(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing regular read statement") {
	Tokenizer tokenizer(*(ValidReadCase1.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	REQUIRE(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing read statement with wrong spelling") {
	Tokenizer tokenizer(*(InvalidReadCase1.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	CHECK_THROWS(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing regular print statement") {
	Tokenizer tokenizer(*(ValidPrintCase1.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	REQUIRE(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing print statement with wrong spelling") {
	Tokenizer tokenizer(*(InvalidPrintCase1.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	CHECK_THROWS(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing call read statement") {
	Tokenizer tokenizer(*(ValidReadCase1.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	REQUIRE(simple_validator.IsValid(tokenizer.GetTokenList()));
}

TEST_CASE("Parsing call statement with wrong spelling") {
	Tokenizer tokenizer(*(InvalidReadCase1.SourceProgram));
	tokenizer.Tokenize();
	SimpleValidator simple_validator;
	CHECK_THROWS(simple_validator.IsValid(tokenizer.GetTokenList()));
}
