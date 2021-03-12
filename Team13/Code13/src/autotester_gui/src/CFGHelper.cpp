#include <FrontendWrapper.h>
#include <TNode.h>
#include "GUINode.h"
#include <CFG.h>
#include "CFGHelper.h"


using namespace std;

CFGHelper::CFGHelper(string file_contents) {
	FrontendWrapper wrapper(file_contents);
	TNode* root_node = wrapper.GetAST();
	cfg_ = wrapper.GetCFG(root_node);

	if (cfg_->GetAdjacencyList().size() == 0) {
		throw "There must be at least one procedure with >1 statement for CFG to be drawn";
	}
}

CFG_NODE_MAP* CFGHelper::GetCFGNodeMap() {
	PopulateCFGNodeMap(GetMaxStmtIdx());

	return cfg_node_map_;
}

int CFGHelper::GetMaxStmtIdx() {
	
	int max = 0;
	for (auto const& pair : cfg_->GetAdjacencyList()) {
		if (pair.first > max) {
			max = pair.first;
		}
		for (auto const& edge : *pair.second) {
			if (edge > max) {
				max = edge;
			}
		}
	}

	return max;
}

void CFGHelper::PopulateCFGNodeMap(int max) {

	cfg_node_map_ = new CFG_NODE_MAP();

	int y = 0;

	STMT_IDX_SET* edges = cfg_->GetAdjacencyList()[1];

	for (STMT_IDX stmt_idx = 1; stmt_idx < max + 1; stmt_idx++) {

		if (cfg_->GetAdjacencyList().find(stmt_idx) == cfg_->GetAdjacencyList().end()) {
			continue;
		}

		STMT_IDX_SET edges = *cfg_->GetAdjacencyList()[stmt_idx];

		GUINode* parent_node;

		//if cant find parent
		if (cfg_node_map_->find(stmt_idx) == cfg_node_map_->end()) {
			(*cfg_node_map_)[stmt_idx] = new GUINode(stmt_idx, unit_x_, y);
			y += unit_y_;
		}

		parent_node = (*cfg_node_map_)[stmt_idx];

		int ctr = 0;

		bool added_new = false;

		for (auto const& edge : edges) {

			GUINode* child_node;

			if (cfg_node_map_->find(edge) == cfg_node_map_->end()) {
				(*cfg_node_map_)[edge] = new GUINode(edge, parent_node->x_ + (ctr * unit_x_), y);
				added_new = true;
			}

			child_node = (*cfg_node_map_)[edge];
			parent_node->edges.push_back(child_node);
			ctr += 1;
		}

		if (added_new) {
			y += unit_y_;
		}
	}

}