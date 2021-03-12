#include <TNode.h>
#include <queue>

#include "TestCases.h"

using namespace std;

TNode* buildAssgnTestCase1() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "AssgnTest1");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);
	
	TNode* stmtNode = new TNode(TNode::assignStmt, 1);
	TNode* varNodeName = new TNode(TNode::varName, "x");
	TNode* constNode1 = new TNode(TNode::constValue, (double) 5);
	TNode* plusNode = new TNode(TNode::expr, TNode::OPERATOR::plus);
	TNode* constNode2 = new TNode(TNode::varName, "y");

	plusNode->AddChild(constNode2);
	plusNode->AddChild(constNode1);

	stmtNode->AddChild(varNodeName);
	stmtNode->AddChild(plusNode);
	
	stmtListNode->AddChild(stmtNode);

	return mainPrg;
}

extern TestCase AssgnTest1 = {
	new string("procedure AssgnTest1 {"
	"x = y + 5;}")	
	, buildAssgnTestCase1()};

TNode* buildAssgnTestCase2() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "AssgnTest2");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);

	TNode* stmtNode = new TNode(TNode::assignStmt, 1);
	TNode* varNodeName = new TNode(TNode::varName, "x");
	TNode* constNode1 = new TNode(TNode::constValue, (double) 7);

	stmtNode->AddChild(varNodeName);
	stmtNode->AddChild(constNode1);

	stmtListNode->AddChild(stmtNode);

	return mainPrg;
}

extern TestCase AssgnTest2 = {
	new string("procedure AssgnTest2 {"
	"x = 7;}")
	, buildAssgnTestCase2() };

TNode* buildAssgnTestCase3() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "AssgnTest3");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);

	TNode* stmtNode = new TNode(TNode::assignStmt, 1);
	TNode* varNode1 = new TNode(TNode::varName, "x");
	TNode* varNode2 = new TNode(TNode::varName, "y");

	stmtNode->AddChild(varNode1);
	stmtNode->AddChild(varNode2);

	stmtListNode->AddChild(stmtNode);

	return mainPrg;
}

extern TestCase AssgnTest3 = {
	new string("procedure AssgnTest3 {"
	"x = y;}")
	, buildAssgnTestCase3() };

TNode* buildAssgnTestCase4() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "AssgnTest4");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);

	TNode* stmtNode = new TNode(TNode::assignStmt, 1);

	// x = y + 5 * (a + b) - c - d * d;}
	TNode* resNode = new TNode(TNode::varName, "x");
	TNode* constNode = new TNode(TNode::constValue, (double) 5);
	TNode* aNode = new TNode(TNode::varName, "a");
	TNode* bNode = new TNode(TNode::varName, "b");
	TNode* yNode = new TNode(TNode::varName, "y");

	TNode* abPlusNode = new TNode(TNode::expr, TNode::OPERATOR::plus);
	abPlusNode->AddChild(aNode);
	abPlusNode->AddChild(bNode);

	TNode* ab5multNode = new TNode(TNode::expr, TNode::OPERATOR::times);
	ab5multNode->AddChild(constNode);
	ab5multNode->AddChild(abPlusNode);

	TNode* yPlusExprNode = new TNode(TNode::expr, TNode::OPERATOR::plus);
	yPlusExprNode->AddChild(yNode);
	yPlusExprNode->AddChild(ab5multNode);

	TNode* cNode = new TNode(TNode::varName, "c");
	TNode* cMinusNode = new TNode(TNode::expr, TNode::OPERATOR::minus);
	cMinusNode->AddChild(yPlusExprNode);
	cMinusNode->AddChild(cNode);

	TNode* dNode1 = new TNode(TNode::varName, "d");
	TNode* dNode2 = new TNode(TNode::varName, "d");
	TNode* ddMultNode = new TNode(TNode::expr, TNode::OPERATOR::times);
	ddMultNode->AddChild(dNode1);
	ddMultNode->AddChild(dNode2);

	TNode* rootMinusNode = new TNode(TNode::expr, TNode::OPERATOR::minus);
	rootMinusNode->AddChild(cMinusNode);
	rootMinusNode->AddChild(ddMultNode);

	stmtNode->AddChild(resNode);
	stmtNode->AddChild(rootMinusNode);

	stmtListNode->AddChild(stmtNode);

	return mainPrg;
}

