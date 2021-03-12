#include "pql/PQLParser.h"
#include "pql/QueryInfo.h"
#include "pql/PQLCustomTypes.h"
#include "catch.hpp"
using namespace std;
#include <map>
#include <iostream>

/*
Returns true if both Query Infos are equivalent, else false
*/

bool compareQueryInfo(QueryInfo actual, QueryInfo expected) {
	bool result = true;
	if (actual.GetOutputList() != expected.GetOutputList()) {
		// if the output variables are different
		cout << "output list not equal" << endl;
		result = false;
		return result;
	}

	if (actual.GetEntityMap() != expected.GetEntityMap()) {
		// cout << "maps are not equal" << endl;
		result = false;
		return result;
	}

	if (actual.GetPatternMap() != expected.GetPatternMap()) {
		result = false;
		return result;
	}

	if (actual.GetStMap() != expected.GetStMap()) {
		result = false;
		return result;
	}

	if (actual.GetWithMap() != expected.GetWithMap()) {
		result = false;
		return result;
	}

	return result;
}


// --------------------- VALID TESTCASES -------------------

// Without clause
// stmt s; Select s

TEST_CASE("Without clause") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s; Select s";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s"] = TYPE_DESIGN_ENTITY_STMT;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// 1st argument constant
// if i1; Select i1 such that Follows* (6, i1)

TEST_CASE("such that clause, Follows*, first argument constant") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "if i1; Select i1 such that Follows* (6, i1)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("i1");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["i1"] = TYPE_DESIGN_ENTITY_IF;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("6");
	st_single_result.push_back("i1");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_FOLLOWS_T] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// 2nd argument constant
// read r1; Select r1 such that Parent(r1, 6)

TEST_CASE("such that clause, Parent, second argument constant") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "read r1; Select r1 such that Parent(r1, 6)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("r1");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["r1"] = TYPE_DESIGN_ENTITY_READ;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("r1");
	st_single_result.push_back("6");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_PARENT] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// 1 such that and 2 unknown synonyms
// stmt s1; if i1; Select s1 such that Follows (s1, i1)

TEST_CASE("such that clause, Follows, both unknown synonyms") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s1; if i1; Select s1 such that Follows (s1, i1)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s1");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s1"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["i1"] = TYPE_DESIGN_ENTITY_IF;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("s1");
	st_single_result.push_back("i1");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_FOLLOWS] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for ModifiesS
// call c; variable v; Select s1 such that Modifies (c, v)

TEST_CASE("such that clause, testing for ModifiesS") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "call c; variable v; Select c such that Modifies (c, v)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("c");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["c"] = TYPE_DESIGN_ENTITY_CALL;
	expected_entity_map["v"] = TYPE_DESIGN_ENTITY_VARIABLE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("c");
	st_single_result.push_back("v");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_MODIFIES_S] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for ModifiesP
// procedure p; Select p such that Modifies (p, "x")

TEST_CASE("such that clause, testing for ModifiesP") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "procedure p; Select p such that Modifies (p, \"x\")";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("p");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["p"] = TYPE_DESIGN_ENTITY_PROCEDURE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("p");
	st_single_result.push_back("\"x\"");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_MODIFIES_P] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for UsesS
// print p; Select p such that Uses (p, “y”)

TEST_CASE("such that clause, testing for UsesS") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "print p; Select p such that Uses (p, \"y\")";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("p");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["p"] = TYPE_DESIGN_ENTITY_PRINT;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("p");
	st_single_result.push_back("\"y\"");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_USES_S] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for UsesS
// variable v; Select v such that Uses ("x", "y")

TEST_CASE("such that clause, testing for UsesP") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "variable v; Select v such that Uses(\"x\", \"y\")";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("v");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["v"] = TYPE_DESIGN_ENTITY_VARIABLE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("\"x\"");
	st_single_result.push_back("\"y\"");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_USES_P] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for Next*
// stmt s; prog_line n; Select s such that Next* (s, n)

TEST_CASE("such that clause, testing for Next*") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s; prog_line n; Select s such that Next* (s, n)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["n"] = TYPE_DESIGN_ENTITY_PROG_LINE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("s");
	st_single_result.push_back("n");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_NEXT_T] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for Next*
// stmt s1, s2; Select s1 such that Affects (s1, s2)

TEST_CASE("such that clause, testing for Affects") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s1, s2; Select s1 such that Affects (s1, s2)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s1");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s1"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["s2"] = TYPE_DESIGN_ENTITY_STMT;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("s1");
	st_single_result.push_back("s2");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_AFFECTS] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for Calls
// procedure p; Select p such that Calls (_, _)

