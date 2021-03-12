
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "GUIWrapper.h"
#include <iostream>
#include <QFileDialog>
#include <QtGui>
#include <QTextStream>
#include <FrontendWrapper.h>
#include <TNode.h>
#include <vector>
#include <list>
#include "GUINode.h"
#include <QVector>
#include <CFG.h>
#include "ASTHelper.h"


using namespace std; 

ASTHelper::ASTHelper(string file_contents) {

	FrontendWrapper wrapper(file_contents);
	root_node_ = wrapper.GetAST();

}

AST_GUI_NODE_TREE* ASTHelper::GetGUINodeTree() {
	
	int max_depth = CountMaxDepth(root_node_, 0);

	InitialiseListOfNodeLists(max_depth);

	int leaf_x = 0;

	// root_node_->Print(root_node_);
	
	PopulateNodeList(root_node_, NULL, 0, &leaf_x);

	return ast_gui_node_tree;
}

int ASTHelper::CountMaxDepth(TNode* root_node, int depth) {
	//base case
	if (root_node->GetChildrenVector().size() == 0) {
		return depth;
	}
	else {

		vector<int> depths;

		for (TNode* node : root_node->GetChildrenVector()) {
			depths.push_back(CountMaxDepth(node, depth + 1));
		}

		int max = depths.at(0);

		for (int depth : depths) {
			if (depth > max) {
				max = depth;
			}
		}

		return max;
	}
}

void ASTHelper::PopulateNodeList(TNode* root_node, TNode* parent_node, int curr_depth, int* leaf_x) {

	if (curr_depth > ast_gui_node_tree->size()) {
		return;
	}

	NODE_LIST* node_list = ast_gui_node_tree->at(curr_depth);

	int y = curr_depth * unit_y_;
	int pos = node_list->size();
	int x = pos * unit_x_;

	if (root_node->GetChildrenVector().size() == 0) {
		GUINode* gui_node = new GUINode(root_node, parent_node, *leaf_x, y);
		*leaf_x += unit_x_;
		node_list->push_back(gui_node);
	}
	else {

		for (TNode* child : root_node->GetChildrenVector()) {
			PopulateNodeList(child, root_node, curr_depth + 1, leaf_x);
		}

		NODE_LIST* child_node_list = ast_gui_node_tree->at(curr_depth + 1);
		NODE_LIST* filtered_child_node_list = new NODE_LIST();

		for (GUINode* gui_node : *child_node_list) {
			if (gui_node->parent_ == root_node) {
				filtered_child_node_list->push_back(gui_node);
			}
		}

		int start_x = filtered_child_node_list->at(0)->x_;
		int end_x = filtered_child_node_list->at(filtered_child_node_list->size() - 1)->x_;

		int parent_x = (end_x + start_x) / 2;

		GUINode* gui_node = new GUINode(root_node, parent_node, parent_x, y);
		node_list->push_back(gui_node);

	}

}

void ASTHelper::PrintNodeList() {

	int level = 0;
	for (NODE_LIST* node_list : *ast_gui_node_tree) {

		cout << "Level " << level << ": [";

		int idx = 0;
		for (GUINode* gui_node : *node_list) {
			cout << gui_node->node_->getData();
			if (idx < node_list->size() - 1) {
				cout << ", ";
			}
		}
		cout << "]" << endl;
		level += 1;
	}
}

void ASTHelper::InitialiseListOfNodeLists(int max_depth) {

	ast_gui_node_tree = new AST_GUI_NODE_TREE();

	for (int i = 0; i < max_depth + 1; i++) {
		NODE_LIST* node_list = new NODE_LIST;
		ast_gui_node_tree->push_back(node_list);
	}
}