extern TestCase AssgnTest4 = {
	new string("procedure AssgnTest4 {"
	"x = y + 5 * (a + b) - c - d * d;}")
	, buildAssgnTestCase4() };

TNode* buildIfTestCase1() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "IfTestCase1");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);

	TNode* stmtNode = new TNode(TNode::ifStmt, 1);
	
	stmtListNode->AddChild(stmtNode);

	TNode* ltNode = new TNode(TNode::relExpr, TNode::OPERATOR::lesser);
	TNode* xNode = new TNode(TNode::varName, "x");
	TNode* constNode = new TNode(TNode::constValue, *(new double(7)));
	ltNode->AddChild(xNode);
	ltNode->AddChild(constNode);

	TNode* ifStmtListNode = new TNode(TNode::stmtList);
	TNode* printNode = new TNode(TNode::printStmt, 2);
	TNode* helloNode = new TNode(TNode::varName, *(new string("hello")));
	printNode->AddChild(helloNode);
	ifStmtListNode->AddChild(printNode);
	
	TNode* elseStmtListNode = new TNode(TNode::stmtList);
	TNode* callNode = new TNode(TNode::callStmt, *(new int(3)));
	TNode* callProcNode = new TNode(TNode::procName, *(new string("something")));
	callNode->AddChild(callProcNode);
	elseStmtListNode->AddChild(callNode);

	stmtNode->AddChild(ltNode);
	stmtNode->AddChild(ifStmtListNode);
	stmtNode->AddChild(elseStmtListNode);
	return mainPrg;
}

extern TestCase IfTestCase1 = {
	new string("procedure IfTestCase1 {"
	"if (x < 7) then {"
	"print hello;"
	"} else {"
	"call something;"
	"}}"), buildIfTestCase1()};

extern TestCase IfTestCase2 = {
	new string("procedure IfTestCase2 {"
	"if (true) then {"
	"print hello;"
	"} else {"
	"call something;"
	"}}"), buildIfTestCase1() };

extern TestCase IfTestCase3 = {
	new string("procedure IfTestCase3 {"
	"if (x < 7) then {"
	"print hello;"
	"}}"), NULL };

extern TestCase IfTestCase4 = {
	new string("procedure IfTestCase4 {"
	"if (x < 7) {"
	"print hello;"
	"}}"), NULL };

TNode* buildIfTestCase5() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "IfTestCase5");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);

	TNode* stmtNode = new TNode(TNode::ifStmt, 1);

	stmtListNode->AddChild(stmtNode);

	TNode* ltNode = new TNode(TNode::relExpr, TNode::OPERATOR::lesser);
	TNode* xNode = new TNode(TNode::varName, "x");
	TNode* constNode = new TNode(TNode::constValue, (double)5);
	TNode* aNode = new TNode(TNode::varName, "a");
	TNode* bNode = new TNode(TNode::varName, "b");
	TNode* yNode = new TNode(TNode::varName, "y");

	TNode* abPlusNode = new TNode(TNode::expr, TNode::OPERATOR::plus);
	abPlusNode->AddChild(aNode);
	abPlusNode->AddChild(bNode);

	TNode* ab5multNode = new TNode(TNode::expr, TNode::OPERATOR::times);
	ab5multNode->AddChild(constNode);
	ab5multNode->AddChild(abPlusNode);

	TNode* yPlusExprNode = new TNode(TNode::expr, TNode::OPERATOR::plus);
	yPlusExprNode->AddChild(yNode);
	yPlusExprNode->AddChild(ab5multNode);

	TNode* cNode = new TNode(TNode::varName, "c");
	TNode* cMinusNode = new TNode(TNode::expr, TNode::OPERATOR::minus);
	cMinusNode->AddChild(yPlusExprNode);
	cMinusNode->AddChild(cNode);

	TNode* dNode1 = new TNode(TNode::varName, "d");
	TNode* dNode2 = new TNode(TNode::varName, "d");
	TNode* ddMultNode = new TNode(TNode::expr, TNode::OPERATOR::times);
	ddMultNode->AddChild(dNode1);
	ddMultNode->AddChild(dNode2);

	TNode* rootMinusNode = new TNode(TNode::expr, TNode::OPERATOR::minus);
	rootMinusNode->AddChild(cMinusNode);
	rootMinusNode->AddChild(ddMultNode);

	ltNode->AddChild(xNode);
	ltNode->AddChild(rootMinusNode);

	TNode* ifStmtListNode = new TNode(TNode::stmtList);
	TNode* printNode = new TNode(TNode::printStmt, 2);
	TNode* helloNode = new TNode(TNode::varName, *(new string("hello")));
	printNode->AddChild(helloNode);
	ifStmtListNode->AddChild(printNode);

	TNode* elseStmtListNode = new TNode(TNode::stmtList);
	TNode* callNode = new TNode(TNode::callStmt, *(new int(3)));
	TNode* callProcNode = new TNode(TNode::procName, *(new string("something")));
	callNode->AddChild(callProcNode);
	elseStmtListNode->AddChild(callNode);

	stmtNode->AddChild(ltNode);
	stmtNode->AddChild(ifStmtListNode);
	stmtNode->AddChild(elseStmtListNode);
	return mainPrg;
}

