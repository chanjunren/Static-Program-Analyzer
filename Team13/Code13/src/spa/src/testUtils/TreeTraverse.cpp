#include <iostream>
#include <queue>
#include <list>

#include "TreeTraverse.h"
#include "../pkb/TNode.h"

using namespace std;

string TreeTraverse::traverse(TNode* root) {

	string result = "";
	
	if (root == NULL) {
		return result;
	}

	std::queue<TNode*> queue;

	queue.push(root);

	std::cout << "Printing Level Order Traversal" << std::endl;

	while (!queue.empty()) {
		TNode* node = queue.front();
		queue.pop();

		result += node->getData();

		for (TNode* cNode : node->GetChildrenVector()) {
			queue.push(cNode);
		}
	}

	return result;

}