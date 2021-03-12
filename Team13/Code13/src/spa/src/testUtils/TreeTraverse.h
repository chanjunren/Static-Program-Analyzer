#pragma once

#include <queue>
#include "TNode.h"

/**
* Helper function for Frontend-PKB integration test.
* Simply prints out a level order traversal of a given tree
**/
class TreeTraverse {
public:
	static string traverse(TNode* root);
};