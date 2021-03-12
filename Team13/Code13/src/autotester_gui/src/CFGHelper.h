#ifndef CFGHELPER_H
#define CFGHELPER_H

#include <QMainWindow>
#include <qgraphicsscene.h>
#include "GUIWrapper.h"
#include <TNode.h>
#include <QGraphicsItem>
#include "GUINode.h"
#include "CFG.h"
#include <map>

typedef map<STMT_IDX, GUINode*> CFG_NODE_MAP;

class CFGHelper
{

public:
	CFGHelper(string file_contents);
	CFG_NODE_MAP* GetCFGNodeMap();
	int GetMaxStmtIdx();

private:
	CFG* cfg_;
	CFG_NODE_MAP* cfg_node_map_;

	void PopulateCFGNodeMap(int max);

	const int unit_x_ = 100;
	const int unit_y_ = 100;
};

#endif // CFGHELPER_H
