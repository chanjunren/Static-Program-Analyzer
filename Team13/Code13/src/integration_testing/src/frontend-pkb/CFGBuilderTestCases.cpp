#include <TNode.h>
#include <queue>
#include <CFG.h>
#include <vector>

#include "TestCases.h"

using namespace std;


CFG* buildSingleStmtTestCase() {
	CFG* cfg = new CFG();
	return cfg;
}

extern CFGTestCase SingleStmtTest = {
	new string("procedure SingleStmtTest {x = x + 1;}"),
	buildSingleStmtTestCase() };

CFG* buildDoubleStmtTestCase() {
	CFG* cfg = new CFG();
	cfg->AddEdge(1, 2);
	return cfg;
}

extern CFGTestCase DoubleStmtTest = {
	new string("procedure DoubleStmtTest {x = x + 1; x = x + 2;}"),
	buildDoubleStmtTestCase() };

CFG* buildIfStmtTestCase() {
	CFG* cfg = new CFG();

	cfg->AddEdge(1, 2);
	cfg->AddEdge(1, 3);
	return cfg;
}

extern CFGTestCase IfStmtTest = {
	new string("procedure IfStmt {"
	"if( x == 1) then {"
	"y = 1;"
	"}"
	"else {"
	"y = 2;}}"
	),
	buildIfStmtTestCase() };

CFG* buildIfStmtWithExitTestCase() {

	CFG* cfg = new CFG();

	cfg->AddEdge(1, 2);
	cfg->AddEdge(1, 3);
	cfg->AddEdge(2, 4);
	cfg->AddEdge(3, 4);

	return cfg;
}

extern CFGTestCase IfStmtWithExitTest = {
	new string("procedure IfStmt {"
	"if( x == 1) then {"
	"y = 1;"
	"}"
	"else {"
	"y = 2;}"
	"y = 4;"
	"}"
	),
	buildIfStmtWithExitTestCase() };

CFG* buildWhileStmtTestCase() {
	CFG* cfg = new CFG();

	cfg->AddEdge(1, 2);
	cfg->AddEdge(2, 1);

	return cfg;
}

extern CFGTestCase WhileStmtTest = {
	new string("procedure WhileStmt {"
	"while( x < 5 ){"
	"x = 1;"
	"}}"
	),
	buildWhileStmtTestCase() }; 

CFG* buildWhileStmtWithExitTest() {
	CFG* cfg = new CFG();

	cfg->AddEdge(1, 2);
	cfg->AddEdge(2, 1);
	cfg->AddEdge(1, 3);

	return cfg;
}

extern CFGTestCase WhileStmtWithExitTest = {
	new string("procedure WhileStmt {"
	"while( x < 5 ){"
	"x = 1;"
	"}"
	"x = 2;}"
	),
	buildWhileStmtWithExitTest() };

CFG* buildNestedIfWhileStmtTest() {

	CFG* cfg = new CFG();

	cfg->AddEdge(1, 2);
	cfg->AddEdge(1, 4);
	cfg->AddEdge(2, 3);
	cfg->AddEdge(3, 2);

	return cfg;
}

extern CFGTestCase NestedIfWhileStmtTest = {
	new string("procedure Example {"
	"if( x < 5 ) then {"
	"while( x < 5 ){"
	"x = 2;"
	"}"
	"} else {"
	"x = 2;"
	"}}"
	),
	buildNestedIfWhileStmtTest() };

CFG* buildNestedWhileIfStmtTest() {

	CFG* cfg = new CFG();

	cfg->AddEdge(1, 2);
	cfg->AddEdge(2, 3);
	cfg->AddEdge(2, 4);
	cfg->AddEdge(3, 1);
	cfg->AddEdge(4, 1);

	return cfg;
}

extern CFGTestCase NestedWhileIfStmtTest = {
	new string("procedure Example {"
	"while ( x < 5 ) {"
	"if ( x < 3 ) then {"
	"x = 2;"
	"} else {"
	"x=4;"
	"}}}"
	),
	buildNestedWhileIfStmtTest() };

CFG* buildMultipleNestedIfTest() {

	CFG* cfg = new CFG();

	cfg->AddEdge(1, 2);
	cfg->AddEdge(1, 7);
	cfg->AddEdge(2, 3);
	cfg->AddEdge(2, 6);
	cfg->AddEdge(3, 4);
	cfg->AddEdge(3, 5);

	return cfg;
}

extern CFGTestCase MultipleNestedIfTest = {
	new string("procedure Example {"
	"if( x == 1) then {"
	"if( x == 2) then {"
	"if (x == 3) then {"
	"x = 4;"
	"} else {"
	"x = 5;"
	"}} else {"
	"x = 6;"
	"}} else {"
	"x = 7;"
	"}}"
	),
	buildMultipleNestedIfTest() };

CFG* buildMultipleNestedWhileTest() {
	CFG* cfg = new CFG();

	cfg->AddEdge(1, 2);
	cfg->AddEdge(2, 1);
	cfg->AddEdge(2, 3);
	cfg->AddEdge(3, 2);
	cfg->AddEdge(3, 4);
	cfg->AddEdge(4, 3);

	return cfg;
}

extern CFGTestCase MultipleNestedWhileTest = {
	new string("procedure Example {"
	"while (x ==1){"
	"while (x == 2){"
	"while (x ==3){"
	"x=4;"
	"}}}}"
	),
	buildMultipleNestedWhileTest() };

CFG* buildMultipleNestedIfWhileTest() {

	CFG* cfg = new CFG();

	cfg->AddEdge(1, 2);
	cfg->AddEdge(1, 6);
	cfg->AddEdge(2, 3);
	cfg->AddEdge(2, 7);
	cfg->AddEdge(3, 4);
	cfg->AddEdge(3, 5);
	cfg->AddEdge(4, 2);
	cfg->AddEdge(5, 2);
	cfg->AddEdge(6, 7);

	return cfg;
}

extern CFGTestCase MultipleNestedIfWhileTest = {
	new string("procedure Example {"
	"if ( x < 5 ) then {"
	"while ( y < 4) {"
	"if ( x < 3) then {"
	"x = 2; } else {"
	"x = 3; }}}"
	"else { x = 2; }"
	"x = 5;"
	"}"
	),
	buildMultipleNestedIfWhileTest() };

CFG* buildMultipleProcTest() {
	CFG* cfg = new CFG();

	cfg->AddEdge(1, 2);
	cfg->AddEdge(3, 4);
	cfg->AddEdge(5, 6);
	
	return cfg;
}

extern CFGTestCase MultipleProcTest = {
	new string("procedure One {"
	"x = 1;"
	"y = 2;"
	"}"
	"procedure One {"
	"x = 1;"
	"y = 2;"
	"}"
	"procedure One {"
	"x = 1;"
	"y = 2;"
	"}"
	),
	buildMultipleProcTest() };