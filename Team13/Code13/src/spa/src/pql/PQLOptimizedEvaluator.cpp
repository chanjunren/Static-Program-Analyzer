#include "PQLOptimizedEvaluator.h"
#include <iostream>
#include <chrono> 
#include "pkb/PKB.h"

using namespace std::chrono;

QueryResult PQLOptimizedEvaluator::Evaluate(QueryInfo query_info) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - Evaluate" << endl;
	}

	// timer
	auto start = high_resolution_clock::now();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	// Final result to be returned
	STRINGLIST_SET final_result_set = *(new STRINGLIST_SET());

	// Expected output list
	STRING_LIST output_list = query_info.GetOutputList();
	bool is_boolean_output = query_info.IsBooleanOutput();

	// Unevaluated clauses & constraints
	STRINGSET_FUNCTIONQUEUE_MAP synonyms_map = STRINGSET_FUNCTIONQUEUE_MAP();

	// Occurrence count - optimization
	STRING_INTEGER_MAP occurrence_count = STRING_INTEGER_MAP();

	// Intermediate results from evaluated clauses
	STRINGLIST_STRINGLISTSET_MAP results_map = STRINGLIST_STRINGLISTSET_MAP();
	STRINGLIST_STRINGLISTSET_MAP final_results_map = STRINGLIST_STRINGLISTSET_MAP();

	// Query Info object
	STRING_STRING_MAP entity_map = query_info.GetEntityMap();

	// Parse clauses conditions	
	start = high_resolution_clock::now();
	STRINGSET_STRINGLISTSET_MAP tmp_map = STRINGSET_STRINGLISTSET_MAP();
	if (!ParseClauses(query_info, &tmp_map, &occurrence_count)) {
		if (PQL_DEBUG) {
			cout << "PQLOptimizedEvaluator - parsing such that and pattern clause: failed." << endl;
		}

		return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
	}

	if (PQL_DEBUG_PRINTING) {
		cout << "synonym map after parsing clause" << endl;
		Print(tmp_map);
	}

	// Set priority for clauses
	for (auto f = tmp_map.cbegin(); f != tmp_map.cend(); f++) {
		FUNCTION_QUEUE priority = FUNCTION_QUEUE(occurrence_count);
		STRINGLIST_SET clauses = (*f).second;

		for (auto c = clauses.cbegin(); c != clauses.cend(); c++) {
			priority.push(**c);
		}

		synonyms_map.insert({ new STRING_SET(*(*f).first), priority });
	}

	if (PQL_DEBUG) {
		cout << "synonym map aft setting priority" << endl;
		Print(synonyms_map);
	}
	stop = high_resolution_clock::now();
	if (DEBUG_TIMER) {
		duration = duration_cast<milliseconds>(stop - start);
		cout << "Time taken for parsing clauses with priority = " << duration.count() << "ms" << endl;
	}

	// Evaluate synonyms_map: 1 related-synonyms group at a time
	for (auto f = synonyms_map.cbegin(); f != synonyms_map.cend(); f++) {
		STRINGLIST_STRINGLISTSET_MAP tmp_map = results_map;
		FUNCTION_QUEUE clauses = (*f).second;
		while (!clauses.empty()) {
			STRING_LIST c = clauses.top();
			clauses.pop();

			string f_call = c.at(0);
			string raw_param1 = c.at(1);
			string raw_param2 = c.at(2);

			STRING_LIST c_key = GetClauseKey(c);
			STRINGLIST_SET c_value = STRINGLIST_SET();

			start = high_resolution_clock::now();
			if (f_call.compare(TYPE_WITH_CLAUSE) == 0) {
				if (c_key.size() == 2) {
					c_value = EvaluateWithClause(entity_map, raw_param1, raw_param2, GetRelatedResult(c_key.at(0), tmp_map), GetRelatedResult(c_key.at(1), tmp_map));
				}
				else {
					c_value = EvaluateWithClause(entity_map, raw_param1, raw_param2, GetRelatedResult(c_key.at(0), tmp_map));
				}
			}
			else if (f_call.compare(TYPE_COND_PATTERN_F) == 0 || f_call.compare(TYPE_COND_PATTERN_P) == 0) {
				string pattern_type = entity_map.at(c_key.at(0));
				if (c_key.size() == 2) {
					// first param = synonym
					STRING_SET param1_result = ConvertSet(GetRelatedResult(c_key.at(1), tmp_map));

					if (param1_result.empty()) {
						c_value = EvaluatePatternCall(f_call, raw_param2, pattern_type);
					}
					else {

						for (string s : param1_result) {
							STRING_SET result = ConvertSet(EvaluatePatternCall(f_call, s, raw_param2, pattern_type));


							for (string new_r : result) {
								STRING_LIST tmp = STRING_LIST();
								tmp.push_back(new_r);
								tmp.push_back(s);

								if (!IsDuplicate(c_value, tmp)) {
									c_value.insert(new STRING_LIST(tmp));
								}
							}
						}
					}
				}
				else {
					// first param != synonym
					c_value = EvaluatePatternCall(f_call, raw_param1, raw_param2, pattern_type);
				}
			}
			else {
				if (raw_param1.compare(raw_param2) == 0) {
					// both params = synonyms - same

					STRING_SET param_result = ConvertSet(GetRelatedResult(c_key.at(0), tmp_map));
					string entity_type = entity_map.at(raw_param1);

					if (param_result.empty()) {
						c_value = EvaluateTwoSynonymSet(f_call, entity_type, entity_type, true);
					}
					else {
						STRING_SET tmp = STRING_SET();
						for (string s : param_result) {
							bool result = EvaluateNoSynonymSet(f_call, s, s);
							if (result) {
								tmp.insert(s);
							}
						}

						if (!tmp.empty()) {
							c_value = ConvertSet(tmp);
						}
					}
				}
				else if (c_key.size() == 2) {
					// both params = synonyms - different
					STRINGLIST_SET param1_result = GetRelatedResult(c_key.at(0), tmp_map);
					STRINGLIST_SET param2_result = GetRelatedResult(c_key.at(1), tmp_map);
					string param1_entity_type = entity_map.at(raw_param1);
					string param2_entity_type = entity_map.at(raw_param2);

					c_value = EvaluateTwoSynonymSet(f_call, param1_entity_type, param2_entity_type);
					cout << "bef size: " << c_value.size() << endl;
					if (!param1_result.empty()) {
						RemoveIrrelevant(&c_value, param1_result, 0);
					}

					if (!param2_result.empty()) {
						RemoveIrrelevant(&c_value, param2_result, 1);
					}

					cout << "aft size: " << c_value.size() << endl;
				}
				else if (!IsVar(raw_param1) && IsVar(raw_param2)) {
					// first param != synonym & second param = synonym
					// GetXXX() call

					string entity_type = entity_map.at(c_key.at(0));
					STRING_SET param_result = ConvertSet(GetRelatedResult(c_key.at(0), tmp_map));

					if (param_result.empty()) {
						c_value = EvaluateOneSynonymSet(f_call, raw_param1, entity_type);
					}
					else {
						for (string s : param_result) {
							STRING_LIST tmp = STRING_LIST();
							bool result = EvaluateNoSynonymSet(f_call, raw_param1, s);
							if (result) {
								tmp.push_back(s);
								c_value.insert(new STRING_LIST(tmp));
							}
						}
					}
				}
				else {
					// first param = synonym & second param != synonym
					// GetInverseXXX() call

					string entity_type = entity_map.at(c_key.at(0));
					STRING_SET param_result = ConvertSet(GetRelatedResult(c_key.at(0), tmp_map));

					if (param_result.empty()) {
						c_value = EvaluateInverseOneSynonymSet(f_call, raw_param2, entity_type);
					}
					else {
						for (string s : param_result) {
							STRING_LIST tmp = STRING_LIST();
							bool result = EvaluateNoSynonymSet(f_call, s, raw_param2);
							if (result) {
								tmp.push_back(s);
								c_value.insert(new STRING_LIST(tmp));
							}
						}
					}
				}
			}
			stop = high_resolution_clock::now();
			if (DEBUG_TIMER) {
				duration = duration_cast<milliseconds>(stop - start);
				cout << "Time taken for 1 clause = " << duration.count() << "ms" << endl;
			}

			if (c_value.empty()) {
				// error
				if (PQL_DEBUG) {
					cout << "PQLOptimizedEvaluator - Evaluating synonyms_map: Empty results." << endl;
				}

				return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
			}

			if (PQL_DEBUG_PRINTING) {
				cout << "Results after evaluating 1 clause" << endl;
				cout << "KEY " << endl;
				Print(c_key);
				cout << "VALUE " << endl;
				Print(c_value);
			}

			// Add into the tmp_map
			start = high_resolution_clock::now();
			STRING_LIST* check_key = GetRelatedSynonyms(c_key, tmp_map);
			bool is_statisfied = false;
			if (check_key != nullptr) {
				INTEGERPAIR_SET total_p = INTEGERPAIR_SET();
				STRINGLIST_SET common_tmp = tmp_map.at(check_key);
				for (int check_idx = 0; check_idx < check_key->size(); check_idx++) {
					for (int c_idx = 0; c_idx < c_key.size(); c_idx++) {
						if (check_key->at(check_idx) == c_key.at(c_idx)) {
							INTEGER_PAIR p = INTEGER_PAIR();
							p.first = check_idx;
							p.second = c_idx;

							total_p.insert(new INTEGER_PAIR(p));
						}
					}
				}
				
				for (STRING_LIST* check_result : common_tmp) {
					for (STRING_LIST* c_result : c_value) {
						if (IsDependencyStatisfied(*check_result, *c_result, total_p)) {
							is_statisfied = true;
							break;
						}
					}

					if (is_statisfied) {
						break;
					}
				}

				if (!is_statisfied) {
					return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
				}
			}

			tmp_map.insert({ new STRING_LIST(c_key), c_value });

			stop = high_resolution_clock::now();
			if (DEBUG_TIMER) {
				duration = duration_cast<milliseconds>(stop - start);
				cout << "Time taken for adding result at tmp_map = " << duration.count() << "ms" << endl;
			}
		}

		if (PQL_DEBUG_PRINTING) {
			cout << "Results after evaluating 1 group of clauses" << endl;
			Print(tmp_map);
		}

		// Check if any of the synonyms is in expected output list: "TRUE" -> set tmp_map as results_map; "FALSE" -> continue
		STRING_SET synonyms = *(*f).first;
		if (IsOutputSynonyms(synonyms, output_list)) {
			for (auto kv : tmp_map) {
				results_map = AddResult(*kv.first, kv.second, results_map);
			}

			if (results_map.empty()) {
				return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
			}
			// results_map = tmp_map;
		}

		if (PQL_DEBUG_PRINTING) {
			cout << "Results map after inserting 1 group's results" << endl;
			Print(results_map);
		}
	}
	stop = high_resolution_clock::now();
	if (DEBUG_TIMER) {
		duration = duration_cast<milliseconds>(stop - start);
		cout << "Time taken for evaluating clauses = " << duration.count() << "ms" << endl;
	}

	if (PQL_DEBUG_PRINTING) {
		cout << "Results map after evaluating clauses" << endl;
		Print(results_map);
	}

	if (!is_boolean_output) {
		start = high_resolution_clock::now();

		// Get output_list's results from results_map
		INTEGER_SET retrieved_output_index = INTEGER_SET();

		for (int o = 0; o < output_list.size(); o++) {
			string parsed_output_synonym = ParsingSynonym(output_list.at(o));
			string output_synonym_attr = ParsingSynonymAttribute(output_list.at(o));

			for (auto result_entry = results_map.cbegin(); result_entry != results_map.cend(); result_entry++) {
				STRING_LIST* result_key = (*result_entry).first;
				bool is_found = false;
				for (int i = 0; i < result_key->size(); i++) {
					if (result_key->at(i).compare(parsed_output_synonym) == 0) {
						// found output synonym
						is_found = true;

						// Add index of retrieved output synonym to list
						retrieved_output_index.insert(o);
					}
				}

				if (is_found) {
					// Has >=1 expected output synonym(s)
					auto inner_start = high_resolution_clock::now();
					final_results_map = AddResult(*result_key, (*result_entry).second, final_results_map, true);
					if (final_results_map.empty()) {
						return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
					}

					auto inner_stop = high_resolution_clock::now();
					if (DEBUG_TIMER) {
						duration = duration_cast<milliseconds>(inner_stop - inner_start);
						cout << "Time taken for adding result at final_results_map = " << duration.count() << "ms" << endl;
					}
				}
			}
		}

		if (PQL_DEBUG_PRINTING) {
			cout << "Final results map after extracting dependent output synonyms " << endl;
			Print(final_results_map);
		}

		// Get all output_list's results that are not in results_map
		for (int i = 0; i < output_list.size(); i++) {
			if (retrieved_output_index.find(i) == retrieved_output_index.end()) {
				string synonym = output_list.at(i);
				string parsed_synonym = ParsingSynonym(synonym);
				string synonym_type = ParsingSynonymAttribute(synonym);

				STRING_LIST key = *(new STRING_LIST());
				key.push_back(parsed_synonym);
				STRINGLIST_SET value = *(new STRINGLIST_SET());

				STRING_LIST* check_key = GetRelatedSynonyms(key, final_results_map);
				if (check_key == nullptr) {
					// not duplicate
					value = EvaluateAllCall(entity_map.at(parsed_synonym));

					if (value.empty()) {
						// error
						if (PQL_DEBUG) {
							cout << "PQLOptimizedEvaluator - Extracting independent output synonyms: Empty results." << endl;
						}

						return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
					}
				}
				else {
					// duplicate
					STRING_LIST new_key = STRING_LIST();
					new_key.insert(new_key.end(), check_key->begin(), check_key->end());
					new_key.insert(new_key.end(), key.begin(), key.end());
					key = new_key;

					value = final_results_map.at(check_key);
					for (auto v = value.cbegin(); v != value.cend(); v++) {
						(*v)->push_back((*v)->at(0));
					}

					final_results_map.erase(check_key);
				}
				final_results_map.insert({ new STRING_LIST(key), value });
			}
		}

		if (PQL_DEBUG_PRINTING) {
			cout << "Final results map after extracting independent output synonyms " << endl;
			Print(final_results_map);
		}

		// Combine all the expected output results
		final_result_set = GetCartesianProduct(final_results_map, output_list, entity_map);

		// Convert to attribute if needed
		for (int i = 0; i < output_list.size(); i++) {
			string synonym = output_list.at(i);
			string synonym_type = entity_map.at(ParsingSynonym(synonym));
			string synonym_attr = ParsingSynonymAttribute(synonym);
			if (!IsSameEntityType(synonym_type, synonym_attr)) {
				final_result_set = GetAlternateResult(final_result_set, i, synonym_type);
			}
		}

		if (PQL_DEBUG) {
			cout << "final result set" << endl;
			Print(final_result_set);
		}

		stop = high_resolution_clock::now();
		if (DEBUG_TIMER) {
			duration = duration_cast<milliseconds>(stop - start);
			cout << "Time taken for extracting output synonyms = " << duration.count() << "ms" << endl;
		}
	}

	return SetResult(is_boolean_output, "TRUE", final_result_set);
}

