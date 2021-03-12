#include <CustomTypes.h>
#include <PQLCustomTypes.h>
#include <QueryInfo.h>
#include <QueryResult.h>
// #include <PQLOptimization.h>

class PQLEvaluator
{
	// typedef priority_queue< STRING_LIST, STRINGLIST_LIST, PQLOptimization> FUNCTION_QUEUE;
	// typedef unordered_map<STRING_SET*, FUNCTION_QUEUE> STRINGSET_FUNCTIONQUEUE_MAP;
public:
	QueryResult Evaluate(QueryInfo query_info);

	void Print(STRING_LIST to_print);
	void Print(STRING_SET to_print);
	void Print(STRINGLIST_SET to_print);
	void Print(STRINGLIST_STRINGLISTSET_MAP to_print);
	void Print(STRINGSET_STRINGLISTSET_MAP to_print);
	void Print(STRINGPAIR_SET to_print);

	void AddResult(STRING_LIST key, STRINGLIST_SET value, STRINGLIST_STRINGLISTSET_MAP* results_map);
	QueryResult SetResult(bool is_boolean_output, string bool_result, STRINGLIST_SET result);
		
	bool ParseClauses(QueryInfo query_info, STRINGPAIR_SET* constraints);
	bool ParseClauses(QueryInfo query_info, STRINGSET_STRINGLISTSET_MAP* synonyms_map);

	STRING_SET* GetRelatedSynonyms(string synonym, STRINGSET_STRINGLISTSET_MAP synonyms_map);
	STRING_SET* GetRelatedSynonyms(STRING_SET synonyms, STRINGSET_STRINGLISTSET_MAP* synonyms_map);
	STRING_LIST* GetRelatedSynonyms(string synonym, STRINGLIST_STRINGLISTSET_MAP synonyms_map);
	STRING_LIST* GetRelatedSynonyms(STRING_LIST synonyms, STRINGLIST_STRINGLISTSET_MAP synonyms_map);

	bool EvaluateConstraints(STRING_STRING_MAP entity_map, STRINGPAIR_SET constraints, STRINGLIST_STRINGLISTSET_MAP* results_map);
	bool EvaluateNoSynonymSet(string f_call, string param1, string param2);
	STRINGLIST_SET EvaluatePatternCall(string f_call, string param1, string param2, string type);
	STRINGLIST_SET EvaluatePatternCall(string f_call, string param, string type);
	STRINGLIST_SET EvaluateOneSynonymSet(string f_call, string param);
	STRINGLIST_SET EvaluateInverseOneSynonymSet(string f_call, string param);
	STRINGLIST_SET EvaluateTwoSynonymSet(string output_var_type);
	STRINGLIST_SET EvaluateAllCall(string output_var_type);

	STRING_SET ConvertSet(STRINGLIST_SET result_set);
	STRINGLIST_SET ConvertSet(INTEGER_SET result_set);
	STRINGLIST_SET ConvertSet(DOUBLE_SET result_set);
	STRINGLIST_SET ConvertSet(STRING_SET result_set);
	STRINGLIST_SET ConvertSet(STMT_STMT_PAIR_LIST result_set);
	STRINGLIST_SET ConvertSet(STMT_VAR_PAIR_LIST result_set);
	STRINGLIST_SET ConvertSet(PROC_VAR_PAIR_LIST result_set);
	STRINGLIST_SET ConvertSet(PROC_PROC_PAIR_LIST result_set);

	//bool CheckConstraints(STRINGPAIR_SET* constraints, STRING_STRING_MAP entity_map, STRINGLIST_STRINGLISTSET_MAP* results_map, STRING_LIST key, STRINGLIST_SET* value);

	bool RemoveIrrelevant(STRINGLIST_SET* value, STRINGLIST_SET tmp, int pos_to_check);
	bool RemoveIrrelevant(STRING_SET* value, STRINGLIST_SET tmp);
	// bool RemoveIrrelevant(STRINGLIST_SET* value, string tmp, int pos_to_check);

	int GetCommonSynonymsIndex(STRING_LIST large_keys, string synonym);
	INTEGERPAIR_SET GetCommonSynonymsIndex(STRING_LIST large_keys, STRING_LIST small_keys);
	STRING_SET GetNewResult(STRINGLIST_SET value, int pos_to_check);
	STRINGLIST_SET GetNewResult(STRINGLIST_SET value, INTEGER_LIST pos_to_check);
	STRINGLIST_SET GetCombinedResult(STRINGLIST_SET large_values, STRINGLIST_SET small_values, INTEGERLIST_LIST indexes);
	STRINGLIST_SET GetCombinedResult(STRINGLIST_SET output_result, STRINGLIST_SET result, int pos_to_compare);
	STRINGLIST_SET GetCartesianProduct(STRINGLIST_STRINGLISTSET_MAP results_map, STRING_LIST output_list, STRING_STRING_MAP entity_map);
	STRINGLIST_SET GetNoDependencyProduct(STRINGLIST_SET results, STRINGLIST_SET values);
	STRINGLIST_SET GetDependencyProduct(STRINGLIST_SET results, STRINGLIST_SET values, int pos_to_add, INTEGERPAIR_SET to_check, string output_type, string output_attr);
	
	STRING_SET GetIntersectResult(STRING_SET val1, STRING_SET val2);
	STRING_SET GetIntersectResult(STRING_SET val1, STRINGLIST_SET val2, int pos_to_check);
	STRINGLIST_SET GetIntersectResult(STRINGLIST_SET val1, STRINGLIST_SET val2);

	STRINGLIST_SET GetAlternateResult(string values, string type);
	STRING_SET GetAlternateResult(STRINGLIST_SET values, string type);
	STRINGLIST_SET GetAlternateOutputResult(STRING_SET values, string type);

	bool IsVar(string var);
	bool IsString(string var);
	bool IsInteger(string var);
	bool IsUnderscore(string var);
	bool IsOutputSynonyms(STRING_LIST synonyms, STRING_LIST output_list);
	bool IsDuplicate(STRINGLIST_SET set, STRING_LIST value);
	bool IsDependencyStatisfied(STRING_LIST result1, STRING_LIST result2, INTEGERPAIR_SET to_check);
	bool IsSameEntityType(string type, string check);

	int ParsingStmtRef(string param);
	string ParsingEntRef(string param);
	string ParsingSynonym(string synonym);
	string ParsingSynonymAttribute(string synonym);
}; 
