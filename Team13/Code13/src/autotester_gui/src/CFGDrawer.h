#ifndef CFGDRAWER_H
#define CFGDRAWER_H

#include "qgraphicsscene.h"
#include <TNode.h>
#include "GUINode.h"
#include <string>
#include "CFGHelper.h"

class CFGDrawer
{

public:
	QGraphicsScene* scene_;
	CFG_NODE_MAP* cfg_nodes_map_;
	CFGDrawer(QGraphicsScene* scene, CFG_NODE_MAP* cfg_nodes_map);
	void DrawCFG();
	void ColorResults(list<string> results);
private:
	void ColorCFGNode(GUINode* gui_node);
	void DrawCFGNode(GUINode* gui_node);
	void DrawCFGLabel(GUINode* gui_node);
	void DrawArrow(QPointF p1, QPointF p2);

	const int diameter_ = 40;
	const int line_offset_x = diameter_ / 2;
	const int line_offset_y = diameter_;
	const int text_offset_x = -6;
	const int text_offset_y = -18;
	const int font_size_ = 14;

	QPen outline_pen_;
	QPen highlight_pen_;
};

#endif // CFGDRAWER_H