void PQLOptimizedEvaluator::Print(STRINGPAIR_SET to_print) {
	for (STRING_PAIR* p : to_print) {
		cout << "<" << p->first << "=" << p->second << "> ";
	}
	cout << endl;
}

void PQLOptimizedEvaluator::Print(INTEGERPAIR_SET to_print) {
	for (INTEGER_PAIR* p : to_print) {
		cout << "<" << p->first << "=" << p->second << "> ";
	}
	cout << endl;
}

void PQLOptimizedEvaluator::Print(STRINGLIST_STRINGLISTSET_MAP to_print) {
	for (auto result_entry = to_print.cbegin(); result_entry != to_print.cend(); result_entry++) {
		for (string s : *(*result_entry).first) {
			cout << s << " ";
		}
		cout << ": ";;
		Print((*result_entry).second);
	}
}

void PQLOptimizedEvaluator::Print(STRINGSET_STRINGLISTSET_MAP to_print) {
	for (auto result_entry = to_print.cbegin(); result_entry != to_print.cend(); result_entry++) {
		Print(*(*result_entry).first);
		cout << ": ";
		Print((*result_entry).second);
	}
}

void PQLOptimizedEvaluator::Print(STRINGSET_FUNCTIONQUEUE_MAP to_print) {
	for (auto result_entry = to_print.cbegin(); result_entry != to_print.cend(); result_entry++) {
		Print(*(*result_entry).first);
		cout << ": ";
		Print((*result_entry).second);
	}
}

void PQLOptimizedEvaluator::Print(FUNCTION_QUEUE to_print) {
	while (!to_print.empty()) {
		STRING_LIST values = to_print.top();
		to_print.pop();
		Print(values);
	}
	cout << endl;
}

void PQLOptimizedEvaluator::Print(STRING_SET to_print) {
	for (auto f = to_print.cbegin(); f != to_print.cend(); f++) {
		cout << (*f) << " ";
	}
	cout << endl;
}

void PQLOptimizedEvaluator::Print(STRING_LIST to_print) {
	cout << "{ ";
	for (string s : to_print) {
		cout << s << " ";
	}
	cout << "} ";
}

void PQLOptimizedEvaluator::Print(STRINGLIST_SET to_print) {
	for (STRING_LIST* set : to_print) {
		Print(*set);
	}
	cout << endl;
}

STRINGLIST_STRINGLISTSET_MAP PQLOptimizedEvaluator::AddResult(STRING_LIST key, STRINGLIST_SET value, STRINGLIST_STRINGLISTSET_MAP results_map, bool is_loop) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - AddResult" << endl;
	}

	if (PQL_DEBUG_PRINTING) {
		Print(key);
		Print(value);
	}

	STRING_LIST* check_key = GetRelatedSynonyms(key, results_map);
	if (check_key == nullptr) {
		results_map.insert({ new STRING_LIST(key), value });
	}
	else {
		STRINGLIST_SET tmp_result = results_map.at(check_key);
		STRING_LIST new_key = *(new STRING_LIST(*check_key));

		// Merge common synonyms & their results
		INTEGERPAIR_SET index_to_compare = INTEGERPAIR_SET();
		INTEGER_LIST pos_to_add = INTEGER_LIST();
		bool has_common = false;

		for (int a = 0; a < key.size(); a++) {
			bool is_unique = true;
			for (int s = 0; s < check_key->size(); s++) {
				if (check_key->at(s).compare(key.at(a)) == 0) {
					// common synonym
					INTEGER_PAIR* pair = new INTEGER_PAIR();
					pair->first = s;
					pair->second = a;

					index_to_compare.insert(pair);

					// toggle to true
					has_common = true;
					is_unique = false;
				}
			}

			if (is_unique) {
				pos_to_add.push_back(a);
			}
		}

		if (PQL_DEBUG_PRINTING) {
			cout << "check key: ";
			Print(*check_key);
			cout << "tmp result: ";
			Print(tmp_result);
			cout << "key: ";
			Print(key);
			cout << "value: ";
			Print(value);
			cout << endl;
		}

		if (has_common) {
			tmp_result = GetDependencyProduct(tmp_result, value, pos_to_add, index_to_compare);
		}

		if (tmp_result.empty()) {
			if (PQL_DEBUG) {
				cout << "PQLOptimizedEvaluator: AddResult: Empty result." << endl;
			}

			results_map.clear();
		}
		else {
			for (int i : pos_to_add) {
				new_key.push_back(key.at(i));
			}

			results_map.erase(check_key);
			results_map = AddResult(new_key, tmp_result, results_map, true);
		}
	}

	return results_map;
}

