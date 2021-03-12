#include <fstream>
#include <string>
#include <stdexcept>

#include <CFG.h>
#include <CFGBuilder.h>
#include <PKB.h>
#include "TestWrapper.h"
#include "Tokenizer.h"
#include <FrontendWrapper.h>
#include "frontend/FileReader.h"
#include "pql/PQLDriver.h"
#include "pkb/DesignExtractor.h"
#include "pkb/PatternManager.h"

using namespace std;

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;

AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}

// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
	pkb = new PKB();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(string filename) {

	try {
		FileReader file_reader(filename);
		string file_contents = file_reader.ReadFile();
		FrontendWrapper frontend_wrapper(file_contents);


		TNode* ast_root = frontend_wrapper.GetAST();
		CFG* cfg = frontend_wrapper.GetCFG(ast_root);

		TestWrapper::pkb->SetASTRoot(ast_root);
		TestWrapper::pkb->SetCFG(*cfg);

		DesignExtractor::ExtractData(pkb->GetDataManager(), pkb->GetASTRoot());
		DesignExtractor::ExtractAssignStmtInProcs(pkb->GetDataManager(), pkb->GetASTRoot());
		DesignExtractor::ExtractFollows(pkb->GetRelationManager(), pkb->GetASTRoot());
		DesignExtractor::ExtractParent(pkb->GetRelationManager(), pkb->GetASTRoot());
		DesignExtractor::ExtractCalls(pkb->GetRelationManager(), pkb->GetASTRoot());
		DesignExtractor::ExtractModifies(pkb->GetRelationManager(), pkb->GetASTRoot());
		DesignExtractor::ExtractUses(pkb->GetRelationManager(), pkb->GetASTRoot());
		DesignExtractor::ExtractAssignPattern(pkb->GetPatternManager(), pkb->GetASTRoot());
		DesignExtractor::ExtractContainerPattern(pkb->GetPatternManager(), pkb->GetASTRoot());
	}
	catch (char* exception) {
		std::cout << exception << std::endl;
		exit(1);
	}
	catch (logic_error& e) {
		cout << e.what() << endl;
		exit(1);
	}

}

// method to evaluating a query
void TestWrapper::Evaluate(string query, list<string>& results){
	// call your evaluator to evaluate the query here
	// ...code to evaluate query...
	PQLDriver* main = new PQLDriver();
	STRING_SET evaluated_result = main->Query(query);

	// store the answers to the query in the results list (it is initially empty)
	// each result must be a string.
	for (string res : evaluated_result) {
		results.push_back(res);
	}
}
