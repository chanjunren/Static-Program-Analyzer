#include "mainwindow.h"
#include "ASTDrawer.h"
#include "LabelHelper.h"

ASTDrawer::ASTDrawer(QGraphicsScene* scene, AST_GUI_NODE_TREE* ast_gui_node_tree) {
	scene_ = scene;
	ast_gui_node_tree_ = ast_gui_node_tree;

	outline_pen_ = QPen(Qt::black, 1);
	highlight_pen_ = QPen(Qt::red, 3, Qt::DashDotDotLine);
}

void ASTDrawer::DrawAST() {
	cout << "DRAWING" << endl;
	for (NODE_LIST* node_list : *ast_gui_node_tree_) {
		for (GUINode* gui_node : *node_list) {
			// cout << gui_node->node_->getData() << endl;
			DrawASTNode(gui_node);
			DrawASTLabel(gui_node);
		}
	}

	for (int i = 1; i < ast_gui_node_tree_->size(); i++) {
		NODE_LIST* node_list = ast_gui_node_tree_->at(i);
		NODE_LIST* parent_node_list = ast_gui_node_tree_->at(i - 1);

		for (GUINode* gui_node : *node_list) {
			for (GUINode* parent_node : *parent_node_list) {
				if (gui_node->parent_ == parent_node->node_) {
					//draw edge
					scene_->addLine(
						QLine(
							gui_node->x_ + line_offset_x,
							gui_node->y_,
							parent_node->x_ + line_offset_x,
							parent_node->y_ + line_offset_y
						), outline_pen_);
				}
			}
		}
	}
}

void ASTDrawer::DrawASTNode(GUINode* gui_node) {
	scene_->addEllipse(gui_node->x_, gui_node->y_, diameter_, diameter_, outline_pen_, QBrush(QColor(gui_node->hex_color_code_.c_str())));
}

void ASTDrawer::ColorASTNode(GUINode* gui_node) {
	scene_->addEllipse(gui_node->x_, gui_node->y_, diameter_, diameter_, highlight_pen_, QBrush(QColor(gui_node->hex_color_code_.c_str())));
}

void ASTDrawer::DrawASTLabel(GUINode* gui_node) {
	QGraphicsTextItem* text = scene_->addText(QString::fromStdString(gui_node->node_->GetGUIData()));
	text->setPos(double(gui_node->x_) - text_offset_x, double(gui_node->y_) + text_offset_y);
}

void ASTDrawer::ColorResults(list<string> results) {

	if (ast_gui_node_tree_ == NULL) {
		return;
	}

	for (auto const& result : results) {

		vector<string> results_strings = LabelHelper::SplitString(result);

		for (string results_string : results_strings) {
			for (NODE_LIST* node_list : *ast_gui_node_tree_) {
				for (GUINode* node : *node_list) {
					if (node->node_->GetSelectValue() == results_string) {
						ColorASTNode(node);
					}
				}
			}
		}

	}
}