QueryResult PQLOptimizedEvaluator::SetResult(bool is_boolean_output, string bool_result, STRINGLIST_SET result) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - SetResult" << endl;
	}

	QueryResult final_result = *(new QueryResult());
	STRINGLIST_SET final_value = *(new STRINGLIST_SET());

	if (is_boolean_output) {
		STRING_LIST* value = new STRING_LIST();
		value->push_back(bool_result);
		final_value.insert(value);
	}
	else {
		final_value = result;
	}

	final_result.SetResult(final_value);
	return final_result;
}

bool PQLOptimizedEvaluator::ParseClauses(QueryInfo query_info, STRINGSET_STRINGLISTSET_MAP* synonyms_map, STRING_INTEGER_MAP* occurrence_count) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - ParseClauses" << endl;
	}

	STRINGPAIR_SET with_map = query_info.GetWithMap();
	STRING_STRINGLISTLIST_MAP st_map = query_info.GetStMap();
	STRING_STRINGLISTLIST_MAP pattern_map = query_info.GetPatternMap();

	for (STRING_PAIR* clause : with_map) {
		string lhs = clause->first;
		string rhs = clause->second;

		if (PQL_DEBUG) {
			cout << "LHS: " << lhs << ", RHS: " << rhs << endl;
		}

		if (!IsVar(lhs) && !IsVar(rhs)) {
			// INT=INT or STRING=STRING: Check if it is equal
			// If "FALSE" -> return false, else continue
			if (lhs.compare(rhs) != 0) {
				return false;
			}
		}
		else {
			if (!IsVar(lhs) && IsVar(rhs)) {
				// INT/STRING=synonyms
				clause->first = rhs;
				clause->second = lhs;
			}

			if (!(IsVar(rhs) && lhs.compare(rhs) == 0)) {
				// synonym=synonym: different synonyms
				// or synonym=INT or synonym=STRING

				// Increase occurrence count & add clause into synonyms_map
				STRING_SET* key = new STRING_SET();
				IncreaseOccurrenceCount(ParsingSynonym(clause->first), occurrence_count);
				key->insert(ParsingSynonym(clause->first));
				if (IsVar(clause->second)) {
					IncreaseOccurrenceCount(ParsingSynonym(clause->second), occurrence_count);
					key->insert(ParsingSynonym(clause->second));
				}

				STRINGLIST_SET value = *(new STRINGLIST_SET());
				STRING_LIST* parsed_clause = new STRING_LIST();
				parsed_clause->push_back(TYPE_WITH_CLAUSE);
				parsed_clause->push_back(clause->first);
				parsed_clause->push_back(clause->second);

				STRING_SET* check_key = GetRelatedSynonyms(*key, synonyms_map);
				if (check_key != nullptr) {
					key = check_key;
					value = synonyms_map->at(key);
					value.insert(parsed_clause);
					synonyms_map->at(key) = value;
				}
				else {
					value.insert(parsed_clause);
					if (synonyms_map->insert({ key, value }).second == 0) {
						// error
						if (PQL_DEBUG) {
							cout << "PQLOptimizedEvaluator - Parsing with clauses: Error inserting into synonyms_map." << endl;
						}
						return false;
					}
				}
			}
		}

	}

	for (auto f = st_map.cbegin(); f != st_map.cend(); f++) {
		string f_call = (*f).first;
		STRINGLIST_LIST all_params = (*f).second;

		for (STRING_LIST p : all_params) {
			if (p[0].compare(p[1]) == 0 && !IsUnderscore(p[0]) && f_call.compare(TYPE_COND_NEXT_T) != 0 && 
				f_call.compare(TYPE_COND_AFFECTS) != 0 && f_call.compare(TYPE_COND_AFFECTS_T) != 0) {
				// same parameter & not _ & not Next* & not Affects & not Affects*
				if (PQL_DEBUG) {
					cout << "PQLOptimizedEvaluator - Evaluating such that clauses: Same parameters." << endl;
				}
				return false;
			}

			STRING_LIST* clause = new STRING_LIST();
			clause->push_back(f_call);
			clause->push_back(p[0]);
			clause->push_back(p[1]);

			if (clause->empty()) {
				// error
				if (PQL_DEBUG) {
					cout << "PQLOptimizedEvaluator - Parsing such that clauses: Error creating value for such that clauses." << endl;
				}

				return false;
			}

			if (!IsVar(p[0]) && !IsVar(p[1])) {
				// both params != synonyms
				string param1 = IsVar(clause->at(1)) ? "" : clause->at(1);
				string param2 = IsVar(clause->at(2)) ? "" : clause->at(2);

				if (!EvaluateNoSynonymSet(f_call, param1, param2)) {
					if (PQL_DEBUG) {
						cout << "PQLOptimizedEvaluator - Evaluating no synonym clauses: False clause." << endl;
					}

					return false;
				}
			}
			else {
				STRING_SET* key = new STRING_SET();
				STRING_SET* check_key = new STRING_SET();
				STRINGLIST_SET value = *(new STRINGLIST_SET());

				if (IsVar(p[0]) && !IsVar(p[1])) {
					// 1st param == synonym
					key->insert(p[0]);

					// Increase count for synonym
					IncreaseOccurrenceCount(p[0], occurrence_count);
				}
				else if (!IsVar(p[0]) && IsVar(p[1])) {
					// 2nd param == synonym
					key->insert(p[1]);

					// Increase count for synonym
					IncreaseOccurrenceCount(p[1], occurrence_count);
				}
				else if (IsVar(p[0]) && IsVar(p[1])) {
					// 2 params = synonyms
					key->insert(p[0]);
					key->insert(p[1]);

					// Increase count for synonym
					IncreaseOccurrenceCount(p[0], occurrence_count);
					IncreaseOccurrenceCount(p[1], occurrence_count);
				}
				else {
					// error
					if (PQL_DEBUG) {
						cout << "PQLOptimizedEvaluator - Parsing such that clauses: Invalid parameters." << endl;
					}

					return false;
				}

				check_key = GetRelatedSynonyms(*key, synonyms_map);
				if (check_key != nullptr) {
					key = check_key;
					value = synonyms_map->at(key);

					int is_needed = 0;
					for (STRING_LIST* s : value) {
						if (s->at(1).compare(clause->at(1)) == 0 && s->at(2).compare(clause->at(2)) == 0) {
							// applicable for follows, parent, next, calls, affects

							// check if there is similar clause 
							is_needed = GetSimilarClause(clause->at(0), s->at(0));

							if (is_needed != 0) {
								if (is_needed == -1) {
									// replace existing clause with current clause relationship
									s->at(0) = clause->at(0);
								}
								break;
							}
						}
					}

					if (is_needed == 0) {
						value.insert(clause);
					}

					synonyms_map->at(key) = value;
				}
				else {
					value.insert(clause);
					if (synonyms_map->insert({ key, value }).second == 0) {
						// error
						if (PQL_DEBUG) {
							cout << "PQLOptimizedEvaluator - Parsing such that clauses: Error inserting into synonyms_map." << endl;
						}
						return false;
					}
				}
			}
		}
	}

	for (auto f = pattern_map.cbegin(); f != pattern_map.cend(); f++) {
		string f_call = (*f).first;
		STRINGLIST_LIST all_params = (*f).second;

		for (STRING_LIST p : all_params) {
			STRING_LIST* clause = new STRING_LIST();
			clause->push_back(f_call);
			clause->push_back(p[0]);
			clause->push_back(p[1]);
			clause->push_back(p[2]);

			string type = p[2];
			if (p.size() == 4) {
				// If pattern
				clause->push_back(p[3]);
				type = p[3];
			}

			if (clause->empty()) {
				// error
				if (PQL_DEBUG) {
					cout << "PQLOptimizedEvaluator - Parsing clauses: Error creating value for pattern clauses." << endl;
				}

				return false;
			}

			STRING_SET* key = new STRING_SET();
			key->insert(type);

			// Increase count for synonym
			IncreaseOccurrenceCount(type, occurrence_count);

			STRING_SET* check_key = new STRING_SET();
			STRINGLIST_SET value = *(new STRINGLIST_SET());

			if (IsVar(p[0])) {
				// 1st param = synonym 
				key->insert(p[0]);

				// Increase count for synonym
				IncreaseOccurrenceCount(p[0], occurrence_count);
			}

			check_key = GetRelatedSynonyms(*key, synonyms_map);
			if (check_key != nullptr) {
				key = check_key;
				value = synonyms_map->at(key);
				value.insert(clause);
				synonyms_map->at(key) = value;
			}
			else {
				value.insert(clause);
				if (synonyms_map->insert({ key, value }).second == 0) {
					// error
					if (PQL_DEBUG) {
						cout << "PQLOptimizedEvaluator - Parsing pattern clauses: Error inserting into synonyms_map." << endl;
					}
					return false;
				}
			}
		}
	}

	return true;
}

