#include <string>

using namespace std;

typedef struct{
	string* SourceProgram;
}TestCase;

extern TestCase CyclicTestCase1; 
extern TestCase CyclicTestCase2;
extern TestCase CyclicTestCase3;
extern TestCase CyclicTestCase4;

extern TestCase AssgnTest1;
extern TestCase AssgnTest2;
extern TestCase AssgnTest3;
extern TestCase AssgnTest4;

extern TestCase IfTestCase1;
extern TestCase IfTestCase2;
extern TestCase IfTestCase3;
extern TestCase InvalidIfTestCase1;
extern TestCase InvalidIfTestCase2;

extern TestCase WhileTestCase1;
extern TestCase WhileTestCase2;
extern TestCase InvalidWhileTestCase1;
extern TestCase InvalidWhileTestCase2;

extern TestCase ExtraTokensCase1;
extern TestCase ExtraTokensCase2;
extern TestCase ExtraTokensCase3;

extern TestCase ValidReadCase1;
extern TestCase InvalidReadCase1;

extern TestCase ValidPrintCase1;
extern TestCase InvalidPrintCase1;

extern TestCase ValidCallCase1;
extern TestCase InvalidCallCase1;