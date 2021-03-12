#include <unordered_map>

#include <TNode.h>
#include <CFG.h>
#include <CFGBuilder.h>

using namespace std;

CFGBuilder::CFGBuilder(TNode* root_node) {

	cfg_ = new CFG();

	for (TNode* proc_node : root_node->GetChildrenVector()) {
		BuildCFG(proc_node);
	}

	//PrintCFG();
}

void CFGBuilder::BuildCFG(TNode* proc_node) {

	TNode* first_stmt_list_node = proc_node->GetChildrenVector().at(1);
	vector<TNode*> first_stmt_list = first_stmt_list_node->GetChildrenVector();

	TraverseAST(first_stmt_list);
}

void CFGBuilder::TraverseAST(vector<TNode*> stmt_list) {

	vector<TNode*> stmt_list_filtered = FilterStmts(stmt_list);

	//add edge from every sibling to the next
	for (int i = 0; i < stmt_list_filtered.size() - 1; i++) {
		TNode* node = stmt_list_filtered.at(i);
		TNode* node_next = stmt_list_filtered.at(i + 1);

		cfg_->AddEdge(node->GetStmtIndex(), node_next->GetStmtIndex());

	}

	int i = 0;

	for (TNode* node: stmt_list_filtered) {
		TNode* node_next = NULL;
		if (i + 1 < stmt_list_filtered.size()) {
			node_next = stmt_list_filtered.at(i + 1);
		}
		for (TNode* child : node->GetChildrenVector()) {
			if (IsStmtList(child)) {

				vector<TNode*> child_stmt_list = child->GetChildrenVector();
				vector<TNode*> child_stmt_list_filtered = FilterStmts(child_stmt_list);

				if (child_stmt_list_filtered.size() > 0) {

					//add edge from parent to first child
					TNode* child_first = child_stmt_list_filtered.at(0);
					cfg_->AddEdge(node->GetStmtIndex(), child_first->GetStmtIndex());

					TNode* child_last = child_stmt_list_filtered.at(child_stmt_list_filtered.size() - 1);
					
					if (node->GetNodeType() == TNode::NODE_TYPE::whileStmt) {
						
						//initialise vector pointer to leaf_nodes
						vector<TNode*>* leaf_nodes = new vector<TNode*>{};


						GetLeafNodes(leaf_nodes, child_last);

						for (TNode* leaf : *leaf_nodes) {
							cfg_->AddEdge(leaf->GetStmtIndex(), node->GetStmtIndex());
						}

					}

					if (node->GetNodeType() == TNode::NODE_TYPE::ifStmt && node_next != NULL) {
						vector<TNode*>* leaf_nodes = new vector<TNode*>{};

						GetLeafNodes(leaf_nodes, child_last);

						for (TNode* n : *leaf_nodes) {
							cfg_->AddEdge(n->GetStmtIndex(), node_next->GetStmtIndex());
						}

						//write similar function to while
						cfg_->RemoveEdge(node->GetStmtIndex(), node_next->GetStmtIndex());

					}
				}

				TraverseAST(child_stmt_list);
			}
		}

		i += 1;
	}


}

bool CFGBuilder::IsStmtList(TNode* ast_node) {
	return ast_node->GetNodeType() == TNode::NODE_TYPE::stmtList;
}

bool CFGBuilder::IsStmt(TNode* ast_node) {
	TNode::NODE_TYPE node_type = ast_node->GetNodeType();

	return(
		node_type == TNode::NODE_TYPE::assignStmt
		|| node_type == TNode::NODE_TYPE::callStmt
		|| node_type == TNode::NODE_TYPE::ifStmt
		|| node_type == TNode::NODE_TYPE::printStmt
		|| node_type == TNode::NODE_TYPE::whileStmt
		|| node_type == TNode::NODE_TYPE::readStmt
		);
}

vector<TNode*> CFGBuilder::FilterStmts(vector<TNode*> stmt_list) {
	
	vector<TNode*> filtered_list;
	
	for (TNode* stmt : stmt_list) {
		if (IsStmt(stmt)) {
			filtered_list.push_back(stmt);
		} 
	}

	return filtered_list;
}

vector<TNode*> CFGBuilder::FilterStmtsAndStmtLists(vector<TNode*>stmt_list) {
	vector<TNode*> filtered_list;

	for (TNode* stmt : stmt_list) {
		if (IsStmt(stmt) || IsStmtList(stmt)) {
			filtered_list.push_back(stmt);
		}
	}

	return filtered_list;
}

void CFGBuilder::GetLeafNodes(vector<TNode*>* leaf_nodes, TNode* root_node) {
	//cout << root_node->getData() << endl;
	if (root_node->GetNodeType() == TNode::NODE_TYPE::whileStmt 
	|| FilterStmtsAndStmtLists(root_node->GetChildrenVector()).size() == 0) {
		leaf_nodes->push_back(root_node);
	}
	else {

		vector<TNode*> nodes = root_node->GetChildrenVector();

		vector<TNode*> filtered_nodes = FilterStmtsAndStmtLists(nodes);

		for (TNode* stmt : filtered_nodes) {

			vector<TNode*> children = stmt->GetChildrenVector();

			TNode* last_child = children.at(children.size() - 1);

			/*if (last_child->GetNodeType() != TNode::NODE_TYPE::whileStmt) {*/
				GetLeafNodes(leaf_nodes, last_child);
			//}

		}


	}

}

void CFGBuilder::PrintCFG() {

	for (auto const& pair : cfg_->GetAdjacencyList()) {
		cout << pair.first << "->" << "[";

		STMT_IDX_SET set = *pair.second;
		int i = 0;

		for (auto const& edge : set) {
			cout << edge;
			if (i < set.size() - 1) {
				cout << ", ";
			}

			i += 1;
		}

		cout << "]" << endl;
		
	}
	
	/*for (STMT_IDX idx = 1; idx < cfg_->GetAdjacencyList().size(); idx++) {

		cout << idx << "->" << "[";

		STMT_IDX_SET set = cfg_->GetAllNeighbors(idx);

		int i = 0;

		for (auto const& edge : set) {
			cout << edge;
			if (i < set.size() - 1) {
				cout << ", ";
			}

			i += 1;
		}

		cout << "]" << endl;
	}*/

}