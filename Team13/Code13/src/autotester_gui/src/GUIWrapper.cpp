#include "GUIWrapper.h"

#include <iostream>
#include <FileReader.h>

#include <FrontendWrapper.h>

#include "pql/PQLDriver.h"

// a default constructor
GUIWrapper::GUIWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
	pkb = new PKB();
}

// method for parsing the SIMPLE source
void GUIWrapper::parse(std::string filename) {

	FileReader file_reader(filename);
	string file_contents = file_reader.ReadFile();
	FrontendWrapper frontend_wrapper(file_contents);

	TNode* ast_root = frontend_wrapper.GetAST();
	CFG* cfg = frontend_wrapper.GetCFG(ast_root);

	GUIWrapper::pkb->SetASTRoot(ast_root);
	GUIWrapper::pkb->SetCFG(*cfg);

	DesignExtractor::ExtractData(pkb->GetDataManager(), pkb->GetASTRoot());
	DesignExtractor::ExtractFollows(pkb->GetRelationManager(), pkb->GetASTRoot());
	DesignExtractor::ExtractParent(pkb->GetRelationManager(), pkb->GetASTRoot());
	DesignExtractor::ExtractCalls(pkb->GetRelationManager(), pkb->GetASTRoot());
	DesignExtractor::ExtractModifies(pkb->GetRelationManager(), pkb->GetASTRoot());
	DesignExtractor::ExtractUses(pkb->GetRelationManager(), pkb->GetASTRoot());
	DesignExtractor::ExtractAssignPattern(pkb->GetPatternManager(), pkb->GetASTRoot());
	DesignExtractor::ExtractContainerPattern(pkb->GetPatternManager(), pkb->GetASTRoot());
}

// method to evaluating a query
void GUIWrapper::Evaluate(std::string query, std::list<std::string>& results){
	// call your evaluator to evaluate the query here
	// ...code to evaluate query...
	PQLDriver* main = new PQLDriver();
	STRING_SET evaluated_result = main->Query(query);

	// store the answers to the query in the results list (it is initially empty)
	// each result must be a string.
	cout << "RESULTS" << endl;
	for (string res : evaluated_result) {
		// cout << res << endl;
		results.push_back(res);
	}
}