STRING_LIST PQLOptimizedEvaluator::GetClauseKey(STRING_LIST clause) {
	STRING_LIST c_key = STRING_LIST();

	string f_call = clause.at(0);
	string param1 = clause.at(1);
	string param2 = clause.at(2);

	if (f_call.compare(TYPE_WITH_CLAUSE) == 0) {
		c_key.push_back(ParsingSynonym(param1));
		if (IsVar(param2)) {
			c_key.push_back(ParsingSynonym(param2));
		}
	}
	else if (f_call.compare(TYPE_COND_PATTERN_F) == 0 || f_call.compare(TYPE_COND_PATTERN_P) == 0) {

		string param3 = clause.at(3);
		if (clause.size() == 5) {
			param3 = clause.at(4);
		}
		c_key.push_back(param3);

		if (IsVar(param1)) {
			// first param = synonym
			c_key.push_back(param1);
		}
	}
	else {
		if (IsVar(param1) && IsVar(param2)) {
			// both params = synonyms
			c_key.push_back(param1);

			if (param1.compare(param2) != 0) {
				// different synonyms
				c_key.push_back(param2);
			}
		}
		else if (!IsVar(param1) && IsVar(param2)) {
			// first param != synonym & second param = synonym
			c_key.push_back(param2);

		}
		else {
			// first param = synonym & second param != synonym
			c_key.push_back(param1);

		}
	}

	return c_key;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetRelatedResult(string synonym, STRINGLIST_STRINGLISTSET_MAP synonyms_map) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - GetRelatedSynonyms" << endl;
	}

	for (auto k = synonyms_map.cbegin(); k != synonyms_map.cend(); k++) {
		STRING_LIST* existing_key = (*k).first;
		for (int i = 0; i < existing_key->size(); i++) {
			if (existing_key->at(i).compare(synonym) == 0) {
				// found related synonyms in the synonyms_map
				return ConvertSet(GetNewResult(synonyms_map.at(existing_key), i));
			}
		}
	}

	return {};
}

STRING_LIST* PQLOptimizedEvaluator::GetRelatedSynonyms(STRING_LIST synonyms, STRINGLIST_STRINGLISTSET_MAP synonyms_map) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - GetRelatedSynonyms" << endl;
	}

	for (auto k = synonyms_map.cbegin(); k != synonyms_map.cend(); k++) {
		STRING_LIST* existing_key = (*k).first;

		for (string existing_synonym : *existing_key) {
			// found related synonyms in the synonyms_map
			for (string checking_synonym : synonyms) {
				if (existing_synonym.compare(checking_synonym) == 0) {
					return existing_key;
				}
			}
		}
	}

	return nullptr;
}

STRING_SET* PQLOptimizedEvaluator::GetRelatedSynonyms(string synonym, STRINGSET_STRINGLISTSET_MAP synonyms_map) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - GetRelatedSynonyms" << endl;
	}

	for (auto k = synonyms_map.cbegin(); k != synonyms_map.cend(); k++) {
		STRING_SET* existing_key = (*k).first;
		if (existing_key->find(synonym) != existing_key->end()) {
			// found related synonyms in the synonyms_map
			return existing_key;
		}
	}

	return nullptr;
}

STRING_SET* PQLOptimizedEvaluator::GetRelatedSynonyms(STRING_SET synonyms, STRINGSET_STRINGLISTSET_MAP* synonyms_map) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - GetRelatedSynonyms" << endl;
	}

	bool has_related_synonyms = false;

	STRING_SET* new_keys = new STRING_SET();
	STRINGLIST_SET new_values = *(new STRINGLIST_SET());

	for (auto k = synonyms.cbegin(); k != synonyms.cend(); k++) {
		string curr_check = *k;
		STRING_SET* check_key = GetRelatedSynonyms(curr_check, *synonyms_map);
		while (check_key != nullptr) {
			// found related synonyms in the synonyms_map
			new_keys->insert(check_key->begin(), check_key->end());

			// combine the clauses
			new_values.insert(synonyms_map->at(check_key).begin(), synonyms_map->at(check_key).end());

			// remove the existing synonyms since already combined
			synonyms_map->erase(check_key);
			check_key = GetRelatedSynonyms(curr_check, *synonyms_map);
		}
	}

	if (!new_keys->empty()) {
		// make sure none is missed out
		new_keys->insert(synonyms.begin(), synonyms.end());

		// insert the combined clauses into the new combined synonyms
		synonyms_map->insert({ new_keys, new_values });

		return new_keys;
	}
	else {
		return nullptr;
	}
}

