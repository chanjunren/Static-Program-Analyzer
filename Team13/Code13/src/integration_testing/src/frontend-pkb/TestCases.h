#include <TNode.h>;
#include <FrontendTypes.h>
#include <string>

using namespace std;

typedef struct {
	string* SourceProgram;
	TNode* root;
}TestCase;

typedef struct {
	string* SourceProgram;
	CFG* cfg;
}CFGTestCase;

// AST TEST CASES
extern TestCase AssgnTest1;
extern TestCase AssgnTest2;
extern TestCase AssgnTest3;
extern TestCase AssgnTest4;

extern TestCase IfTestCase1;
extern TestCase IfTestCase2;
extern TestCase IfTestCase3;
extern TestCase IfTestCase4;
extern TestCase IfTestCase5;

extern TestCase WhileTestCase1;
extern TestCase WhileTestCase2;
extern TestCase WhileTestCase3;
extern TestCase WhileTestCase4;

extern TestCase ExtraTokensCase1;
extern TestCase ExtraTokensCase2;
extern TestCase ExtraTokensCase3;

extern TestCase ValidReadCase1;
extern TestCase InvalidReadCase1;

extern TestCase ValidPrintCase1;
extern TestCase InvalidPrintCase1;

extern TestCase ValidCallCase1;
extern TestCase InvalidCallCase1;

// CFG TEST CASES
extern CFGTestCase SingleStmtTest;
extern CFGTestCase DoubleStmtTest;
extern CFGTestCase IfStmtTest;
extern CFGTestCase IfStmtWithExitTest;
extern CFGTestCase WhileStmtTest;
extern CFGTestCase WhileStmtWithExitTest;
extern CFGTestCase NestedIfWhileStmtTest;
extern CFGTestCase NestedWhileIfStmtTest;
extern CFGTestCase MultipleNestedIfWhileTest;
extern CFGTestCase MultipleProcTest;