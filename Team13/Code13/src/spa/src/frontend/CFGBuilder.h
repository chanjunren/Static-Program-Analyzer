#ifndef _CFGBUILDER_H_
#define _CFGBUILDER_H_

#include <vector>

#include "FrontendTypes.h"
#include <TNode.h>
#include "Token.h"
#include <CFG.h>

using namespace std;

class CFGBuilder {
public:

	TNode* ast_root_;
	CFG* cfg_;

	CFGBuilder(TNode* root_node);
	void BuildCFG(TNode* proc_node);
	void TraverseAST(vector<TNode*> stmt_list);
	bool IsStmt(TNode* ast_node);
	bool IsStmtList(TNode* ast_node);
	vector<TNode*> FilterStmts(vector<TNode*> stmt_list);
	vector<TNode*> FilterStmtsAndStmtLists(vector<TNode*> stmt_list);
	void GetLeafNodes(vector<TNode*>* leaf_nodes, TNode* root_node);
	void PrintCFG();

private:
	
};

#endif