TEST_CASE("such that clause, testing for Calls both empty") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "procedure p; Select p such that Calls (_, _)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("p");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["p"] = TYPE_DESIGN_ENTITY_PROCEDURE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("_");
	st_single_result.push_back("_");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_CALLS] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Test for attrRef in result clause
// assign a; Select a.stmt# such that Follows (a, 10)

TEST_CASE("attrRef in result clause. a.stmt#") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a; Select a.stmt# such that Follows (a, 10)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("a.stmt#");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["a"] = TYPE_DESIGN_ENTITY_ASSIGN;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("a");
	st_single_result.push_back("10");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_FOLLOWS] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Test for attrRef in result clause
// variable v; Select v.varName such that Uses (v, "z")

TEST_CASE("attrRef in result clause. v.varName") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "variable v; Select v.varName such that Uses (v, \"z\")";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("v.varName");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["v"] = TYPE_DESIGN_ENTITY_VARIABLE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("v");
	st_single_result.push_back("\"z\"");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_USES_P] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Tuple in result clause :
// stmt s, s2, s3; variable v; Select <  s , s2 >  such that Follows (s, s2)

TEST_CASE("one tuple") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s, s2; variable v; Select <  s , s2 >  such that Follows (s, s2)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s");
	expected_output_list.push_back("s2");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["s2"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["v"] = TYPE_DESIGN_ENTITY_VARIABLE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("s");
	st_single_result.push_back("s2");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_FOLLOWS] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Tuple with attrRef
// stmt s; variable v; Select <s.stmt#, v.varName> such that Uses(s, v)

TEST_CASE("one tuple with attr ref") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s; variable v; Select <s.stmt#, v.varName> such that Uses(s, v)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s.stmt#");
	expected_output_list.push_back("v.varName");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["v"] = TYPE_DESIGN_ENTITY_VARIABLE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("s");
	st_single_result.push_back("v");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_USES_S] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}


// Test for MULTIPLE CLAUSES
// Big tuple, Multiple clauses

// Tuple in result clause :
// stmt s, s2; assign a, a2, a3; 
// Select <s, s2, a, a2, a3> such that Follows (s, 10) and Follows* (5, s2) and Parent*( a, a2) such that Parent(2, a3)

