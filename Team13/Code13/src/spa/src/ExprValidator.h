#ifndef _EXPRVALIDATOR_H
#define _EXPRVALIDATOR_H

#include <FrontendTypes.h>
#include <TNode.h>

using namespace std;

class ExprValidator {
public:
	static void Validate(TNode* root_node);
private:
	static void ValidateExpression(TNode* ast_node);

};

#endif