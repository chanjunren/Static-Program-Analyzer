#include <Tokenizer.h>
#include <testUtils/Statement.h>
#include <catch.hpp>
#include <TNode.h>
#include <queue>
#include <ASTBuilder.h>
#include <CFGBuilder.h>
#include "TestCases.h"

using namespace std;

void print_map(CFG_ADJACENCY_LIST const& m)
{
	for (auto const& pair : m) {
		cout << "{" << pair.first << ": ";
		cout << "[";
		
		for (STMT_IDX node : *(pair.second)) {
			cout << node << ", ";
		}

		cout << "]" << endl;
	}
}


bool SameCFG(CFG* cfg_1, CFG* cfg_2) {

	CFG_ADJACENCY_LIST l_1 = cfg_1->GetAdjacencyList();
	CFG_ADJACENCY_LIST l_2 = cfg_2->GetAdjacencyList();

	//print_map(l_1);
	//print_map(l_2);

	if (l_1.size() != l_2.size()) {
		return false;
	}

	for (auto const& pair: l_1) {
		
		STMT_IDX from = pair.first;

		STMT_IDX_SET* s_1 = pair.second;
		STMT_IDX_SET* s_2 = l_2.at(from);

		if (s_1->size() != s_2->size()) {
			return false;
		}

		for (STMT_IDX node : *s_1) {
			bool found = s_2->find(node) != s_2->end();

			if (!found) {
				return false;
			}
		}

	}

	return true;
}

TEST_CASE("Build Single Stmt CFG") {
	ASTBuilder astbuilder = ASTBuilder();
	Tokenizer tokenizer(*(SingleStmtTest.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());
	
	CFGBuilder cfgbuilder = CFGBuilder(actualRoot);
	
	REQUIRE(SameCFG(SingleStmtTest.cfg, cfgbuilder.cfg_));
}

TEST_CASE("Build Double Stmt CFG") {
	ASTBuilder astbuilder = ASTBuilder();
	Tokenizer tokenizer(*(DoubleStmtTest.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	CFGBuilder cfgbuilder = CFGBuilder(actualRoot);

	REQUIRE(SameCFG(DoubleStmtTest.cfg, cfgbuilder.cfg_));
}

TEST_CASE("If Stmt CFG") {
	ASTBuilder astbuilder = ASTBuilder();
	Tokenizer tokenizer(*(IfStmtTest.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	CFGBuilder cfgbuilder = CFGBuilder(actualRoot);

	REQUIRE(SameCFG(IfStmtTest.cfg, cfgbuilder.cfg_));
}

TEST_CASE("If Stmt CFG with exit") {
	ASTBuilder astbuilder = ASTBuilder();
	Tokenizer tokenizer(*(IfStmtWithExitTest.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	CFGBuilder cfgbuilder = CFGBuilder(actualRoot);

	REQUIRE(SameCFG(IfStmtWithExitTest.cfg, cfgbuilder.cfg_));
}

TEST_CASE("While Stmt CFG") {
	ASTBuilder astbuilder = ASTBuilder();
	Tokenizer tokenizer(*(WhileStmtTest.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	CFGBuilder cfgbuilder = CFGBuilder(actualRoot);

	REQUIRE(SameCFG(WhileStmtTest.cfg, cfgbuilder.cfg_));
}

TEST_CASE("While Stmt CFG with exit") {
	ASTBuilder astbuilder = ASTBuilder();
	Tokenizer tokenizer(*(WhileStmtWithExitTest.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	CFGBuilder cfgbuilder = CFGBuilder(actualRoot);

	REQUIRE(SameCFG(WhileStmtWithExitTest.cfg, cfgbuilder.cfg_));
}

TEST_CASE("Nested If While Stmt CFG") {
	ASTBuilder astbuilder = ASTBuilder();
	Tokenizer tokenizer(*(NestedIfWhileStmtTest.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	CFGBuilder cfgbuilder = CFGBuilder(actualRoot);

	REQUIRE(SameCFG(NestedIfWhileStmtTest.cfg, cfgbuilder.cfg_));
}

TEST_CASE("Nested While If Stmt CFG") {
	ASTBuilder astbuilder = ASTBuilder();
	Tokenizer tokenizer(*(NestedWhileIfStmtTest.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	CFGBuilder cfgbuilder = CFGBuilder(actualRoot);

	REQUIRE(SameCFG(NestedWhileIfStmtTest.cfg, cfgbuilder.cfg_));
}

TEST_CASE("Multiple Nested If While Stmt CFG") {
	ASTBuilder astbuilder = ASTBuilder();
	Tokenizer tokenizer(*(MultipleNestedIfWhileTest.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	CFGBuilder cfgbuilder = CFGBuilder(actualRoot);

	REQUIRE(SameCFG(MultipleNestedIfWhileTest.cfg, cfgbuilder.cfg_));
}

TEST_CASE("Multiple Proc CFG") {
	ASTBuilder astbuilder = ASTBuilder();
	Tokenizer tokenizer(*(MultipleProcTest.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	CFGBuilder cfgbuilder = CFGBuilder(actualRoot);

	REQUIRE(SameCFG(MultipleProcTest.cfg, cfgbuilder.cfg_));
}