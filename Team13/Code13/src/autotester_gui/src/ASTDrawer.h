#ifndef ASTDRAWER_H
#define ASTDRAWER_H

#include "qgraphicsscene.h"
#include <TNode.h>
#include "GUINode.h"
#include <string>
#include "ASTHelper.h"

class ASTDrawer
{

public:
	QGraphicsScene* scene_;
	AST_GUI_NODE_TREE* ast_gui_node_tree_;
	ASTDrawer(QGraphicsScene* scene, AST_GUI_NODE_TREE* ast_gui_node_tree);
	void DrawAST();
	void ColorResults(list<string> results);
private:
	void ColorASTNode(GUINode* gui_node);
	void DrawASTNode(GUINode* gui_node);
	void DrawASTLabel(GUINode* gui_node);

	const int diameter_ = 30;
	const int line_offset_x = diameter_ / 2;
	const int line_offset_y = diameter_;
	const int text_offset_x = 10;
	const int text_offset_y = diameter_;

	QPen outline_pen_;
	QPen highlight_pen_;
};

#endif // ASTDRAWER_H
