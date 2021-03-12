#include "catch.hpp"
using namespace std;

#include "pkb/PKB.h"
#include "pql/PQLEvaluator.h"

PKB pkb = PKB();
PQLEvaluator pql = PQLEvaluator();

TEST_CASE("No clauses: GetAllVariables") {
	REQUIRE(pkb.GetDataManager().GetAllVariables() == pql.ConvertSet(pql.EvaluateAllCall(TYPE_VAR)));
}

TEST_CASE("No clauses: Procedures") {
	REQUIRE(pkb.GetDataManager().GetAllProcedures() == pql.ConvertSet(pql.EvaluateAllCall(TYPE_PROC)));
}

TEST_CASE("No clauses: Constants") {
	REQUIRE(pql.ConvertSet(pkb.GetDataManager().GetAllConstants()) == pql.EvaluateAllCall(TYPE_CONST));
}

TEST_CASE("No clauses: Statements") {
	REQUIRE(pql.ConvertSet(pkb.GetDataManager().GetAllStatements()) == pql.EvaluateAllCall(TYPE_STMT));
}

TEST_CASE("No clauses: Statements - Assign") {
	REQUIRE(pql.ConvertSet(pkb.GetDataManager().GetAllStatements()) == pql.EvaluateAllCall(TYPE_STMT_ASSIGN));
}

TEST_CASE("No clauses: Statements - Call") {
	REQUIRE(pql.ConvertSet(pkb.GetDataManager().GetAllStatements()) == pql.EvaluateAllCall(TYPE_STMT_CALL));
}

TEST_CASE("No clauses: Statements - If") {
	REQUIRE(pql.ConvertSet(pkb.GetDataManager().GetAllStatements()) == pql.EvaluateAllCall(TYPE_STMT_IF));
}

TEST_CASE("No clauses: Statements - Print") {
	REQUIRE(pql.ConvertSet(pkb.GetDataManager().GetAllStatements()) == pql.EvaluateAllCall(TYPE_STMT_PRINT));
}

TEST_CASE("No clauses: Statements - Read") {
	REQUIRE(pql.ConvertSet(pkb.GetDataManager().GetAllStatements()) == pql.EvaluateAllCall(TYPE_STMT_READ));
}

TEST_CASE("No clauses: Statements - While") {
	REQUIRE(pql.ConvertSet(pkb.GetDataManager().GetAllStatements()) == pql.EvaluateAllCall(TYPE_STMT_WHILE));
}

