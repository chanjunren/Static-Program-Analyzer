#include <iostream>

#include <CustomTypes.h>
#include <ExprValidator.h>
#include <FrontendTypes.h>
#include <TNode.h>

using namespace std;

void ExprValidator::Validate(TNode* root_node) {

	ValidateExpression(root_node);

	for (TNode* child : root_node->GetChildrenVector()) {
		Validate(child);
	}

}

void ExprValidator::ValidateExpression(TNode* ast_node) {

	if (ast_node->GetNodeType() == TNode::condExpr) {
	//allowed types are condExpr, relExpr
		for (TNode* child : ast_node->GetChildrenVector()) {
			if (child->GetNodeType() != TNode::condExpr
				&& child->GetNodeType() != TNode::relExpr) {
				throw "Invalid Expression";
			}
		}
	}

	if (ast_node->GetNodeType() == TNode::relExpr) {
	//allowed types are expr or constant or var
		for (TNode* child : ast_node->GetChildrenVector()) {
				if(child->GetNodeType() != TNode::expr
				&& child->GetNodeType() != TNode::constValue
				&& child->GetNodeType() != TNode::varName) {
				throw "Invalid Expression";
			}
		}
	}

	if (ast_node->GetNodeType() == TNode::expr) {
	// allowed types are expr and const and var
		for (TNode* child : ast_node->GetChildrenVector()) {
			if (child->GetNodeType() != TNode::constValue
				&& child->GetNodeType() != TNode::expr
				&& child->GetNodeType() != TNode::varName) {
				throw "Invalid Expression";
			}
		}
	}

}