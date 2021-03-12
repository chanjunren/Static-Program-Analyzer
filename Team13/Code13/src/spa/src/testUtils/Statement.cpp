#include <vector>
#include <algorithm>
#include <iostream>

#include "Statement.h"
#include "frontend/Token.h"

Statement::Statement() {}

void printToken(Token t) {
	std::cout << t.GetValue() << " ";
}

void Statement::addToken(Token token) {
	Statement::tokensList.push_back(token);
}

void Statement::print() {
	std::cout << "STATEMENT: ";
	std::for_each(Statement::tokensList.begin(), Statement::tokensList.end(), printToken);
	std::cout << std::endl;
}
