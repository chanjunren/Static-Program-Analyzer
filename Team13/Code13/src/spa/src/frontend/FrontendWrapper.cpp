#include <ASTBuilder.h>
#include <CFGBuilder.h>
#include <FileReader.h>
#include <FrontendWrapper.h>
#include <Tokenizer.h>
#include <TNode.h>
#include <SimpleValidator.h>

FrontendWrapper::FrontendWrapper(string file_contents) {

	Tokenizer tokenizer(file_contents);
	token_list_ = tokenizer.GetTokenList();
		
	SimpleValidator simple_validator;
	simple_validator.IsValid(tokenizer.GetTokenList());

}

CFG* FrontendWrapper::GetCFG(TNode* ast_root_node) {

	CFGBuilder cfg_builder = CFGBuilder(ast_root_node);
	CFG* cfg = cfg_builder.cfg_;

	return cfg;
}

TNode* FrontendWrapper::GetAST() {

	ASTBuilder ast_builder;
	TNode* ast_root_node = ast_builder.BuildMainPrgNode(token_list_);


	// ast_root_node->Print(ast_root_node);

	return ast_root_node;

}