bool PQLOptimizedEvaluator::EvaluateNoSynonymSet(string f_call, string param1, string param2) {
	auto start = high_resolution_clock::now();
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateNoSynonymSet" << endl;
		cout << "fcall: " << f_call << "; param1: " << param1 << "; param2: " << param2 << endl;
	}

	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();
	PKB::AffectsManager am = pkb.GetAffectsManager();
	CFGManager cfgm = pkb.GetCFGManager();

	if (PQL_UNIT_TESTING) {
		cout << "PQLOptimizedEvaluator - EvaluateNoSynonymSet - UNIT TESTING" << endl;
		return true;
	}

	bool bool_value;

	if (f_call.compare(TYPE_COND_FOLLOWS) == 0) {
		bool_value = rm.IsFollows(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_FOLLOWS_T) == 0) {
		bool_value = rm.IsFollowsStar(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_PARENT) == 0) {
		bool_value = rm.IsParent(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_PARENT_T) == 0) {
		bool_value = rm.IsParentStar(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_USES_S) == 0) {
		bool_value = rm.IsStmtUses(ParsingStmtRef(param1), ParsingEntRef(param2));
	}
	else if (f_call.compare(TYPE_COND_USES_P) == 0) {
		bool_value = rm.IsProcUses(ParsingEntRef(param1), ParsingEntRef(param2));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_S) == 0) {
		bool_value = rm.IsStmtModifies(ParsingStmtRef(param1), ParsingEntRef(param2));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_P) == 0) {
		bool_value = rm.IsProcModifies(ParsingEntRef(param1), ParsingEntRef(param2));
	}
	else if (f_call.compare(TYPE_COND_CALLS) == 0) {
		bool_value = rm.IsCalls(ParsingEntRef(param1), ParsingEntRef(param2));
	}
	else if (f_call.compare(TYPE_COND_CALLS_T) == 0) {
		bool_value = rm.IsCallsStar(ParsingEntRef(param1), ParsingEntRef(param2));
	}
	else if (f_call.compare(TYPE_COND_NEXT) == 0) {
		bool_value = cfgm.IsNext(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_NEXT_T) == 0) {
		bool_value = cfgm.IsNextStar(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS) == 0) {
		bool_value = am.IsAffects(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		bool_value = am.IsAffectsStar(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else {
		// error
		if (PQL_DEBUG) {
			cout << "PQLOptimizedEvaluator - EvaluateNoSynonymSet: No such relref." << endl;
		}

		bool_value = false;
	}

	if (DEBUG_TIMER) {
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << "Time taken for boolean " << f_call << " = " << duration.count() << "ms" << endl;
	}

	return bool_value;
}

STRINGLIST_SET PQLOptimizedEvaluator::EvaluatePatternCall(string f_call, string param1, string param2, string type) {
	auto start = high_resolution_clock::now();
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluatePatternCall (2 params to pass)" << endl;
		cout << "fcall: " << f_call << "; param: " << param1 << "; param2: " << param2 << "; type: " << type << endl;
	}

	PKB pkb;
	PatternManager pm = pkb.GetPatternManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (f_call.compare(TYPE_COND_PATTERN_P) == 0) {
		if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignWithSubPattern(ParsingEntRef(param1), ParsingEntRef(param2)));
		}
		else {
			// error
			if (PQL_DEBUG) {
				cout << "PQLOptimizedEvaluator - EvaluatePatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else if (f_call.compare(TYPE_COND_PATTERN_F) == 0) {
		if (type.compare(TYPE_STMT_IF) == 0) {
			result = ConvertSet(pm.GetIfWithPattern(ParsingEntRef(param1)));
		}
		else if (type.compare(TYPE_STMT_WHILE) == 0) {
			result = ConvertSet(pm.GetWhileWithPattern(ParsingEntRef(param1)));
		}
		else if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignWithFullPattern(ParsingEntRef(param1), ParsingEntRef(param2)));
		}
		else {
			// error
			if (PQL_DEBUG) {
				cout << "PQLOptimizedEvaluator - EvaluatePatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else {
		// error
		if (PQL_DEBUG) {
			cout << "PQLOptimizedEvaluator - EvaluatePatternCall: No such pattern call." << endl;
		}

		return {};
	}

	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluatePatternCall: Result clause's size = " << result.size() << endl;
	}

	if (DEBUG_TIMER) {
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << "Time taken for " << type << " = " << duration.count() << "ms" << endl;
	}

	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::EvaluatePatternCall(string f_call, string param, string type) {
	auto start = high_resolution_clock::now();
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluatePatternCall (1 param to pass)" << endl;
		cout << "fcall: " << f_call << "; param: " << param << "; type: " << type << endl;
	}

	PKB pkb;
	PatternManager pm = pkb.GetPatternManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (f_call.compare(TYPE_COND_PATTERN_P) == 0) {
		if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignStmtVarPairWithSubPattern("", ParsingEntRef(param)));
		}
		else {
			// error 
			if (PQL_DEBUG) {
				cout << "PQLOptimizedEvaluator - EvaluatePatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else if (f_call.compare(TYPE_COND_PATTERN_F) == 0) {
		if (type.compare(TYPE_STMT_IF) == 0) {
			result = ConvertSet(pm.GetIfStmtVarPairWithPattern(""));
		}
		else if (type.compare(TYPE_STMT_WHILE) == 0) {
			result = ConvertSet(pm.GetWhileStmtVarPairWithPattern(""));
		}
		else if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignStmtVarPairWithFullPattern("", ParsingEntRef(param)));
		}
		else {
			// error 
			if (PQL_DEBUG) {
				cout << "PQLOptimizedEvaluator - EvaluatePatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else {
		// error
		if (PQL_DEBUG) {
			cout << "PQLOptimizedEvaluator - EvaluatePatternCall: No such pattern call." << endl;
		}

		return {};
	}

	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluatePatternCall: Result clause's size = " << result.size() << endl;
	}

	if (DEBUG_TIMER) {
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << "Time taken for " << type << " = " << duration.count() << "ms" << endl;
	}

	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::EvaluateWithClause(STRING_STRING_MAP entity_map, string lhs, string rhs, STRINGLIST_SET lhs_result, STRINGLIST_SET rhs_result) {
	auto start = high_resolution_clock::now();

	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateConstraints" << endl;
	}

	string parsed_lhs = ParsingSynonym(lhs);
	string parsed_rhs = ParsingSynonym(rhs);
	string lhs_attr = ParsingSynonymAttribute(lhs);
	string rhs_attr = ParsingSynonymAttribute(rhs);
	string lhs_type = entity_map.at(parsed_lhs);

	STRINGLIST_SET value = *(new STRINGLIST_SET());

	if (IsVar(lhs) && !IsVar(rhs)) {
		// synonym=int/string

		string parsed_string = rhs;
		if (IsString(rhs)) {
			parsed_string = ParsingEntRef(rhs);
		}

		value = lhs_result.empty() ? EvaluateAllCall(lhs_type) : lhs_result;

		if (IsSameEntityType(lhs_type, lhs_attr)) {
			// remove directly
			STRING_SET tmp = STRING_SET({ parsed_string });
			RemoveIrrelevant(&value, ConvertSet(tmp), 0);
			Print(value);
		}
		else {
			// convert then remove
			STRINGLIST_SET tmp = GetAlternateResult(parsed_string, lhs_type);
			cout << parsed_string << endl;
			Print(tmp);
			lhs_result.empty() || tmp.empty() ? value = tmp : RemoveIrrelevant(&value, tmp, 0);
			Print(value);
		}
	}
	else if (IsVar(lhs) && IsVar(rhs)) {
		//synonym=synonym

		string rhs_type = entity_map.at(parsed_rhs);
		STRINGLIST_SET lhs_value = lhs_result.empty() ? EvaluateAllCall(lhs_type) : lhs_result;
		STRINGLIST_SET rhs_value = rhs_result.empty() ? EvaluateAllCall(rhs_type) : rhs_result;
		STRING_STRINGSET_MAP lhs_w_attr = STRING_STRINGSET_MAP();
		STRING_STRINGSET_MAP rhs_w_attr = STRING_STRINGSET_MAP();

		if (!IsSameEntityType(entity_map.at(parsed_lhs), lhs_attr)) {
			for (STRING_LIST* s : lhs_value) {
				STRING_SET v = ConvertSet(GetAlternateResult(s->at(0), lhs_type));

				lhs_w_attr.insert({ s->at(0), v });
			}
		}

		if (!IsSameEntityType(entity_map.at(parsed_rhs), rhs_attr)) {
			for (STRING_LIST* s : rhs_value) {
				STRING_SET v = ConvertSet(GetAlternateResult(s->at(0), rhs_type));

				rhs_w_attr.insert({ s->at(0), v });
			}
		}

		if (lhs_w_attr.empty() && rhs_w_attr.empty()) {
			// comparing lhs and rhs
			for (STRING_LIST* lhs_s : lhs_value) {
				for (STRING_LIST* rhs_s : rhs_value) {
					if (lhs_s->at(0).compare(rhs_s->at(0)) == 0) {
						STRING_LIST* to_insert = new STRING_LIST();
						to_insert->push_back(lhs_s->at(0));
						to_insert->push_back(lhs_s->at(0));

						if (!IsDuplicate(value, *to_insert)) {
							value.insert(to_insert);
						}
					}
				}
			}
		}
		else if (!lhs_w_attr.empty() && !rhs_w_attr.empty()) {
			// compare lhs_w_attr and rhs_w_attr
			for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
				for (auto g = rhs_w_attr.cbegin(); g != rhs_w_attr.cend(); g++) {
					string lhs_key = (*f).first;
					string rhs_key = (*g).first;
					STRING_SET lhs_v = (*f).second;
					STRING_SET rhs_v = (*g).second;

					STRING_SET val = GetIntersectResult(lhs_v, rhs_v);

					if (!val.empty()) {
						STRING_LIST* to_insert = new STRING_LIST();
						to_insert->push_back(lhs_key);
						to_insert->push_back(rhs_key);

						if (!IsDuplicate(value, *to_insert)) {
							value.insert(to_insert);
						}
					}
				}
			}
		}
		else if (!lhs_w_attr.empty() && rhs_w_attr.empty()) {
			// comparing lhs_w_attr and rhs
			for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
				string key = (*f).first;
				STRING_SET v = (*f).second;
				STRING_SET val = GetIntersectResult(v, rhs_value, 0);

				for (string s : val) {
					STRING_LIST* to_insert = new STRING_LIST();
					to_insert->push_back(key);
					to_insert->push_back(s);

					if (!IsDuplicate(value, *to_insert)) {
						value.insert(to_insert);
					}
				}
			}
		}
		else if (lhs_w_attr.empty() && !rhs_w_attr.empty()) {
			// comparing lhs and rhs_w_attr
			for (auto f = rhs_w_attr.cbegin(); f != rhs_w_attr.cend(); f++) {
				string key = (*f).first;
				STRING_SET v = (*f).second;
				STRING_SET val = GetIntersectResult(v, lhs_value, 0);

				for (string s : val) {
					STRING_LIST* to_insert = new STRING_LIST();
					to_insert->push_back(s);
					to_insert->push_back(key);

					if (!IsDuplicate(value, *to_insert)) {
						value.insert(to_insert);
					}
				}
			}
		}
	}

	if (DEBUG_TIMER) {
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << "Time taken for evaluating with clause = " << duration.count() << "ms" << endl;
	}

	return value;
}

