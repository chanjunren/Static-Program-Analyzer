#include "mainwindow.h"
#include "CFGDrawer.h"
#include "LabelHelper.h"
#include "GUINode.h"

#define M_PI 3.14159265358979323846

CFGDrawer::CFGDrawer(QGraphicsScene* scene, CFG_NODE_MAP* cfg_nodes_map) {
	scene_ = scene;
	cfg_nodes_map_ = cfg_nodes_map;
	outline_pen_ = QPen(Qt::black, 1);
	highlight_pen_ = QPen(Qt::red, 3, Qt::DashDotDotLine);
}

void CFGDrawer::DrawCFG() {

	for (int i = 1; i < cfg_nodes_map_->size()+1; i++) {

		if (cfg_nodes_map_->find(i) == cfg_nodes_map_->end()) {
			continue;
		}

		GUINode* gui_node = (*cfg_nodes_map_)[i];

		DrawCFGNode(gui_node);
		DrawCFGLabel(gui_node);
		int ctr =0;

		while (ctr < gui_node->edges.size()) {

			GUINode* edge_node = gui_node->edges.at(ctr);

			bool isSource = gui_node->y_ < edge_node->y_;

			int x1 = gui_node->x_ + line_offset_x;
			int y1 = gui_node->y_;
			int x2 = edge_node->x_ + line_offset_x;
			int y2 = edge_node->y_;

			//if arrow is from top down, 
			if (y1 < y2) {
				y1 += line_offset_y;
			}
			else {
				y2 += line_offset_y;
			}

			DrawArrow(QPointF(x1, y1), QPointF(x2, y2));

			ctr += 1;
		}

	}

}

void CFGDrawer::DrawCFGNode(GUINode* gui_node) {

	scene_->addEllipse(gui_node->x_, gui_node->y_, diameter_, diameter_, outline_pen_, QBrush(gui_node->hex_color_code_.c_str()));

}

void CFGDrawer::ColorCFGNode(GUINode* gui_node) {
	scene_->addEllipse(gui_node->x_, gui_node->y_, diameter_, diameter_, highlight_pen_, QBrush(gui_node->hex_color_code_.c_str()));

}

void CFGDrawer::DrawCFGLabel(GUINode* gui_node){
	QGraphicsTextItem* text = scene_->addText(QString::fromStdString(to_string(gui_node->stmt_idx_)));
	text->setPos(gui_node->x_ + text_offset_x, gui_node->y_ + text_offset_y);
	text->setFont(QFont("Arial", font_size_));
}

void CFGDrawer::ColorResults(list<string> results) {

	if (cfg_nodes_map_ == NULL) {
		return;
	}

	for (auto const& result : results) {

		vector<string> results_strings = LabelHelper::SplitString(result);

		for (string results_string : results_strings) {

			for (int i = 1; i < cfg_nodes_map_->size() + 1; i++) {
				GUINode* gui_node = (*cfg_nodes_map_)[i];
				if (to_string(gui_node->stmt_idx_) == results_string) {
					ColorCFGNode(gui_node);
					DrawCFGLabel(gui_node);
				}
			}
		}
	}

}

void CFGDrawer::DrawArrow(QPointF start, QPointF end) {

	qreal arrowSize = 10; // size of head

	QLineF line(end, start);

	scene_->addLine(line);

	double angle = std::atan2(-line.dy(), line.dx());

	QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
		cos(angle + M_PI / 3) * arrowSize);
	QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
		cos(angle + M_PI - M_PI / 3) * arrowSize);

	QPolygonF arrow_head;
	arrow_head << line.p1() << arrowP1 << arrowP2;
	scene_->addPolygon(arrow_head, outline_pen_, QBrush(QColor("#000")));
}