extern TestCase IfTestCase5 = {
	new string("procedure IfTestCase5 {"
	"if (x < y + 5 * (a + b) - c - d * d) then {"
	"print hello;"
	"} else {"
	"call something;"
	"}}"), buildIfTestCase5()
};

TNode* buildWhileTestCase1() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "WhileTestCase1");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);

	TNode* stmtNode = new TNode(TNode::whileStmt, 1);

	stmtListNode->AddChild(stmtNode);

	TNode* ltNode = new TNode(TNode::relExpr, TNode::OPERATOR::lesser);
	TNode* xNode = new TNode(TNode::varName, "x");
	TNode* constNode = new TNode(TNode::constValue, *(new double(7)));
	ltNode->AddChild(xNode);
	ltNode->AddChild(constNode);

	TNode* whileStmtListNode = new TNode(TNode::stmtList);
	TNode* printNode = new TNode(TNode::printStmt, 2);
	TNode* helloNode = new TNode(TNode::varName, *(new string("hello")));
	printNode->AddChild(helloNode);
	whileStmtListNode->AddChild(printNode);

	stmtNode->AddChild(ltNode);
	stmtNode->AddChild(whileStmtListNode);
	return mainPrg;
}

extern TestCase WhileTestCase1 = {
	new string("procedure WhileTestCase1 {"
	"while (x < 7) {"
	"print hello;"
	"}}"), buildWhileTestCase1()};

extern TestCase WhileTestCase2 = {
	new string("procedure WhileTestCase2 {"
	"while (true) {"
	"print hello;"
	"}}"), NULL};

extern TestCase WhileTestCase3 = {
	new string("procedure WhileTestCase3 {"
	"while (x < 7) then {"
	"print hello;"
	"}}"), NULL};