STRINGLIST_SET PQLOptimizedEvaluator::EvaluateOneSynonymSet(string f_call, string param, string entity_type) {
	auto start = high_resolution_clock::now();

	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateOneSynonymSet" << endl;
		cout << "fcall: " << f_call << "; param: " << param << endl;
	}

	PKB pkb;
	RelationManager rm = pkb.GetRelationManager();
	CFGManager cfgm = pkb.GetCFGManager();
	PKB::AffectsManager am = pkb.GetAffectsManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	STATEMENT_TYPE param_type = GetStmtType(entity_type);

	if (f_call.compare(TYPE_COND_FOLLOWS) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(rm.GetFollows(ParsingStmtRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_FOLLOWS_T) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(rm.GetFollowsStars(ParsingStmtRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_PARENT) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(rm.GetParents(ParsingStmtRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_PARENT_T) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(rm.GetParentStars(ParsingStmtRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_USES_S) == 0) {
		result = ConvertSet(rm.GetStmtUses(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_USES_P) == 0) {
		result = ConvertSet(rm.GetProcUses(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_S) == 0) {
		result = ConvertSet(rm.GetStmtModifies(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_P) == 0) {
		result = ConvertSet(rm.GetProcModifies(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_CALLS) == 0) {
		result = ConvertSet(rm.GetCalls(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_CALLS_T) == 0) {
		result = ConvertSet(rm.GetCallsStars(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_NEXT) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(cfgm.GetNext(ParsingStmtRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_NEXT_T) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(cfgm.GetNextStar(ParsingStmtRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS) == 0) {
		result = ConvertSet(am.GetAffects(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		result = ConvertSet(am.GetAffectsStar(ParsingStmtRef(param)));
	}
	else {
		// error
		if (PQL_DEBUG) {
			cout << "PQLOptimizedEvaluator - EvaluateOneSynonymSet: No such relref." << endl;
		}

		return {};
	}

	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateOneSynonymSet: Result clause's size = " << result.size() << endl;
	}

	if (DEBUG_TIMER) {
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << "Time taken for getXXX api on " << f_call << " clause = " << duration.count() << "ms" << endl;
	}

	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::EvaluateInverseOneSynonymSet(string f_call, string param, string entity_type) {
	auto start = high_resolution_clock::now();

	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateInverseOneSynonymSet" << endl;
		cout << "fcall: " << f_call << "; param: " << param << endl;
	}

	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();
	CFGManager cfgm = pkb.GetCFGManager();
	PKB::AffectsManager am = pkb.GetAffectsManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	STATEMENT_TYPE param_type = GetStmtType(entity_type);

	if (f_call.compare(TYPE_COND_FOLLOWS) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(rm.GetInverseFollows(ParsingStmtRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_FOLLOWS_T) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(rm.GetInverseFollowsStars(ParsingStmtRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_PARENT) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(rm.GetInverseParents(ParsingStmtRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_PARENT_T) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(rm.GetInverseParentStars(ParsingStmtRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_USES_S) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(rm.GetInverseStmtUses(ParsingEntRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_USES_P) == 0) {
		result = ConvertSet(rm.GetInverseProcUses(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_S) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(rm.GetInverseStmtModifies(ParsingEntRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_P) == 0) {
		result = ConvertSet(rm.GetInverseProcModifies(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_CALLS) == 0) {
		result = ConvertSet(rm.GetInverseCalls(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_CALLS_T) == 0) {
		result = ConvertSet(rm.GetInverseCallsStars(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_NEXT) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(cfgm.GetInverseNext(ParsingStmtRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_NEXT_T) == 0) {
		result = ConvertSet(pkb.FilterStmtTypes(cfgm.GetInverseNextStar(ParsingStmtRef(param)), param_type));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS) == 0) {
		result = ConvertSet(am.GetInverseAffects(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		result = ConvertSet(am.GetInverseAffectsStar(ParsingStmtRef(param)));
	}
	else {
		// error
		if (PQL_DEBUG) {
			cout << "PQLOptimizedEvaluator - EvaluateInverseOneSynonymSet: No such relref." << endl;
		}
		return {};
	}

	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateInverseOneSynonymSet: Result clause's size = " << result.size() << endl;
	}

	if (DEBUG_TIMER) {
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << "Time taken for getInverseXXX api on " << f_call << " clause = " << duration.count() << "ms" << endl;
	}

	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::EvaluateTwoSynonymSet(string f_call, string param1_entity_type, string param2_entity_type, bool is_same_synonyms) {
	auto start = high_resolution_clock::now();
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateTwoSynonymSet" << endl;
		cout << "fcall: " << f_call << endl;
	}

	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();
	CFGManager cfgm = pkb.GetCFGManager();
	PKB::AffectsManager am = pkb.GetAffectsManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	STATEMENT_TYPE param1_type = GetStmtType(param1_entity_type);
	STATEMENT_TYPE param2_type = GetStmtType(param2_entity_type);

	if (is_same_synonyms) {
		if (f_call.compare(TYPE_COND_NEXT_T) == 0) {
			result = ConvertSet(pkb.FilterStmtTypes(cfgm.GetAllNextStarWithSameSynonyms(), param1_type, param2_type));
		}
		else if (f_call.compare(TYPE_COND_AFFECTS) == 0) {
			result = ConvertSet(am.GetAllAffectsWithSameSynonyms());
		}
		else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
			result = ConvertSet(am.GetAllAffectsStarWithSameSynonyms());
		}
		else {
			// error
			if (PQL_DEBUG) {
				cout << "PQLOptimizedEvaluator - EvaluateTwoSynonymSet: No such relref (same synonyms)." << endl;
			}
			return {};
		}
	}
	else {
		if (f_call.compare(TYPE_COND_FOLLOWS) == 0) {
			result = ConvertSet(pkb.FilterStmtTypes(rm.GetAllFollows(), param1_type, param2_type));
		}
		else if (f_call.compare(TYPE_COND_FOLLOWS_T) == 0) {
			result = ConvertSet(pkb.FilterStmtTypes(rm.GetAllFollowsStars(), param1_type, param2_type));
		}
		else if (f_call.compare(TYPE_COND_PARENT) == 0) {
			result = ConvertSet(pkb.FilterStmtTypes(rm.GetAllParents(), param1_type, param2_type));
		}
		else if (f_call.compare(TYPE_COND_PARENT_T) == 0) {
			result = ConvertSet(pkb.FilterStmtTypes(rm.GetAllParentStars(), param1_type, param2_type));
		}
		else if (f_call.compare(TYPE_COND_USES_S) == 0) {
			result = ConvertSet(pkb.FilterStmtTypes(rm.GetAllStmtUses(), param1_type));
		}
		else if (f_call.compare(TYPE_COND_USES_P) == 0) {
			result = ConvertSet(rm.GetAllProcUses());
		}
		else if (f_call.compare(TYPE_COND_MODIFIES_S) == 0) {
			result = ConvertSet(pkb.FilterStmtTypes(rm.GetAllStmtModifies(), param1_type));
		}
		else if (f_call.compare(TYPE_COND_MODIFIES_P) == 0) {
			result = ConvertSet(rm.GetAllProcModifies());
		}
		else if (f_call.compare(TYPE_COND_CALLS) == 0) {
			result = ConvertSet(rm.GetAllCalls());
		}
		else if (f_call.compare(TYPE_COND_CALLS_T) == 0) {
			result = ConvertSet(rm.GetAllCallsStar());
		}
		else if (f_call.compare(TYPE_COND_NEXT) == 0) {
			result = ConvertSet(pkb.FilterStmtTypes(cfgm.GetAllNext(), param1_type, param2_type));
		}
		else if (f_call.compare(TYPE_COND_NEXT_T) == 0) {
			result = ConvertSet(pkb.FilterStmtTypes(cfgm.GetAllNextStar(), param1_type, param2_type));
		}
		else if (f_call.compare(TYPE_COND_AFFECTS) == 0) {
			result = ConvertSet(am.GetAllAffects());
		}
		else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
			result = ConvertSet(am.GetAllAffectsStar());
		}
		else {
			// error
			if (PQL_DEBUG) {
				cout << "PQLOptimizedEvaluator - EvaluateTwoSynonymSet: No such relref." << endl;
			}
			return {};
		}
	}

	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateTwoSynonymSet: Result clause's size = " << result.size() << endl;
	}

	if (DEBUG_TIMER) {
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << "Time taken for getAllXXX api on " << f_call << " clause = " << duration.count() << "ms" << endl;
	}

	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::EvaluateAllCall(string output_var_type) {
	auto start = high_resolution_clock::now();
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateAllCall" << endl;
		cout << "output_var_type: " << output_var_type << endl;
	}

	PKB pkb = PKB();
	DataManager dm = pkb.GetDataManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (output_var_type.compare(TYPE_STMT) == 0 || output_var_type.compare(TYPE_PROG_LINE) == 0) {
		result = ConvertSet(dm.GetAllStatements());
	}
	else if (output_var_type.compare(TYPE_STMT_ASSIGN) == 0) {
		result = ConvertSet(dm.GetAllStatements(STATEMENT_TYPE::assignStatement));
	}
	else if (output_var_type.compare(TYPE_STMT_CALL) == 0) {
		result = ConvertSet(dm.GetAllStatements(STATEMENT_TYPE::callStatement));
	}
	else if (output_var_type.compare(TYPE_STMT_IF) == 0) {
		result = ConvertSet(dm.GetAllStatements(STATEMENT_TYPE::ifStatement));
	}
	else if (output_var_type.compare(TYPE_STMT_WHILE) == 0) {
		result = ConvertSet(dm.GetAllStatements(STATEMENT_TYPE::whileStatement));
	}
	else if (output_var_type.compare(TYPE_STMT_PRINT) == 0) {
		result = ConvertSet(dm.GetAllStatements(STATEMENT_TYPE::printStatement));
	}
	else if (output_var_type.compare(TYPE_STMT_READ) == 0) {
		result = ConvertSet(dm.GetAllStatements(STATEMENT_TYPE::readStatement));
	}
	else if (output_var_type.compare(TYPE_VAR) == 0) {
		result = ConvertSet(dm.GetAllVariables());
	}
	else if (output_var_type.compare(TYPE_PROC) == 0) {
		result = ConvertSet(dm.GetAllProcedures());
	}
	else if (output_var_type.compare(TYPE_CONST) == 0) {
		result = ConvertSet(dm.GetAllConstants());
	}
	else {
		// error
		if (PQL_DEBUG) {
			cout << "PQLOptimizedEvaluator - EvaluateAllCall: No such relref." << endl;
		}
		return {};
	}

	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateAllCall: Result clause's size = " << result.size() << endl;
	}

	if (DEBUG_TIMER) {
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << "Time taken for getAllXXX api on " << output_var_type << "  = " << duration.count() << "ms" << endl;
	}
	return result;
}

STRING_SET PQLOptimizedEvaluator::ConvertSet(STRINGLIST_SET result_set) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRING_SET final_result = *(new STRING_SET());

	if (!result_set.empty()) {
		for (STRING_LIST* k : result_set) {
			final_result.insert(k->at(0));
		}
	}
	return final_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::ConvertSet(STRING_SET result_set) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (string k : result_set) {
			STRING_LIST* value = new STRING_LIST();
			value->push_back(k);

			final_result.insert(value);
		}
	}
	return final_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::ConvertSet(INTEGER_SET result_set) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (int k : result_set) {
			STRING_LIST* value = new STRING_LIST();
			value->push_back(to_string(k));

			final_result.insert(value);
		}
	}

	return final_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::ConvertSet(STMT_STMT_PAIR_LIST result_set) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (STMT_STMT_PAIR k : result_set) {
			string first = to_string(k.s1);
			string second = to_string(k.s2);

			STRING_LIST* value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result.insert(value);
		}
	}

	return final_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::ConvertSet(STMT_VAR_PAIR_LIST result_set) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (STMT_VAR_PAIR k : result_set) {
			string first = to_string(k.s);
			string second = k.v;

			STRING_LIST* value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result.insert(value);
		}
	}

	return final_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::ConvertSet(PROC_VAR_PAIR_LIST result_set) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (PROC_VAR_PAIR k : result_set) {
			string first = k.p;
			string second = k.v;

			STRING_LIST* value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result.insert(value);
		}
	}

	return final_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::ConvertSet(PROC_PROC_PAIR_LIST result_set) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (PROC_PROC_PAIR k : result_set) {
			string first = k.p1;
			string second = k.p2;

			STRING_LIST* value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result.insert(value);
		}
	}

	return final_result;
}

STRING_SET PQLOptimizedEvaluator::GetIntersectResult(STRING_SET val1, STRINGLIST_SET val2, int pos_to_check) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - GetIntersectResult" << endl;
	}

	STRING_SET result = *(new STRING_SET());

	for (STRING_LIST* i : val2) {
		for (string j : val1) {
			if ((*i).at(pos_to_check) == j) {
				result.insert(j);
			}
		}
	}

	return result;
}

STRING_SET PQLOptimizedEvaluator::GetIntersectResult(STRING_SET val1, STRING_SET val2) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - GetIntersectResult" << endl;
	}

	STRING_SET result = *(new STRING_SET());

	if (val1.size() > val2.size()) {
		for (auto i = val2.begin(); i != val2.end(); i++) {
			if (val1.find(*i) != val1.end())
				result.insert(*i);
		}
	}
	else {
		for (auto i = val1.begin(); i != val1.end(); i++) {
			if (val2.find(*i) != val2.end())
				result.insert(*i);
		}
	}

	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetAlternateResult(string values, string type) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - GetAlternateResult" << endl;
	}

	RelationManager rm = PKB().GetRelationManager();
	DataManager dm = PKB().GetDataManager();

	STRINGLIST_SET results = *(new STRINGLIST_SET());

	if (type.compare(TYPE_STMT_PRINT) == 0) {
		if (IsInteger(values)) {
			results = ConvertSet(rm.GetStmtUses(ParsingStmtRef(values)));
		}
		else {
			results = ConvertSet(PKB().FilterStmtTypes(rm.GetInverseStmtUses(ParsingEntRef(values)), STATEMENT_TYPE::printStatement));
		}

	}
	else if (type.compare(TYPE_STMT_READ) == 0) {
		if (IsInteger(values)) {
			results = ConvertSet(rm.GetStmtModifies(ParsingStmtRef(values)));
		}
		else {
			results = ConvertSet(PKB().FilterStmtTypes(rm.GetInverseStmtModifies(ParsingEntRef(values)), STATEMENT_TYPE::readStatement));
		}
	}
	else {
		// calls
		if (IsInteger(values)) {
			string new_value = dm.GetCalledByStmt(ParsingStmtRef(values));
			if (new_value.compare("") != 0) {
				results.insert(new STRING_LIST({ new_value }));
			}
		}
		else {
			results = ConvertSet(dm.GetInverseCalledBy(ParsingEntRef(values)));
		}
	}
	return results;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetAlternateResult(STRING_LIST values, int pos_to_check, string type) {
	STRINGLIST_SET tmp = *(new STRINGLIST_SET());

	for (int i = 0; i < values.size(); i++) {
		STRING_SET new_values = STRING_SET();
		if (pos_to_check != i) {
			new_values.insert(values.at(i));
		}
		else {
			new_values = ConvertSet(GetAlternateResult(values.at(pos_to_check), type));
		}

		if (tmp.empty()) {
			tmp = ConvertSet(new_values);
		}
		else {
			STRINGLIST_SET tmp_clone = STRINGLIST_SET();
			for (string s : new_values) {
				for (STRING_LIST* t : tmp) {
					STRING_LIST t_clone = *t;
					t_clone.push_back(s);
					tmp_clone.insert(new STRING_LIST(t_clone));
				}
			}

			tmp = tmp_clone;
		}
	}

	return tmp;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetAlternateResult(STRINGLIST_SET values, int pos_to_check, string type) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - GetAlternateResult" << endl;
	}

	STRINGLIST_SET results = *(new STRINGLIST_SET());

	for (STRING_LIST* v : values) {
		STRINGLIST_SET tmp = GetAlternateResult(*v, pos_to_check, type);
		results.insert(tmp.begin(), tmp.end());
	}

	return results;
}

STRING_SET PQLOptimizedEvaluator::GetNewResult(STRINGLIST_SET value, int pos_to_check) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - GetNewResult" << endl;
	}

	STRING_SET result = *(new STRING_SET());

	for (STRING_LIST* i : value) {
		result.insert((*i).at(pos_to_check));
	}

	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetNewResult(STRINGLIST_SET value, INTEGER_LIST pos_to_check) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - GetNewResult" << endl;
	}

	STRINGLIST_SET result = *(new STRINGLIST_SET());

	for (STRING_LIST* entry : value) {
		STRING_LIST* entry_value = new STRING_LIST();

		for (int j = 0; j < pos_to_check.size(); j++) {
			// Found value to be included in the result
			entry_value->push_back(entry->at(pos_to_check.at(j)));
		}

		if (!IsDuplicate(result, *entry_value)) {
			result.insert(entry_value);
		}
	}

	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetCartesianProduct(STRINGLIST_STRINGLISTSET_MAP results_map, STRING_LIST output_list, STRING_STRING_MAP entity_map) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - GetCartesianProduct" << endl;
	}

	STRINGLIST_SET results = *(new STRINGLIST_SET());
	STRING_LIST added_output = STRING_LIST();
	for (string output : output_list) {
		string parsed_output = ParsingSynonym(output);

		for (auto entry = results_map.cbegin(); entry != results_map.cend(); entry++) {
			STRING_LIST* synonyms = (*entry).first;
			STRINGLIST_SET values = (*entry).second;
			bool is_tuple = synonyms->size() > 1 ? true : false;

			for (int i = 0; i < synonyms->size(); i++) {
				if (synonyms->at(i).compare(parsed_output) == 0) {
					// output synonym found

					// Check constraint if synonym has already been added in output list
					// Added = dependency, else no
					INTEGERPAIR_SET added_index = INTEGERPAIR_SET();
					for (int s = 0; s < synonyms->size(); s++) {
						for (int a = 0; a < added_output.size(); a++) {
							if (synonyms->at(s).compare(added_output.at(a)) == 0) {
								INTEGER_PAIR* pair = new INTEGER_PAIR();
								pair->first = a;
								pair->second = s;

								added_index.insert(pair);
							}
						}
					}

					string type = entity_map.at(parsed_output);
					string attr = ParsingSynonymAttribute(output);

					if (added_index.empty()) {
						// Get the values to be added
						if (is_tuple) {
							results = GetNoDependencyProduct(results, GetNewResult(values, i));
						}
						else {
							results = GetNoDependencyProduct(results, ConvertSet(values));
						}
					}
					else {
						// Merge with respect to dependencies
						results = GetDependencyProduct(results, values, { i }, added_index);
					}
					added_output.push_back(parsed_output);
					break;
				}
			}
		}
	}

	return results;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetDependencyProduct(STRINGLIST_SET results, STRINGLIST_SET values, INTEGER_LIST pos_to_add, INTEGERPAIR_SET to_check, string type, string attr) {
	auto start = high_resolution_clock::now();
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - GetDependencyProduct" << endl;
	}

	STRINGLIST_SET final_results = *(new STRINGLIST_SET());

	if (results.empty()) {
		final_results = GetNewResult(values, pos_to_add);

		// Only should be called by GetCartesianProduct()
		if (!type.empty() && !attr.empty() && !IsSameEntityType(type, attr)) {
			final_results = GetAlternateResult(final_results, pos_to_add.at(0), type);
		}
	}
	else {
		STRINGLIST_SET base_values = GetNewResult(values, pos_to_add);

		for (STRING_LIST* set : results) {
			STRINGLIST_SET values_remaining = base_values;
			for (STRING_LIST* val : values) {
				if (IsDependencyStatisfied(*set, *val, to_check)) {
					STRING_LIST new_value = *(new STRING_LIST());

					new_value.insert(new_value.end(), set->begin(), set->end());

					INTEGER_LIST index_to_convert = INTEGER_LIST();
					for (int i : pos_to_add) {
						new_value.push_back(val->at(i));

						// check attribute if present
						if (!type.empty() && !attr.empty() && !IsSameEntityType(type, attr)) {
							index_to_convert.push_back(new_value.size() - 1);
						}
					}

					STRINGLIST_SET tmp = *(new STRINGLIST_SET());
					for (int idx : index_to_convert) {
						tmp = GetAlternateResult(new_value, idx, type);
					}

					if (tmp.empty()) {
						 if (!IsDuplicate(final_results, new_value)) {
						 	final_results.insert(new STRING_LIST(new_value));
						 }
					}
					else {
						for (STRING_LIST* l : tmp) {
							if (!IsDuplicate(final_results, *l)) {
								final_results.insert(l);
							}
						}
					}

				}
			}
		}
	}

	auto stop = high_resolution_clock::now();
	if (DEBUG_TIMER) {
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << "Time taken for GetDependencyProduct = " << duration.count() << "ms" << endl;
	}

	return final_results;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetNoDependencyProduct(STRINGLIST_SET results, STRING_SET values, string type, string attr) {
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - GetNoDependencyProduct" << endl;
	}

	STRINGLIST_SET final_results = *(new STRINGLIST_SET());

	if (results.empty()) {
		// check attribute if present
		final_results = ConvertSet(values);
		if (!type.empty() && !attr.empty() && !IsSameEntityType(type, attr)) {
			final_results = GetAlternateResult(final_results, 0, type);
		}
	}
	else {
		for (STRING_LIST* set : results) {
			for (string val : values) {
				STRING_LIST new_value = STRING_LIST();
				new_value.insert(new_value.end(), set->begin(), set->end());
				new_value.push_back(val);

				// check attribute if present
				int index_to_convert = -1;
				if (!type.empty() && !attr.empty() && !IsSameEntityType(type, attr)) {
					index_to_convert = new_value.size() - 1;
				}

				STRINGLIST_SET tmp = *(new STRINGLIST_SET());
				if (index_to_convert != -1) {
					tmp = GetAlternateResult(new_value, index_to_convert, type);
				}

				if (tmp.empty()) {
					if (!IsDuplicate(final_results, new_value)) {
						final_results.insert(new STRING_LIST(new_value));
					}
				}
				else {
					for (STRING_LIST* l : tmp) {
						if (!IsDuplicate(final_results, *l)) {
							final_results.insert(l);
						}
					}
				}
			}
		}
	}

	return final_results;
}

INTEGER_LIST PQLOptimizedEvaluator::GetOutputSynonyms(STRING_LIST synonyms, STRING_LIST output_list) {
	INTEGER_LIST index = *(new INTEGER_LIST());

	for (int i = 0; i < synonyms.size(); i++) {
		for (string output : output_list) {
			if (synonyms.at(i).compare(ParsingSynonym(output)) == 0) {
				index.push_back(i);
			}
		}
	}

	return index;
}

int PQLOptimizedEvaluator::GetSimilarClause(string to_be_added_function, string existing_function) {
	if ((to_be_added_function.compare(TYPE_COND_PARENT) == 0 && existing_function.compare(TYPE_COND_PARENT_T) == 0) ||
		(to_be_added_function.compare(TYPE_COND_FOLLOWS) == 0 && existing_function.compare(TYPE_COND_FOLLOWS_T) == 0) ||
		(to_be_added_function.compare(TYPE_COND_CALLS) == 0 && existing_function.compare(TYPE_COND_CALLS_T) == 0) ||
		(to_be_added_function.compare(TYPE_COND_NEXT) == 0 && existing_function.compare(TYPE_COND_NEXT_T) == 0) ||
		(to_be_added_function.compare(TYPE_COND_AFFECTS) == 0 && existing_function.compare(TYPE_COND_AFFECTS_T) == 0)) {
		// replace
		return -1;
	}
	else if ((to_be_added_function.compare(TYPE_COND_PARENT_T) == 0 && existing_function.compare(TYPE_COND_PARENT) == 0) ||
		(to_be_added_function.compare(TYPE_COND_FOLLOWS_T) == 0 && existing_function.compare(TYPE_COND_FOLLOWS) == 0) ||
		(to_be_added_function.compare(TYPE_COND_CALLS_T) == 0 && existing_function.compare(TYPE_COND_CALLS) == 0) ||
		(to_be_added_function.compare(TYPE_COND_NEXT_T) == 0 && existing_function.compare(TYPE_COND_NEXT) == 0) ||
		(to_be_added_function.compare(TYPE_COND_AFFECTS_T) == 0 && existing_function.compare(TYPE_COND_AFFECTS) == 0)) {
		// skip 
		return 1;
	}
	else {
		// add
		return 0;
	}
}

STATEMENT_TYPE PQLOptimizedEvaluator::GetStmtType(string entity_type) {
	if (entity_type.compare(TYPE_STMT_ASSIGN) == 0) {
		return STATEMENT_TYPE::assignStatement;
	}
	else if (entity_type.compare(TYPE_STMT_IF) == 0) {
		return STATEMENT_TYPE::ifStatement;
	}
	else if (entity_type.compare(TYPE_STMT_WHILE) == 0) {
		return STATEMENT_TYPE::whileStatement;
	}
	else if (entity_type.compare(TYPE_STMT_READ) == 0) {
		return STATEMENT_TYPE::readStatement;
	}
	else if (entity_type.compare(TYPE_STMT_PRINT) == 0) {
		return STATEMENT_TYPE::printStatement;
	}
	else if (entity_type.compare(TYPE_STMT_CALL) == 0) {
		return STATEMENT_TYPE::callStatement;
	}
	else {
		return STATEMENT_TYPE::allStatement;
	}
}

void PQLOptimizedEvaluator::RemoveIrrelevant(STRINGLIST_SET* value, STRINGLIST_SET tmp, int pos_to_check) {
	auto start = high_resolution_clock::now();
	if (PQL_DEBUG) {
		cout << "PQLOptimizedEvaluator - RemoveIrrelevant" << endl;
	}

	auto it = value->begin();
	while (it != value->end()) {
		string v = (*it)->at(pos_to_check);
		bool has_value = false;

		for (STRING_LIST* tmp_entry : tmp) {
			string check = tmp_entry->at(0);
			if (check == v) {
				has_value = true;
			}
		}

		has_value ? it++ : it = value->erase(it);
	}

	auto stop = high_resolution_clock::now();
	if (DEBUG_TIMER) {
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << "Time taken for removeirrelevant = " << duration.count() << "ms" << endl;
	}
}

bool PQLOptimizedEvaluator::IsVar(string var) {
	return IsString(var) || IsInteger(var) || IsUnderscore(var) ? false : true;
}

bool PQLOptimizedEvaluator::IsString(string var) {
	return var[0] == '\"' && var[var.length() - 1] == '\"' ? true : false;
}

bool PQLOptimizedEvaluator::IsInteger(string var) {
	return !var.empty() && find_if(var.begin(),
		var.end(), [](unsigned char c) { return !isdigit(c); }) == var.end();
}

bool PQLOptimizedEvaluator::IsUnderscore(string var) {
	return var.compare("_") == 0 ? true : false;
}

bool PQLOptimizedEvaluator::IsOutputSynonyms(STRING_LIST synonyms, STRING_LIST output_list) {
	for (string synonym : synonyms) {
		for (string output : output_list) {
			if (synonym.compare(ParsingSynonym(output)) == 0) {
				return true;
			}
		}
	}

	return false;
}

bool PQLOptimizedEvaluator::IsOutputSynonyms(STRING_SET synonyms, STRING_LIST output_list) {
	for (string output : output_list) {
		if (synonyms.find(ParsingSynonym(output)) != synonyms.end()) {
			return true;
		}
	}

	return false;
}

bool PQLOptimizedEvaluator::IsConstraintSynonym(STRING_SET synonyms, STRING_SET constraint_synonyms) {
	for (string s : synonyms) {
		if (constraint_synonyms.find(s) != constraint_synonyms.end()) {
			return true;
		}
	}

	return false;
}

bool PQLOptimizedEvaluator::IsDuplicate(STRINGLIST_SET set, STRING_LIST value) {
	for (STRING_LIST* lst : set) {
		if (*lst == value) {
			return true;
		}
	}
	return false;
}

bool PQLOptimizedEvaluator::IsDependencyStatisfied(STRING_LIST result1, STRING_LIST result2, INTEGERPAIR_SET to_check) {
	if (to_check.empty()) {
		return true;
	}
	else {
		for (INTEGER_PAIR* pair : to_check) {
			int r_index = pair->first;
			int v_index = pair->second;

			if (result1.at(r_index) != result2.at(v_index)) {
				return false;
			}
		}
	}

	return true;
}

bool PQLOptimizedEvaluator::IsSameEntityType(string type, string check) {
	STRING_SET list_of_stmts = { TYPE_STMT_CALL, TYPE_STMT_PRINT, TYPE_STMT_READ };
	if (check.empty()) {
		return true;
	}

	if (list_of_stmts.find(type) != list_of_stmts.end() && check.compare("stmt#") != 0) {
		// stmt# but asking for names
		return false;
	}

	return true;
}

int PQLOptimizedEvaluator::ParsingStmtRef(string param) {
	return param.compare("_") == 0 || param.compare("") == 0 ? -1 : stoi(param);
}

string PQLOptimizedEvaluator::ParsingEntRef(string param) {
	if (param.compare("_") == 0) {
		return "";
	}
	else {
		// trim away the quotation marks
		param.erase(remove(param.begin(), param.end(), '\"'), param.end());
		return param;
	}
}

string PQLOptimizedEvaluator::ParsingSynonym(string synonym) {
	int index = synonym.find(".");
	return index != string::npos ? synonym.substr(0, index) : synonym;
}

string PQLOptimizedEvaluator::ParsingSynonymAttribute(string synonym) {
	int index = synonym.find(".");
	return index != string::npos ? synonym.substr(index + 1, synonym.length()) : "";
}

void PQLOptimizedEvaluator::IncreaseOccurrenceCount(string target, STRING_INTEGER_MAP* occurrence_count, int count) {
	if (occurrence_count->find(target) != occurrence_count->end()) {
		if (count == 0) {
			occurrence_count->at(target) = 0;
		}
		else {
			occurrence_count->at(target) += count;
		}
	}
	else {
		occurrence_count->insert({ target, count });
	}
}