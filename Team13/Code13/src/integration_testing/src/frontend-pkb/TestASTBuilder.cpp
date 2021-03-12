#include <Tokenizer.h>
#include <testUtils/Statement.h>
#include <catch.hpp>
#include <TNode.h>
#include <queue>
#include <ASTBuilder.h>
#include "TestCases.h"

using namespace std;

ASTBuilder astbuilder = ASTBuilder();
vector<TNode*> InOrderTraversal(TNode* root) {

	std::vector<TNode*> inOrder;
	std::queue<TNode*> temp;

	inOrder.push_back(root);
	temp.push(root);

	if (root == NULL) {
		return inOrder;
	}

	while (!temp.empty()) {
		TNode* curr = temp.front();
		temp.pop();

		for (TNode* child : curr->GetChildrenVector()) {
			inOrder.push_back(child);
			temp.push(child);
		}
	}
	return inOrder;
}

bool SameTNode(TNode* n1, TNode* n2) {
	return n1->getData() == n2->getData();
}

bool SameAST(vector<TNode*> v1, vector<TNode*> v2) {
	if (v1.size() != v2.size()) {
		return false;
	}
	for (int i = 0; i < v1.size(); i++) {
		if (!SameTNode(v1.at(i), v2.at(i))) {
			return false;
		}
	}
	return true;
}

TEST_CASE("Parsing assignment statement for variable to simple expression") {
	Tokenizer tokenizer(*(AssgnTest1.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest1.root)));
}

TEST_CASE("Parsing assignment statement for variable to constant") {
	Tokenizer tokenizer(*(AssgnTest2.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest2.root)));
}

TEST_CASE("Parsing assignment statement for variable to variable") {
	Tokenizer tokenizer(*(AssgnTest3.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest3.root)));
}

TEST_CASE("Parsing assignment statement for variable to more complex experssion") {
	Tokenizer tokenizer(*(AssgnTest4.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest4.root)));
}

TEST_CASE("Parsing regular if/else program") {
	Tokenizer tokenizer(*(IfTestCase1.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(IfTestCase1.root)));
}

TEST_CASE("Parsing regular if statement with complex expression program") {
	Tokenizer tokenizer(*(IfTestCase5.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(IfTestCase5.root)));
}

TEST_CASE("Parsing regular while statement") {
	Tokenizer tokenizer(*(WhileTestCase1.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());
	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(WhileTestCase1.root)));
}


TEST_CASE("Parsing regular while statement with complex expression program") {
	Tokenizer tokenizer(*(WhileTestCase4.SourceProgram));
	tokenizer.Tokenize();
	cout << "Expected Tree:" << endl;
	WhileTestCase4.root->Print(WhileTestCase4.root);
	cout << endl << endl << "Actual Tree:" << endl;
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(WhileTestCase4.root)));
}

TEST_CASE("Parsing regular read statement") {
	Tokenizer tokenizer(*(ValidReadCase1.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(ValidReadCase1.root)));
}


TEST_CASE("Parsing regular print statement") {
	Tokenizer tokenizer(*(ValidPrintCase1.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(ValidPrintCase1.root)));
}

TEST_CASE("Parsing call read statement") {
	Tokenizer tokenizer(*(ValidReadCase1.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = astbuilder.BuildMainPrgNode(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(ValidReadCase1.root)));
}
