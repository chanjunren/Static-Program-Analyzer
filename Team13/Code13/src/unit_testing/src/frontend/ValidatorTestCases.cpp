#include <string>
#include "ValidatorTestCases.h"
using namespace std;

extern TestCase AssgnTest1 = {
	new string("procedure AssgnTest1 {"
	"x = y + 5;}") };


extern TestCase AssgnTest2 = {
	new string("procedure AssgnTest2 {"
	"x = 7;}") };

extern TestCase AssgnTest3 = {
	new string("procedure AssgnTest3 {"
	"x = y;}") };

extern TestCase AssgnTest4 = {
	new string("procedure AssgnTest4 {"
	"x = y + 5 * (a + b) - c - d * d;}") };

extern TestCase IfTestCase1 = {
	new string("procedure IfTestCase1 {"
	"if (x < 7) then {"
	"print hello;"
	"} else {"
	"print something;"
	"}}") };

extern TestCase IfTestCase2 = {
	new string("procedure IfTestCase2 {"
	"if (true) then {"
	"print hello;"
	"} else {"
	"call something;"
	"}}") };

extern TestCase IfTestCase3 = {
	new string("procedure IfTestCase5 {"
	"if (x < y + 5 * (a + b) - c - d * d) then {"
	"print hello;"
	"} else {"
	"print something;"
	"}}")
};

extern TestCase InvalidIfTestCase1 = {
	new string("procedure IfTestCase3 {"
	"if (x < 7) then {"
	"print hello;"
	"}}") };

extern TestCase InvalidIfTestCase2 = {
	new string("procedure IfTestCase4 {"
	"if (x < 7) {"
	"print hello;"
	"}}")
};

extern TestCase WhileTestCase1 = {
	new string("procedure WhileTestCase1 {"
	"while (x < 7) {"
	"print hello;"
	"}}")
};

extern TestCase WhileTestCase2{
	new string("procedure WhileTestCase4 {"
	"while (x < y + 5 * (a + b) - c - d * d) {"
	"print hello;"
	"}}")
};

extern TestCase InvalidWhileTestCase1 = {
	new string("procedure WhileTestCase2 {"
	"while (true) {"
	"print hello;"
	"}}")
};

extern TestCase InvalidWhileTestCase2 = {
	new string("procedure WhileTestCase3 {"
	"while (x < 7) then {"
	"print hello;"
	"}}")
};

extern TestCase ExtraTokensCase1 = {
	new string("procedure ExtraTokensCase1 {"
	"print hello;;"
	"}")};

extern TestCase ExtraTokensCase2 = {
	new string("procedure ExtraTokensCase1 {"
	"print hello;"
	"}}")};

extern TestCase ExtraTokensCase3 = {
	new string("procedure hihi ExtraTokensCase3 {"
	"print hello;"
	"}")};

extern TestCase ValidReadCase1 = {
	new string("procedure ValidReadCase1 {"
	"read HarukiMurakami;}")
};

extern TestCase InvalidReadCase1 = {
	new string("procedure ValidReadCase1 {"
	"red HarukiMurakami;}")
};

extern TestCase ValidPrintCase1 = {
	new string("procedure ValidPrintCase1 {"
	"print HarukiMurakami;}")
};

extern TestCase InvalidPrintCase1 = {
	new string("procedure ValidReadCase1 {"
	"prnt HarukiMurakami;}")
};


extern TestCase ValidCallCase1 = {
	new string("procedure ValidReadCase1 {"
	"call HarukiMurakami;}")
};

extern TestCase InvalidCallCase1 = {
	new string("procedure InvalidCallCase1 {"
	"cal HarukiMurakami;}")
};

extern TestCase CyclicTestCase1 = { new string("procedure A {"
	"call B;}"
	"procedure B { call C;}"
	"procedure C { call A; }") 
};

extern TestCase CyclicTestCase2 = { new string("procedure A {"
	"call A;}")
};

extern TestCase CyclicTestCase3 = { new string("procedure A {"
	"call B;"
	"call C;"
	"call D;}"
	"procedure B {"
	"call C;}"
	"procedure C {"
	"print hello;}"
	"procedure D {"
	"call E;}"
	"procedure E {"
	"call F;}"
	"procedure F{"
	"call D;}"
) };