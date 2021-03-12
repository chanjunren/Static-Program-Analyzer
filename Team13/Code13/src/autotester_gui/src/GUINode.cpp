#include "GUINode.h"

GUINode::GUINode(TNode* node, TNode* parent, int x, int y) {
    node_ = node;
    parent_ = parent;
    x_ = x;
    y_ = y; 
	hex_color_code_ = GetHexCode(node);
}

GUINode::GUINode(int stmt_idx, int x, int y) {
    stmt_idx_ = stmt_idx;
    x_ = x;
    y_ = y;
	hex_color_code_ = hex_stmt;
}

string GUINode::GetHexCode(TNode* node) {

    switch (node->GetNodeType()) {
	case TNode::procedure:
	case TNode::procName:
		return hex_procedure;
	case TNode::readStmt:
	case TNode::printStmt:
	case TNode::callStmt:
	case TNode::whileStmt:
	case TNode::ifStmt:
	case TNode::assignStmt:
		return hex_stmt;
	case TNode::condExpr:
	case TNode::relExpr:
	case TNode::relFactor:
	case TNode::expr:
	case TNode::term:
	case TNode::factor:
		return hex_op;
	case TNode::varName:
	case TNode::constValue:
		return hex_const;
	default:
		return hex_others;
    }

}