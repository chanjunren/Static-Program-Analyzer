#pragma once

#include <vector>

#include "frontend/Token.h"

// temp class for testing
class Statement {
public:
	std::vector<Token> tokensList;
	Statement();
	void addToken(Token token);
	void print();
};