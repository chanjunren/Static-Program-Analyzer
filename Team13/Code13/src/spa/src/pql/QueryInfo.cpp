#include "QueryInfo.h"

#include <iostream>

/* getters */
STRING_STRINGLISTLIST_MAP QueryInfo::GetStMap() {
	return this->st_map_;
}

STRING_STRINGLISTLIST_MAP QueryInfo::GetPatternMap() {
	return this->pattern_map_;
}

STRINGPAIR_SET QueryInfo::GetWithMap() {
	return this->with_map_;
}

STRING_STRING_MAP QueryInfo::GetEntityMap() {
	return this->entity_map_;
}

STRING_LIST QueryInfo::GetOutputList() {
	return this->output_list_;
}

STRINGLIST_SET QueryInfo::GetFalseResult() {
	STRINGLIST_SET false_result = *(new STRINGLIST_SET());
	if (this->is_invalid_due_to_semantics_ && IsBooleanOutput()) {
		STRING_LIST* bool_false = new STRING_LIST();
		bool_false->push_back("FALSE");
		false_result.insert(bool_false);
	}

	return  false_result;
}

bool QueryInfo::IsQueryInfoValid() {
	return this->is_valid_ && !this->is_invalid_due_to_semantics_;
}

bool QueryInfo::IsBooleanOutput() {
	return this->output_list_.size() == 1 && this->output_list_.at(0).compare("BOOLEAN") == 0 ? true : false;
}

bool QueryInfo::IsSemanticsInvalid() {
	return this->is_invalid_due_to_semantics_;
}

/* setters */
void QueryInfo::SetStMap(STRING_STRINGLISTLIST_MAP rel_ref_map) {
	this->st_map_ = rel_ref_map;
}

void QueryInfo::SetPatternMap(STRING_STRINGLISTLIST_MAP pattern_map) {
	this->pattern_map_ = pattern_map;
}

void QueryInfo::SetWithMap(STRINGPAIR_SET with_map) {
	this->with_map_ = with_map;
}

void QueryInfo::SetEntityMap(STRING_STRING_MAP var_map) {
	this->entity_map_ = var_map;
}

void QueryInfo::SetOutputList(STRING_LIST output_var) {
	this->output_list_ = output_var;
}

void QueryInfo::SetValidToFalse() {
	this->is_valid_ = false;
}

void QueryInfo::SetInvalidDueToSemanticsTrue() {
	this->is_invalid_due_to_semantics_ = true;
}

void QueryInfo::SetInvalidDueToSemanticsFalse() {
	this->is_invalid_due_to_semantics_ = false;
}

// test print functions
void QueryInfo::PrintClausesMap() {
	cout << "----- Clauses ----- " << endl;
	PrintStMap();
	PrintPatternMap();
	PrintWithMap();
	cout << "-------------------- " << endl;
}

void QueryInfo::PrintPatternMap() {
	cout << "----- Pattern Clauses ----- " << endl;
	for (auto const& pair : this->pattern_map_) {
		cout << " { " << pair.first << " , ";
		for (STRING_LIST v : pair.second) {
			cout << "< ";
			for (auto i : v) {
				cout << i << " ";
			}
			cout << ">";
		}
		cout << endl;
	}
	cout << "-------------------- " << endl;
}

void QueryInfo::PrintStMap() {
	cout << "----- St Clauses ----- " << endl;
	for (auto const& pair : this->st_map_) {
		cout << pair.first << ": ";
		for (STRING_LIST v : pair.second) {
			cout << "< ";
			for (auto i : v) {
				cout << i << " ";
			}
			cout << ">";
		}
		cout << endl;
	}
	cout << "-------------------- " << endl;
}

void QueryInfo::PrintWithMap() {
	cout << "----- With Clauses ----- " << endl;
	for (auto pair : this->with_map_) {
		cout << "< " << pair->first << " " << pair->second << " >" << endl;
	}
	cout << "-------------------- " << endl;
}

void QueryInfo::PrintEntityMap() {
	cout << "----- ENTITY MAP ----- " << endl;
	for (auto f = entity_map_.cbegin(); f != entity_map_.cend(); f++) {
		cout << " { " << (*f).first << " , " << (*f).second << " } " << endl;
	}
	cout << "-------------------- " << endl;
}

void QueryInfo::PrintOutputList() {
	cout << "---- OUTPUT VAR ---- " << endl;
	for (string s : output_list_) {
		cout << s << " " << endl;
	}
	cout << "-------------------- " << endl;
}