TEST_CASE("big tuple, multiple clauses, similar relref and different relref") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s, s2; assign a, a2, a3; Select <s, s2, a, a2, a3> such that Follows (s, 10) and Follows(5, s2) and Parent*( a, a2) such that Parent(2, a3)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s");
	expected_output_list.push_back("s2");
	expected_output_list.push_back("a");
	expected_output_list.push_back("a2");
	expected_output_list.push_back("a3");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["s2"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["a"] = TYPE_DESIGN_ENTITY_ASSIGN;
	expected_entity_map["a2"] = TYPE_DESIGN_ENTITY_ASSIGN;
	expected_entity_map["a3"] = TYPE_DESIGN_ENTITY_ASSIGN;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result_one;
	STRING_LIST st_single_result_one;
	st_single_result_one.push_back("s");
	st_single_result_one.push_back("10");

	STRING_LIST st_single_result_two;
	st_single_result_two.push_back("5");
	st_single_result_two.push_back("s2");

	STRINGLIST_LIST st_result_two;
	STRING_LIST st_single_result_three;
	st_single_result_three.push_back("a");
	st_single_result_three.push_back("a2");

	STRINGLIST_LIST st_result_three;
	STRING_LIST st_single_result_four;
	st_single_result_four.push_back("2");
	st_single_result_four.push_back("a3");


	st_result_one.push_back(st_single_result_one);
	st_result_one.push_back(st_single_result_two);
	
	st_result_two.push_back(st_single_result_three);
	st_result_three.push_back(st_single_result_four);

	expected_st_map[TYPE_COND_FOLLOWS] = st_result_one;
	expected_st_map[TYPE_COND_PARENT_T] = st_result_two;
	expected_st_map[TYPE_COND_PARENT] = st_result_three;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Test for pattern clauses :

TEST_CASE("simple pattern clause, assign type, full pattern") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a; Select a pattern a (_ , \"x + y\")";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("a");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["a"] = TYPE_DESIGN_ENTITY_ASSIGN;

	STRING_STRINGLISTLIST_MAP expected_pattern_map;
	STRINGLIST_LIST pattern_result;
	STRING_LIST pattern_single_result;
	pattern_single_result.push_back("_");
	pattern_single_result.push_back("\"x + y\"");
	pattern_single_result.push_back("a");
	pattern_result.push_back(pattern_single_result);
	
	expected_pattern_map[TYPE_COND_PATTERN_F] = pattern_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetPatternMap(expected_pattern_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

TEST_CASE("simple pattern clause, assign type, partial pattern, white some whitespace") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a; Select a pattern a (_ , _\"   x + y    \"_)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("a");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["a"] = TYPE_DESIGN_ENTITY_ASSIGN;

	STRING_STRINGLISTLIST_MAP expected_pattern_map;
	STRINGLIST_LIST pattern_result;
	STRING_LIST pattern_single_result;
	pattern_single_result.push_back("_");
	pattern_single_result.push_back("\"x + y\"");
	pattern_single_result.push_back("a");
	pattern_result.push_back(pattern_single_result);

	expected_pattern_map[TYPE_COND_PATTERN_P] = pattern_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetPatternMap(expected_pattern_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

TEST_CASE("simple pattern clause, while type") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "while w; Select w pattern w ( \"x\" , _)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("w");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["w"] = TYPE_DESIGN_ENTITY_WHILE;

	STRING_STRINGLISTLIST_MAP expected_pattern_map;
	STRINGLIST_LIST pattern_result;
	STRING_LIST pattern_single_result;
	pattern_single_result.push_back("\"x\"");
	pattern_single_result.push_back("_");
	pattern_single_result.push_back("w");
	pattern_result.push_back(pattern_single_result);

	expected_pattern_map[TYPE_COND_PATTERN_F] = pattern_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetPatternMap(expected_pattern_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

TEST_CASE("simple pattern clause, if type") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "if ifs; Select ifs pattern ifs ( \"z\" , _,_)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("ifs");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["ifs"] = TYPE_DESIGN_ENTITY_IF;

	STRING_STRINGLISTLIST_MAP expected_pattern_map;
	STRINGLIST_LIST pattern_result;
	STRING_LIST pattern_single_result;
	pattern_single_result.push_back("\"z\"");
	pattern_single_result.push_back("_");
	pattern_single_result.push_back("_");
	pattern_single_result.push_back("ifs");
	pattern_result.push_back(pattern_single_result);

	expected_pattern_map[TYPE_COND_PATTERN_F] = pattern_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetPatternMap(expected_pattern_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// multiple pattern clauses with tuple
TEST_CASE("multiple pattern clauses with tuple") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "if ifs; assign a, a1; variable v; Select <ifs, a, a1> pattern ifs ( \"z\" , _,_) and a1 ( v, \"  haha  \") and a ( \"x\" , _\"a/b/c+z\"_)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("ifs");
	expected_output_list.push_back("a");
	expected_output_list.push_back("a1");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["ifs"] = TYPE_DESIGN_ENTITY_IF;
	expected_entity_map["a"] = TYPE_DESIGN_ENTITY_ASSIGN;
	expected_entity_map["a1"] = TYPE_DESIGN_ENTITY_ASSIGN;
	expected_entity_map["v"] = TYPE_DESIGN_ENTITY_VARIABLE;

	STRING_STRINGLISTLIST_MAP expected_pattern_map;
	STRINGLIST_LIST pattern_result;
	STRING_LIST pattern_single_result;
	pattern_single_result.push_back("\"z\"");
	pattern_single_result.push_back("_");
	pattern_single_result.push_back("_");
	pattern_single_result.push_back("ifs");
	pattern_result.push_back(pattern_single_result);

	STRING_LIST pattern_single_result_two;
	pattern_single_result_two.push_back("v");
	pattern_single_result_two.push_back("\"haha\"");
	pattern_single_result_two.push_back("a1");
	pattern_result.push_back(pattern_single_result_two);

	expected_pattern_map[TYPE_COND_PATTERN_F] = pattern_result;

	STRINGLIST_LIST pattern_result_two;
	STRING_LIST pattern_single_result_three;
	pattern_single_result_three.push_back("\"x\"");
	pattern_single_result_three.push_back("\"a/b/c+z\"");
	pattern_single_result_three.push_back("a");

	pattern_result_two.push_back(pattern_single_result_three);

	expected_pattern_map[TYPE_COND_PATTERN_P] = pattern_result_two;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetPatternMap(expected_pattern_map);

	// cout << "------ACTUAL----" << endl;
	// query_info_actual.PrintPatternMap();
	// cout << "-----expected----" << endl;
	// query_info_expected.PrintPatternMap();

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// with clauses 
// procedure p; variable v;
// Select p with p.procName = v.varName
TEST_CASE("simple with clause") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "procedure p; variable v; Select p with p.procName = v.varName";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("p");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["p"] = TYPE_DESIGN_ENTITY_PROCEDURE;
	expected_entity_map["v"] = TYPE_DESIGN_ENTITY_VARIABLE;

	STRING_STRINGLISTLIST_MAP expected_pattern_map;
	
	STRINGPAIR_SET expected_with_map;
	STRING_PAIR* single_with_result = new STRING_PAIR();
	single_with_result->first = "p.procName";
	single_with_result->second = "v.varName";
	expected_with_map.insert(single_with_result);

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetWithMap(expected_with_map);

	cout << "ACTUAL WITH MAP---" << endl;
	query_info_actual.PrintWithMap();
	cout << "expected WITH MAP---" << endl;
	query_info_expected.PrintWithMap();

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(true);
}

// invalid query
// variable v, v; Select v (repeated synonym)
TEST_CASE("invalid query, repeated synonym") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "variable v, v; Select v";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
}

// invalid query
// variable v; stmt s; procedure v; Select v
TEST_CASE("invalid query, repeated synonym in different declaration") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "variable v; stmt s; procedure v; Select v";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
}

// invalid query
// variable vp[; Select v
TEST_CASE("invalid query, invalid syntax in declaration") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "variable vp[; Select v";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
}

// invalid query
// invalid design entity declaration
TEST_CASE("invalid query, invalid design entity declaration") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "stmt s, s1; design a2; Select s1";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
}

// invalid query
// stmt s, s1; Select s2
TEST_CASE("invalid query, invalid semantics in result-cl, valid syntax") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "stmt s, s1; Select s2";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
	// REQUIRE(query_info_actual.IsBooleanOutputFalse());
}

// invalid query
// stmt s, s1; Select s1 such that Follows (s2, 10)
TEST_CASE("invalid query, invalid semantics in relref arg, valid syntax") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "stmt s, s1; Select s1 such that Follows (s2, 10)";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
	REQUIRE(query_info_actual.IsSemanticsInvalid());
}

