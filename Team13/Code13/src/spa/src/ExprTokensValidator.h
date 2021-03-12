#ifndef _EXPR_TOKENS_VALIDATOR_H
#define _EXPR_TOKENS_VALIDATOR_H

#include <FrontendTypes.h>

using namespace std;

class ExprTokensValidator {
public:
	static bool Validate(TOKEN_LIST expr_list);

private:
	static bool CheckValid(Token prevToken, Token currToken);

};

#endif