TNode* buildWhileTestCase4() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "WhileTestCase4");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);

	TNode* stmtNode = new TNode(TNode::whileStmt, 1);

	stmtListNode->AddChild(stmtNode);

	TNode* ltNode = new TNode(TNode::relExpr, TNode::OPERATOR::lesser);
	TNode* xNode = new TNode(TNode::varName, "x");
	TNode* constNode = new TNode(TNode::constValue, (double)5);
	TNode* aNode = new TNode(TNode::varName, "a");
	TNode* bNode = new TNode(TNode::varName, "b");
	TNode* yNode = new TNode(TNode::varName, "y");

	TNode* abPlusNode = new TNode(TNode::expr, TNode::OPERATOR::plus);
	abPlusNode->AddChild(aNode);
	abPlusNode->AddChild(bNode);

	TNode* ab5multNode = new TNode(TNode::expr, TNode::OPERATOR::times);
	ab5multNode->AddChild(constNode);
	ab5multNode->AddChild(abPlusNode);

	TNode* yPlusExprNode = new TNode(TNode::expr, TNode::OPERATOR::plus);
	yPlusExprNode->AddChild(yNode);
	yPlusExprNode->AddChild(ab5multNode);

	TNode* cNode = new TNode(TNode::varName, "c");
	TNode* cMinusNode = new TNode(TNode::expr, TNode::OPERATOR::minus);
	cMinusNode->AddChild(yPlusExprNode);
	cMinusNode->AddChild(cNode);

	TNode* dNode1 = new TNode(TNode::varName, "d");
	TNode* dNode2 = new TNode(TNode::varName, "d");
	TNode* ddMultNode = new TNode(TNode::expr, TNode::OPERATOR::times);
	ddMultNode->AddChild(dNode1);
	ddMultNode->AddChild(dNode2);

	TNode* rootMinusNode = new TNode(TNode::expr, TNode::OPERATOR::minus);
	rootMinusNode->AddChild(cMinusNode);
	rootMinusNode->AddChild(ddMultNode);

	ltNode->AddChild(xNode);
	ltNode->AddChild(rootMinusNode);

	TNode* whileStmtListNode = new TNode(TNode::stmtList);
	TNode* printNode = new TNode(TNode::printStmt, 2);
	TNode* helloNode = new TNode(TNode::varName, *(new string("hello")));
	printNode->AddChild(helloNode);
	whileStmtListNode->AddChild(printNode);

	stmtNode->AddChild(ltNode);
	stmtNode->AddChild(whileStmtListNode);
	return mainPrg;
}

extern TestCase WhileTestCase4{
	new string("procedure WhileTestCase4 {"
	"while (x < y + 5 * (a + b) - c - d * d) {"
	"print hello;"
	"}}"), buildWhileTestCase4()
};

extern TestCase ExtraTokensCase1 = {
	new string("procedure ExtraTokensCase1 {"
	"print hello;;"
	"}"), NULL };

extern TestCase ExtraTokensCase2 = {
	new string("procedure ExtraTokensCase1 {"
	"print hello;"
	"}}"), NULL };

extern TestCase ExtraTokensCase3 = {
	new string("procedure hihi ExtraTokensCase3 {"
	"print hello;"
	"}"), NULL };

TNode* buildValidReadCase1() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "ValidReadCase1");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);

	TNode* stmtNode = new TNode(TNode::readStmt, 1);
	TNode* varNode = new TNode(TNode::varName, *(new string("HarukiMurakami")));

	stmtNode->AddChild(varNode);

	stmtListNode->AddChild(stmtNode);

	return mainPrg;
}

extern TestCase ValidReadCase1 = {
	new string("procedure ValidReadCase1 {"
	"read HarukiMurakami;}"), buildValidReadCase1()
};

extern TestCase InvalidReadCase1 = {
	new string("procedure ValidReadCase1 {"
	"red HarukiMurakami;}"), NULL
};

TNode* buildValidPrintCase1() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "ValidPrintCase1");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);

	TNode* stmtNode = new TNode(TNode::printStmt, 1);
	TNode* varNode = new TNode(TNode::varName, *(new string("HarukiMurakami")));

	stmtNode->AddChild(varNode);

	stmtListNode->AddChild(stmtNode);

	return mainPrg;
}

extern TestCase ValidPrintCase1 = {
	new string("procedure ValidPrintCase1 {"
	"print HarukiMurakami;}"), buildValidPrintCase1()
};

extern TestCase InvalidPrintCase1 = {
	new string("procedure ValidReadCase1 {"
	"prnt HarukiMurakami;}"), NULL
};

TNode* buildValidCallCase1() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "ValidCallCase1");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);

	TNode* stmtNode = new TNode(TNode::callStmt, 1);
	TNode* callProcNameNode = new TNode(TNode::procName, *(new string("HarukiMurakami")));

	stmtNode->AddChild(callProcNameNode);

	stmtListNode->AddChild(stmtNode);

	return mainPrg;
}

extern TestCase ValidCallCase1 = {
	new string("procedure ValidReadCase1 {"
	"call HarukiMurakami;}"), buildValidCallCase1()
};

extern TestCase InvalidCallCase1 = {
	new string("procedure InvalidCallCase1 {"
	"cal HarukiMurakami;}"), NULL
};

