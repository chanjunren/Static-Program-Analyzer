#ifndef _GUINODE_H_
#define _GUINODE_H_

#include <TNode.h>

using namespace std;


const string hex_procedure = "#ee4035";
const string hex_stmt = "#F7CAC9";
const string hex_const = "#0392cf";
const string hex_op = "#7bc043";
const string hex_others = "#fdf498";


class GUINode {
public:
	TNode* parent_;
	TNode* node_;
	int x_; 
	int y_;
	int stmt_idx_;
	string hex_color_code_;
	
	vector<GUINode*> edges;
	GUINode(TNode* node, TNode* parent, int x, int y);
	GUINode(int stmt_idx, int x, int y);
	string GetHexCode(TNode* node);


};

#endif