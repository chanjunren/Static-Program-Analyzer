#include "PKB.h"
#include "PQLDriver.h"
#include "PQLEvaluator.h"
#include "PQLParser.h"
#include "PQLProjector.h"
#include "PQLOptimizedEvaluator.h"
#include "QueryInfo.h"
#include "QueryResult.h"

#include <iostream>
#include <chrono> 

using namespace std::chrono;

STRING_SET PQLDriver::Query(string query_string) {
	if (PQL_DEBUG) {
		cout << "PQLDriver - Query" << endl;
	}

	PQLParser parser = PQLParser();
	PQLProjector projector = PQLProjector();

	STRING_SET final_result;

	if (PQL_DEBUG) {
		cout << "Query: " << query_string << endl;
	}

	auto start = high_resolution_clock::now();

	QueryInfo parsed_info = parser.Parse(query_string);
	QueryResult result = QueryResult();

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "Time taken for PQLParser = " << duration.count() << "ms" << endl;

	if (!parsed_info.IsQueryInfoValid()) {
		// Query invalid - get false result: bool/empty
		result.SetResult(parsed_info.GetFalseResult());

		if (PQL_DEBUG) {
			if (parsed_info.IsSemanticsInvalid()) {
				cout << "Query - Semantically Invalid" << endl;
			}
			else {
				cout << "Query - Syntactically Invalid" << endl;
			}
		}
	}
	else {
		// loop: check whats in Query info
		if (PQL_DEBUG) {
			parsed_info.PrintEntityMap();
			parsed_info.PrintOutputList();
			parsed_info.PrintStMap();
			parsed_info.PrintPatternMap();
			parsed_info.PrintWithMap();
		}

		start = high_resolution_clock::now();

		// Toggle optimization switch
		if (OPTIMIZED_EVALUATION) {
			if (PQL_DEBUG) {
				cout << "Using optimized evaluator" << endl;
			}

			result = PQLOptimizedEvaluator().Evaluate(parsed_info);
		}
		else {
			if (PQL_DEBUG) {
				cout << "Using basic evaluator" << endl;
			}

			result = PQLEvaluator().Evaluate(parsed_info);
		}

		stop = high_resolution_clock::now();
		duration = duration_cast<milliseconds>(stop - start);
		cout << "Time taken for PQLEvaluator = " << duration.count() << "ms" << endl;

		// Clear PKB's cache
		PKB().ClearCache();
	}

	start = high_resolution_clock::now();

	final_result = projector.Project(result);

	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);
	cout << "Time taken for PQLProjector = " << duration.count() << "ms" << endl;

	return final_result;
}