#include <CustomTypes.h>
#include <PQLCustomTypes.h>
#include <QueryInfo.h>
#include <QueryResult.h>
#include <PQLScoreRanking.h>

class PQLOptimizedEvaluator {
	typedef priority_queue<STRING_LIST, STRINGLIST_LIST, PQLScoreRanking> FUNCTION_QUEUE;
	typedef unordered_map<STRING_SET*, FUNCTION_QUEUE> STRINGSET_FUNCTIONQUEUE_MAP;
public:
	QueryResult Evaluate(QueryInfo query_info);
private:
	STRINGLIST_STRINGLISTSET_MAP AddResult(STRING_LIST key, STRINGLIST_SET value, STRINGLIST_STRINGLISTSET_MAP results_map, bool is_loop = false);
	QueryResult SetResult(bool is_boolean_output, string bool_result, STRINGLIST_SET result);
		
	bool ParseClauses(QueryInfo query_info, STRINGSET_STRINGLISTSET_MAP* synonyms_map, STRING_INTEGER_MAP* occurrence_count);
	bool EvaluateNoSynonymSet(string f_call, string param1, string param2);
	STRINGLIST_SET EvaluateWithClause(STRING_STRING_MAP entity_map, string lhs, string rhs, STRINGLIST_SET lhs_result = STRINGLIST_SET(), STRINGLIST_SET rhs_result = STRINGLIST_SET());
	STRINGLIST_SET EvaluatePatternCall(string f_call, string param1, string param2, string type);
	STRINGLIST_SET EvaluatePatternCall(string f_call, string param, string type);
	STRINGLIST_SET EvaluateOneSynonymSet(string f_call, string param, string entity_type);
	STRINGLIST_SET EvaluateInverseOneSynonymSet(string f_call, string param, string entity_type);
	STRINGLIST_SET EvaluateTwoSynonymSet(string output_var_type, string param1_entity_type, string param2_entity_type, bool is_same_synonyms = false);
	STRINGLIST_SET EvaluateAllCall(string output_var_type);

	STRING_SET ConvertSet(STRINGLIST_SET result_set);
	STRINGLIST_SET ConvertSet(INTEGER_SET result_set);
	STRINGLIST_SET ConvertSet(STRING_SET result_set);
	STRINGLIST_SET ConvertSet(STMT_STMT_PAIR_LIST result_set);
	STRINGLIST_SET ConvertSet(STMT_VAR_PAIR_LIST result_set);
	STRINGLIST_SET ConvertSet(PROC_VAR_PAIR_LIST result_set);
	STRINGLIST_SET ConvertSet(PROC_PROC_PAIR_LIST result_set);

	int GetSimilarClause(string to_be_added_function, string existing_function);
	INTEGER_LIST GetOutputSynonyms(STRING_LIST synonyms, STRING_LIST output_list);
	STRING_SET GetIntersectResult(STRING_SET val1, STRING_SET val2);
	STRING_SET GetIntersectResult(STRING_SET val1, STRINGLIST_SET val2, int pos_to_check);
	STRINGLIST_SET GetRelatedResult(string synonym, STRINGLIST_STRINGLISTSET_MAP synonyms_map);
	STRING_SET GetNewResult(STRINGLIST_SET value, int pos_to_check);
	STRING_SET* GetRelatedSynonyms(string synonym, STRINGSET_STRINGLISTSET_MAP synonyms_map);
	STRING_SET* GetRelatedSynonyms(STRING_SET synonyms, STRINGSET_STRINGLISTSET_MAP* synonyms_map);
	STRING_LIST* GetRelatedSynonyms(STRING_LIST synonyms, STRINGLIST_STRINGLISTSET_MAP synonyms_map);
	STRING_LIST GetClauseKey(STRING_LIST clause);
	STRINGLIST_SET GetNewResult(STRINGLIST_SET value, INTEGER_LIST pos_to_check);
	STRINGLIST_SET GetCartesianProduct(STRINGLIST_STRINGLISTSET_MAP results_map, STRING_LIST output_list, STRING_STRING_MAP entity_map);
	STRINGLIST_SET GetNoDependencyProduct(STRINGLIST_SET results, STRING_SET values, string type = "", string attr = "");
	STRINGLIST_SET GetDependencyProduct(STRINGLIST_SET results, STRINGLIST_SET values, INTEGER_LIST pos_to_add, INTEGERPAIR_SET to_check, string type = "", string attr = "");
	STRINGLIST_SET GetAlternateResult(string values, string type);
	STRINGLIST_SET GetAlternateResult(STRING_LIST values, int pos_to_check, string type);
	STRINGLIST_SET GetAlternateResult(STRINGLIST_SET values, int pos_to_check, string type);
	STATEMENT_TYPE GetStmtType(string entity_type);

	bool IsVar(string var);
	bool IsString(string var);
	bool IsInteger(string var);
	bool IsUnderscore(string var);
	bool IsOutputSynonyms(STRING_LIST synonyms, STRING_LIST output_list);
	bool IsOutputSynonyms(STRING_SET synonyms, STRING_LIST output_list);
	bool IsConstraintSynonym(STRING_SET synonyms, STRING_SET constraint_synonyms);
	bool IsDuplicate(STRINGLIST_SET set, STRING_LIST value);
	bool IsDependencyStatisfied(STRING_LIST result1, STRING_LIST result2, INTEGERPAIR_SET to_check);
	bool IsSameEntityType(string type, string check);

	int ParsingStmtRef(string param);
	string ParsingEntRef(string param);
	string ParsingSynonym(string synonym);
	string ParsingSynonymAttribute(string synonym);

	void IncreaseOccurrenceCount(string target, STRING_INTEGER_MAP* occurrence_count, int count = 1);
	void RemoveIrrelevant(STRINGLIST_SET* value, STRINGLIST_SET tmp, int pos_to_check);

	void Print(STRING_LIST to_print);
	void Print(STRING_SET to_print);
	void Print(STRINGLIST_SET to_print);
	void Print(STRINGLIST_STRINGLISTSET_MAP to_print);
	void Print(STRINGSET_STRINGLISTSET_MAP to_print);
	void Print(STRINGPAIR_SET to_print);
	void Print(INTEGERPAIR_SET to_print);
	void Print(STRINGSET_FUNCTIONQUEUE_MAP to_print);
	void Print(FUNCTION_QUEUE to_print);
}; 
