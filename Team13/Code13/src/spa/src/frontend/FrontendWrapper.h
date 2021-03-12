#ifndef _FRONTENDWRAPPER_H_
#define _FRONTENDWRAPPER_H_

#include <string>

#include <FrontendTypes.h>
#include <TNode.h>
#include <CFG.h>

using namespace std;

class FrontendWrapper {
public:
	FrontendWrapper(string file_name);

	TOKEN_LIST token_list_;

	TNode* GetAST();
	CFG* GetCFG(TNode* ast_root_node);

private:
};

#endif