// invalid query
// stmt s, s1; Select s1 such that Uses (s[]2, "x")
TEST_CASE("invalid query, invalid syntax in relref arg") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "stmt s, s1; Select s1 such that Follows (s[]2, \"x\")";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
	REQUIRE(!query_info_actual.IsSemanticsInvalid());
}

// invalid query
// stmt s, s1; Select s1 such that Uses (s[]2, "x")
TEST_CASE("invalid query, extra whitespace inbetween such that") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "stmt s, s1; Select s1 such  that Follows (1, 2)";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
}

// invalid query
// stmt s, s1; Select s1 such that Uses (s[]2, "x")
TEST_CASE("invalid query, invalid clause type") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "stmt s, s1; Select s1 SuchyThatz Follows (1, 2)";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
}

// invalid query
// underscore found in first arg of Uses
TEST_CASE("invalid query, underscore in first arg of Uses") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "stmt s, s1; Select s1 such that Uses (_, \"x\")";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
}

// invalid query
// stmt s, s1; Select s1 such that Uses (s[]2, "x")
TEST_CASE("invalid query, invalid pattern first arg") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "stmt s, s1; Select s1 SuchyThatz Follows (1, 2)";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
}

// invalid query
// missing var after pattern
TEST_CASE("invalid query, missing var after pattern") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "assign a; Select a pattern (_, \"x+y\")";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
}

// invalid query
// missing var after pattern
TEST_CASE("invalid query, stmt ref inside first arg of pattern") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "assign a; stmt s; Select a pattern a (s, \"x\")";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
}

// invalid query
// missing var after pattern
TEST_CASE("invalid query, extra and/pattern inbetween clauses") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "assign a; stmt s; Select a pattern a (_, \"x\") and pattern a (_, \"y\")";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
}

// invalid query
// consecutive num in expr-spec
TEST_CASE("invalid query, consecutive num in expr-spec") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "assign a; Select a pattern a (_, \"10 10\")";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
}

// invalid query
TEST_CASE("invalid query, invalid semantics, undeclared relref arg valid syntax") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "assign a; Select BOOLEAN such that Calls ( s, \"procA\")";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
	REQUIRE(query_info_actual.IsSemanticsInvalid());
}

// invalid query
TEST_CASE("invalid query, invalid semantics, and then found invalid syntax") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "assign a; Select BOOLEAN such that Calls ( s, \"procA\") and Calls ( \"procA\", \"[!]\")";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
	REQUIRE(!query_info_actual.IsSemanticsInvalid());
}

// invalid query
TEST_CASE("invalid query, invalid semantics, wrong attribute type, valid syntax") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "assign a; Select BOOLEAN with a.varName = \"x\"";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
	REQUIRE(query_info_actual.IsSemanticsInvalid());
}

// invalid query
TEST_CASE("invalid query, wrong case for BOOLEAN") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;

	string pql_query = "Select BOOLEAn";

	query_info_actual = pql_parser.Parse(pql_query);

	REQUIRE(!query_info_actual.IsQueryInfoValid());
}
