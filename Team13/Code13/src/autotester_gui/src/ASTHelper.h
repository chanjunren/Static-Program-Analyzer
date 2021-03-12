#ifndef ASTHELPER_H
#define ASTHELPER_H

#include <TNode.h>
#include "GUINode.h"
#include <string>

typedef vector<GUINode*> NODE_LIST;
typedef vector<NODE_LIST*> AST_GUI_NODE_TREE;

class ASTHelper
{

public:
	ASTHelper(string file_contents);
	AST_GUI_NODE_TREE* GetGUINodeTree();

private:
	TNode* root_node_;
	AST_GUI_NODE_TREE* ast_gui_node_tree;

	int CountMaxDepth(TNode* root_node, int depth);
	void PopulateNodeList(TNode* root_node, TNode* parent_node, int curr_depth, int* leaf_x);
	void PrintNodeList();
	void InitialiseListOfNodeLists(int max_depth);

	const int unit_x_ = 50;
	const int unit_y_ = 50;

};

#endif // ASTHELPER_H
