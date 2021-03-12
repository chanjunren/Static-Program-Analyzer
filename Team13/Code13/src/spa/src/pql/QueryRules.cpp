#include "QueryRules.h"
#include "WhitespaceHandler.h"
#include <sstream>
#include <regex>
#include <string>
#include <iostream>
using namespace std;

const STRING_SET valid_design_entities = { TYPE_DESIGN_ENTITY_STMT , TYPE_DESIGN_ENTITY_READ , TYPE_DESIGN_ENTITY_PRINT ,
											TYPE_DESIGN_ENTITY_CALL , TYPE_DESIGN_ENTITY_WHILE , TYPE_DESIGN_ENTITY_IF ,
											TYPE_DESIGN_ENTITY_ASSIGN , TYPE_DESIGN_ENTITY_VARIABLE , TYPE_DESIGN_ENTITY_CONSTANT ,
											TYPE_DESIGN_ENTITY_PROG_LINE , TYPE_DESIGN_ENTITY_PROCEDURE };
const STRING_SET valid_rel_ref_names = { TYPE_COND_FOLLOWS , TYPE_COND_FOLLOWS_T , TYPE_COND_FOLLOWS_STAR , TYPE_COND_PARENT ,
										TYPE_COND_PARENT_T , TYPE_COND_PARENT_STAR , TYPE_COND_USES , TYPE_COND_USES_S ,
										TYPE_COND_USES_P , TYPE_COND_MODIFIES ,  TYPE_COND_MODIFIES_S , TYPE_COND_MODIFIES_P ,
										TYPE_COND_CALLS , TYPE_COND_CALLS_T , TYPE_COND_CALLS_STAR , TYPE_COND_NEXT ,
										TYPE_COND_NEXT_T , TYPE_COND_NEXT_STAR , TYPE_COND_AFFECTS , TYPE_COND_AFFECTS_T ,
										TYPE_COND_AFFECTS_STAR };
const STRING_SET valid_stmt_ref_synonyms = { TYPE_DESIGN_ENTITY_STMT , TYPE_DESIGN_ENTITY_READ , TYPE_DESIGN_ENTITY_PRINT ,
											TYPE_DESIGN_ENTITY_CALL , TYPE_DESIGN_ENTITY_WHILE , TYPE_DESIGN_ENTITY_IF ,
											TYPE_DESIGN_ENTITY_ASSIGN };
const STRING_SET valid_ent_ref_synonyms = { TYPE_DESIGN_ENTITY_VARIABLE, TYPE_DESIGN_ENTITY_PROCEDURE };
const STRING_SET valid_line_ref_synonyms = { TYPE_DESIGN_ENTITY_PROG_LINE };
const STRING_SET valid_attr_names = { TYPE_ATTRNAME_PROCNAME , TYPE_ATTRNAME_VARNAME , TYPE_ATTRNAME_VALUE , TYPE_ATTRNAME_STMT };
const STRING_SET valid_stmt_attr_types = { TYPE_DESIGN_ENTITY_CALL , TYPE_DESIGN_ENTITY_READ , TYPE_DESIGN_ENTITY_PRINT,
										TYPE_DESIGN_ENTITY_STMT, TYPE_DESIGN_ENTITY_WHILE, TYPE_DESIGN_ENTITY_IF,
										TYPE_DESIGN_ENTITY_ASSIGN };
const STRING_SET valid_procname_attr_types = { TYPE_DESIGN_ENTITY_PROCEDURE , TYPE_DESIGN_ENTITY_CALL };
const STRING_SET valid_varname_attr_types = { TYPE_DESIGN_ENTITY_VARIABLE , TYPE_DESIGN_ENTITY_READ , TYPE_DESIGN_ENTITY_PRINT };
const STRING_SET valid_value_attr_types = { TYPE_DESIGN_ENTITY_CONSTANT };


bool QueryRules::IsIdent(string token) {
	bool result = false;
	regex expr("[A-Za-z][a-zA-Z_0-9]*");
	if (regex_match(token, expr)) {
		result = true;
	}
	return result;
}

bool QueryRules::IsName(string token) {
	bool result = false;
	regex expr("[A-Za-z][a-zA-Z_0-9]*");
	if (regex_match(token, expr)) {
		result = true;
	}
	return result;
}

bool QueryRules::IsInteger(string token) {
	bool result = false;
	regex expr("^0$|^[1-9][0-9]*$");
	if (regex_match(token, expr)) {
		result = true;
	}
	return result;
}

bool QueryRules::IsSynonym(string token) {
	bool result = false;
	if (IsIdent(token)) {
		result = true;
	}
	return result;
}

bool QueryRules::IsStmtRef(string token, string synonym_type) {
	bool result = false;
	if (valid_stmt_ref_synonyms.count(synonym_type) == 1) {
		if (IsSynonym(token)) {
			result = true;
		}
	}
	else if (token.compare("_") == 0) {
		result = true;
	}
	else if (IsInteger(token)) {
		result = true;
	}
	return result;
}

bool QueryRules::IsEntRef(string token, string synonym_type) {
	bool result = false;
	if (valid_ent_ref_synonyms.count(synonym_type) == 1) {
		if (IsSynonym(token)) {
			result = true;
		}
	}
	else if (token.compare("_") == 0) {
		result = true;
	}
	else if (token.front() == '\"' && token.back() == '\"') {
		// remember to ensure that what's inside the "" is already trimmed!
		string ident = token.substr(1, token.length() - 2);
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&ident);
		if (IsIdent(ident)) {
			result = true;
		}
	}
	return result;
}

bool QueryRules::IsLineRef(string token, string synonym_type) {
	bool result = false;
	if (valid_line_ref_synonyms.count(synonym_type) == 1) {
		if (IsSynonym(token)) {
			result = true;
		}
	}
	else if (token.compare("_") == 0) {
		result = true;
	}
	else if (IsInteger(token)) {
		result = true;
	}
	return result;
}

bool QueryRules::IsElem(string token, STRING_STRING_MAP declared_var_names) {
	bool result = false;
	if (IsSynonym(token)) {
		result = true;
	}
	string synonym_subtoken;
	string synonym_subtoken_type;
	if (token.find(".") != string::npos) {
		synonym_subtoken = token.substr(0, token.find_first_of("."));
		synonym_subtoken_type = declared_var_names.at(synonym_subtoken);
	}
	if (IsAttrRef(token, synonym_subtoken_type)) {
		result = true;
	}

	return result;
}

bool QueryRules::IsAttrName(string token) {
	bool result = false;
	if (valid_attr_names.count(token) == 1) {
		result = true;
	}
	return result;
}

bool QueryRules::IsDesignEntity(string token) {
	bool result = false;
	if (valid_design_entities.count(token) == 1) {
		result = true;
	}
	return result;
}

bool QueryRules::IsSelect(string token) {
	bool result = false;
	if (token.compare(TYPE_SELECT_CLAUSE) == 0) {
		result = true;
	}
	return result;
}

/*
	Remember to pass in "stmt s" without the semicolon.
*/
bool QueryRules::IsDeclaration(string token, STRING_STRING_MAP entity_map) {
	bool result = true;
	string temp_token = token;
	string sub_token;
	string comma = ",";
	size_t pos = 0;
	bool first_pass = true;
	STRING_SET declared_synonyms;
	// If there is a comma found (indicating multiple synonyms)
	if (temp_token.find(comma) != string::npos) {
		while ((pos = temp_token.find(comma)) != string::npos) {
			WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
			sub_token = temp_token.substr(0, temp_token.find_first_of(","));
			WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&sub_token);
			//cout << "SUBTOKEN:" << sub_token << endl;
			if (first_pass) {
				// Here should have the design-entity and synonym
				string supposed_design_entity = sub_token.substr(0, sub_token.find_first_of(" "));
				//cout << "sde:" << supposed_design_entity << "|" << endl;
				if (!IsDesignEntity(supposed_design_entity)) {
					result = false;
					break;
				}
				// Erase the design-entity
				sub_token.erase(0, sub_token.find_first_of(" "));
				temp_token.erase(0, temp_token.find_first_of(" "));
				first_pass = false;
			}
			// sub_token is now a synonym. check if is synonym
			WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&sub_token);
			//cout << "Subtoken:" << sub_token << endl;
			if (!IsSynonym(sub_token)) {
				result = false;
				break;
			}
			if (declared_synonyms.count(sub_token) != 0) {
				result = false;
				break;
			}
			if (entity_map.count(sub_token) != 0) {
				result = false;
				break;
			}

			else {
				declared_synonyms.insert(sub_token);
			}
			// Erase the validated segment from the temp_token
			// cout << "TEMPTOKEN:" << temp_token << endl;
			WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
			temp_token.erase(0, temp_token.find_first_of(",") + 1);
		}
		// Passed the last comma, this is checking for last synonym.
		// cout << "Last value:" << temp_token << endl;
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		if (!IsSynonym(temp_token) || declared_synonyms.count(temp_token) != 0) {
			result = false;
		}
		if (entity_map.count(temp_token) != 0) {
			result = false;
		}
	}
	else {
		// No comma found. One synonym declared.
		string supposed_design_entity = temp_token.substr(0, temp_token.find_first_of(" "));
		if (!IsDesignEntity(supposed_design_entity)) {
			result = false;
		}
		temp_token.erase(0, temp_token.find_first_of(" "));
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		if (!IsSynonym(temp_token)) {
			result = false;
		}
		if (entity_map.count(temp_token) != 0) {
			result = false;
			return result;
		}
	}
	if (PQL_DEBUG) {
		if (!result) {
			cout << "Error at QueryRules::IsDeclaration()" << endl;
		}
	}
	return result;
}

bool QueryRules::IsResultClause(string token, STRING_STRING_MAP declared_var_names) {
	bool result = false;
	if (IsTuple(token, declared_var_names)) {
		result = true;
	}
	/*
	else {
		cout << "tuple is not valid in result clase" << endl;
	}
	*/
	if (token.compare("BOOLEAN") == 0) {
		result = true;
	}
	return result;
}

bool QueryRules::IsTuple(string token, STRING_STRING_MAP declared_var_names) {
	bool result = false;
	string temp_token = token;
	// cout << "temp_token:" << temp_token << endl;
	string synonym_token;
	if (temp_token.find(".") != string::npos && temp_token.front() != '<' && temp_token.back() != '>') {
		synonym_token = temp_token.substr(0, temp_token.find_first_of("."));
		// cout << "found dot in elem, this is not tuple" << endl; 
	}
	// cout << "reachhere1" << endl;
	if (temp_token.front() != '<' && temp_token.back() != '>') {
		if (IsElem(temp_token, declared_var_names)) {
			// cout << "IT IS AN ELEM" << endl;
			// cout << "yes is elem" << endl;
			// element must be declared by user
			if (temp_token.find(".") != string::npos) {
				string synonym_type = declared_var_names.at(synonym_token);
				// cout << "passed synonym Type: " << synonym_type << endl;
				if (IsAttrRef(temp_token, synonym_type)) {
					// cout << "yes is attrRef" << endl;
					// there is a dot
					// cout << "synonym_st" << synonym_subtoken << endl;
					if (declared_var_names.count(synonym_token) == 1) {
						result = true;
						return result;
					}
				}
			}
			if (declared_var_names.count(token) == 1) {
				result = true;
				return result;
			}
		}
	}
	// cout << "reachhere2" << endl;
	string sub_token;
	string comma = ",";
	size_t pos = 0;
	if (temp_token.front() == '<' && temp_token.back() == '>') {
		// cout << "entered here is tuple" << endl;
		temp_token = temp_token.substr(1, temp_token.length() - 2);
		while ((pos = temp_token.find(comma)) != string::npos) {
			sub_token = temp_token.substr(0, pos);
			// cout << "subtoken:" << sub_token << "|" << endl;
			WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&sub_token);
			string synonym_subtoken;
			string synonym_subtoken_type;
			if (sub_token.find(".") != string::npos) {
				synonym_subtoken = sub_token.substr(0, sub_token.find_first_of("."));
				synonym_subtoken_type = declared_var_names.at(synonym_subtoken);
			}
			// cout << "subtoken:" << sub_token << endl;
			// cout << "synonymtype:" << synonym_subtoken_type << endl;
			// cout << "entered here but didnt enter isAttrRef" << endl;
			if (sub_token.find(".") != string::npos) {
				if (IsAttrRef(sub_token, synonym_subtoken_type)) {
					// cout << "yes it's attr ref in tuple" << endl;
					// cout << "synonym_st" << synonym_subtoken << endl;
					if (declared_var_names.count(synonym_subtoken) != 1) {
						result = false;
						return result; // synonym not declared.
					}
					else {
						result = true;
					}
				}
			}
			else if (IsSynonym(sub_token)) {
				if (declared_var_names.count(sub_token) != 1) {
					result = false;
					return result;
				}
				else {
					result = true;
				}
			}
			else {
				// if not attrRef or synonym, return false.
				result = false;
				return result;
			}
			temp_token.erase(0, temp_token.find_first_of(",") + 1);
		}
		// last token : 
		// cout << "we reach last token. " << endl;
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		// cout << "last token:" << temp_token << endl;
		string synonym_lasttoken;
		string synonym_lasttoken_type;
		if (temp_token.find(".") != string::npos) {
			synonym_lasttoken = temp_token.substr(0, temp_token.find_first_of("."));
			synonym_lasttoken_type = declared_var_names.at(synonym_lasttoken);
		}
		if (temp_token.find(".") != string::npos) {
			if (IsAttrRef(temp_token, synonym_lasttoken_type)) {
				string synonym_subtoken = temp_token.substr(0, temp_token.find_first_of("."));
				// cout << "synonym_st" << synonym_subtoken << endl;
				if (declared_var_names.count(synonym_subtoken) != 1) {
					result = false;
					return result; // synonym not declared.
				}
				else {
					result = true;
				}
			}
		}
		else if (IsSynonym(temp_token)) {
			if (declared_var_names.count(temp_token) != 1) {
				result = false;
				return result;
			}
			else {
				result = true;
			}
		}
		else {
			result = false;
			return result;
		}
	}
	if (PQL_DEBUG) {
		if (!result) {
			// cout << "Error at QueryRules::IsTuple()" << endl;
		}
	}
	return result;
}

bool QueryRules::IsWithClause(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string with_token = temp_token.substr(0, temp_token.find_first_of(" "));
	if (with_token.compare(TYPE_WITH_CLAUSE) != 0) {
		result = false;
	}
	temp_token.erase(0, temp_token.find_first_of(" "));
	WhitespaceHandler::TrimLeadingWhitespaces(&temp_token);
	if (temp_token.empty()) {
		result = false;
		return result;
	}
	string attr_cond_token = temp_token;

	if (!IsAttrCond(attr_cond_token, declared_var_names)) {
		result = false;
	}

	return result;
}

bool QueryRules::IsSuchThatClause(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	size_t second_whitespace_pos = 0;
	int count = 0;
	while (count != 2)
	{
		second_whitespace_pos += 1;
		second_whitespace_pos = temp_token.find(" ", second_whitespace_pos);
		count++;
	}
	if (second_whitespace_pos > 9) {
		second_whitespace_pos = temp_token.length() - 1;
	}
	string suchthat_token = temp_token.substr(0, second_whitespace_pos);
	if (suchthat_token.compare(TYPE_SUCH_THAT_CLAUSE) != 0) {
		result = false;
	}
	temp_token.erase(0, temp_token.find_first_of(" ") + 1); // erase such
	temp_token.erase(0, temp_token.find_first_of(" ")); // erase that
	WhitespaceHandler::TrimLeadingWhitespaces(&temp_token);
	string rel_cond_token = temp_token;

	if (!IsRelCond(rel_cond_token, declared_var_names)) {
		result = false;
	}

	//cout << "relcond:" << rel_cond_token << endl;
	return result;
}

bool QueryRules::IsPatternClause(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string pattern_token = temp_token.substr(0, temp_token.find_first_of(" "));
	if (pattern_token.compare(TYPE_COND_PATTERN) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of(" "));
	WhitespaceHandler::TrimLeadingWhitespaces(&temp_token);
	string pattern_cond_token = temp_token;

	if (!IsPatternCond(pattern_cond_token, declared_var_names)) {
		result = false;
		return result;
	}

	// cout << "patterncond:" << pattern_cond_token << endl;
	return result;
}

bool QueryRules::IsAttrCond(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	// cout << "CHECKING IF ATTRCOND.." << endl;
	string temp_token = token;
	string and_token = "and";
	size_t pos = 0;
	if (!IsAttrCompare(temp_token, declared_var_names)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsAttrCompare(string token, STRING_STRING_MAP declared_var_names) {
	// cout << "TRYING TO ATTR COMPARE.." << endl;
	// cout << "TOKEN IS:" << token << endl;
	bool result = true;
	string first_ref = token.substr(0, token.find_first_of("="));
	string second_ref = token.substr(token.find_first_of("=") + 1, token.length());
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_ref);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_ref);
	// cout << "first_ref:" << first_ref << endl;
	// cout << "second_ref:" << second_ref << endl;
	string first_ref_synonym;
	string second_ref_synonym;
	string first_ref_synonym_type = "none";
	string second_ref_synonym_type = "none";
	// cout << "ATTRCOMPARE CALLED:" << endl;
	// cout << "first_ref:" << first_ref << endl;
	string first_ref_type = "none1";
	string second_ref_type = "none2";
	if (IsSynonym(first_ref)) {
		// cout << "FIRST REF!!!" << first_ref << endl;
		if (declared_var_names.count(first_ref) == 1) {
			first_ref_synonym_type = declared_var_names.at(first_ref);
			if (first_ref_synonym_type.compare(TYPE_DESIGN_ENTITY_PROG_LINE) != 0) {
				result = false;
				return result;
			}
		}
		else {
			// synonym not declared
			result = false;
			return result;
		}
		// it is a progline
		first_ref_type = TYPE_WITH_ARGUMENT_INTEGER;
	}
	if (IsSynonym(second_ref)) {
		// cout << "YES SECOND REF IS SYNOMYM:" << second_ref << endl;
		if (declared_var_names.count(second_ref) == 1) {
			second_ref_synonym_type = declared_var_names.at(second_ref);
			if (second_ref_synonym_type.compare(TYPE_DESIGN_ENTITY_PROG_LINE) != 0) {
				result = false;
				return result;
			}
		}
		else {
			// synonym not declared
			result = false;
			return result;
		}
		second_ref_type = TYPE_WITH_ARGUMENT_INTEGER;
	}
	if (first_ref.find(".") != string::npos) {
		first_ref_synonym = first_ref.substr(0, first_ref.find_first_of("."));
		if (declared_var_names.count(first_ref_synonym) != 0) {
			first_ref_synonym_type = declared_var_names.at(first_ref_synonym);
		}
		if (!IsRef(first_ref, first_ref_synonym_type)) {
			result = false;
			return result;
		}
		string first_ref_attr_ref_type = first_ref.substr(first_ref.find_first_of(".") + 1, first_ref.length());
		// cout << "1st attrref type:" << first_ref_attr_ref_type << endl;
		if (first_ref_attr_ref_type.compare(TYPE_ATTRNAME_STMT) == 0 || first_ref_attr_ref_type.compare(TYPE_ATTRNAME_VALUE) == 0) {
			first_ref_type = TYPE_WITH_ARGUMENT_INTEGER;
		}
		if (first_ref_attr_ref_type.compare(TYPE_ATTRNAME_PROCNAME) == 0 || first_ref_attr_ref_type.compare(TYPE_ATTRNAME_VARNAME) == 0) {
			first_ref_type = TYPE_WITH_ARGUMENT_STRING;
		}
	}
	else if (IsInteger(first_ref)) {
		// cout << "YES FIRST REF IS INT" << endl;
		first_ref_type = TYPE_WITH_ARGUMENT_INTEGER;
	}
	else if (first_ref.length() != 0) {
		if (first_ref.front() == '\"' && first_ref.back() == '\"') {
			string ident = first_ref.substr(1, first_ref.length() - 2);
			//cout << "IDENT IS:" << ident << endl;
			if (IsIdent(ident)) {
				first_ref_type = TYPE_WITH_ARGUMENT_STRING;
			}
			else {
				result = false;
				return result;
			}
		}
	}
	else if (first_ref.length() == 0) {
		result = false;
		return result;
	}
	if (second_ref.find(".") != string::npos) {
		second_ref_synonym = second_ref.substr(0, second_ref.find_first_of("."));
		if (declared_var_names.count(second_ref_synonym) != 0) {
			second_ref_synonym_type = declared_var_names.at(second_ref_synonym);
		}
		if (!IsRef(second_ref, second_ref_synonym_type)) {
			result = false;
			return result;
		}
		string second_ref_attr_ref_type = second_ref.substr(second_ref.find_first_of(".") + 1, second_ref.length());
		//cout << "2nd attrref type:" << second_ref_attr_ref_type << endl;
		if (second_ref_attr_ref_type.compare(TYPE_ATTRNAME_STMT) == 0 || second_ref_attr_ref_type.compare(TYPE_ATTRNAME_VALUE) == 0) {
			second_ref_type = TYPE_WITH_ARGUMENT_INTEGER;
		}
		if (second_ref_attr_ref_type.compare(TYPE_ATTRNAME_PROCNAME) == 0 || second_ref_attr_ref_type.compare(TYPE_ATTRNAME_VARNAME) == 0) {
			second_ref_type = TYPE_WITH_ARGUMENT_STRING;
		}
	}
	else if (IsInteger(second_ref)) {
		// cout << "YES SECOND REF IS INT" << endl;
		second_ref_type = TYPE_WITH_ARGUMENT_INTEGER;
		result = true;
	}
	else if (second_ref.length() != 0) {
			if (second_ref.front() == '\"' && second_ref.back() == '\"') {
			string ident = second_ref.substr(1, second_ref.length() - 2);
			// cout << "ident is :" << ident << endl;
			if (IsIdent(ident)) {
				second_ref_type = TYPE_WITH_ARGUMENT_STRING;
			}
			else {
				result = false;
				return result;
			}
		}
	}
	else if (second_ref.length() == 0) {
		result = false;
		return result;
	}
	// cout << "FIRST_ARG_TYPE:" << first_ref_type << endl;
	// cout << "SECOND_ARG_TYPE:" << second_ref_type << endl;
	if (first_ref_type.compare(second_ref_type) != 0) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsRef(string token, string synonym_type) {
	bool result = false;
	if (token.length() == 0) {
		result = false;
		return result;
	}
	if (token.front() == '\"' && token.back() == '\"') {
		// remember to ensure that what's inside the "" is already trimmed!
		string ident = token.substr(1, token.length() - 2);
		if (IsIdent(ident)) {
			result = true;
		}
	}
	if (IsInteger(token)) {
		result = true;
	}
	if (token.find(".") != string::npos) {
		// cout << "token is :" << token << endl;
		// cout << "syntype: " << synonym_type << endl;
		if (IsAttrRef(token, synonym_type)) {
			// cout << "checking if:" << token << " is an attr ref" << endl;
			// cout << "its type:" << synonym_type << endl;
			result = true;
			// cout << "result:" << result << endl;
		}
	}
	if (IsSynonym(token) && synonym_type.compare("prog_line") == 0) {
		result = true;
	}
	return result;
}

bool QueryRules::IsAttrRef(string token, string synonym_type) {
	bool result = true;
	size_t pos_of_fullstop = token.find(".");
	string synonym_token = token.substr(0, pos_of_fullstop);
	string attr_name_token = token.substr(pos_of_fullstop + 1, token.length() - 1);
	if (!IsSynonym(synonym_token)) {
		result = false;
		return result;
	}
	if (!IsAttrName(attr_name_token)) {
		result = false;
		return result;
	}
	if (attr_name_token.compare(TYPE_ATTRNAME_PROCNAME) == 0) {
		// 2nd is procName
		if (valid_procname_attr_types.count(synonym_type) == 0) {
			// first synonym must be type procedure or call
			result = false;
			return result;
		}
	}
	if (attr_name_token.compare(TYPE_ATTRNAME_VARNAME) == 0) {
		// 2nd is varname
		if (valid_varname_attr_types.count(synonym_type) == 0) {
			result = false;
			return result;
		}
	}
	if (attr_name_token.compare(TYPE_ATTRNAME_STMT) == 0) {
		if (valid_stmt_attr_types.count(synonym_type) == 0) {
			result = false;
			return result;
		}
	}
	if (attr_name_token.compare(TYPE_ATTRNAME_VALUE) == 0) {
		if (valid_value_attr_types.count(synonym_type) == 0) {
			result = false;
			return result;
		}
	}

	return result;
}

bool QueryRules::IsRelCond(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
	if (!IsRelRef(temp_token, declared_var_names)) {
		result = false;
		return result;
	}
	/*
	string and_token = "and";
	size_t pos = 0;
	while ((pos = temp_token.find(and_token)) != string::npos) {
		// while we can find 'and'
		cout << "found AND: " << endl;
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		string supposed_rel_ref = temp_token.substr(0, temp_token.find_first_of("and"));
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&supposed_rel_ref);
		cout << "supposed_rel_ref:" << supposed_rel_ref << "|" << endl;
		temp_token.erase(0, temp_token.find_first_of("and") + 3);
		if (!IsRelRef(supposed_rel_ref, declared_var_names)) {
			result = false;
			return result;
		}
	}
	*/
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token); // no and only one clause, OR and is erased until last clause

	// cout << "relCond temptoken:" << temp_token << endl;
	if (!IsRelRef(temp_token, declared_var_names)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsRelRef(string token, STRING_STRING_MAP declared_var_names) {
	bool result = false;
	//cout << "THE RELREFCHECKING IS:" << token << endl;
	string removed_relref_cond = token.substr(token.find_first_of("("), token.length());
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&removed_relref_cond);
	//cout << "REMOVED RELREF COND:" << removed_relref_cond << endl;

	string first_arg = removed_relref_cond.substr(0, removed_relref_cond.find_first_of(","));
	//cout << "his length:" << removed_relref_cond.length() << endl;
	string second_arg = removed_relref_cond.substr(removed_relref_cond.find_first_of(",") + 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	//cout << "FIRST_ARG:" << first_arg << "|" << endl;
	//cout << "SECOND_ARG:" << second_arg << "|" << endl;
	if (first_arg.compare("(") == 0) {
		// first arg empty
		// cout << "first arg empty." << endl;
		result = false;
		return result;
	}
	if (second_arg.compare(")") == 0) {
		// second arg empty
		// cout << "second arg empty." << endl;
		result = false;
		return result;
	}

	if (IsModifiesP(token, declared_var_names)) {
		result = true;
		// cout << "It is ModifiesP" << endl;
		return result;
	}
	if (IsModifiesS(token, declared_var_names)) {
		result = true;
		// cout << "It is ModifiesS" << endl;
		return result;
	}
	if (IsUsesP(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsUsesS(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsCalls(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsCallsT(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsParent(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsParentT(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsFollows(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsFollowsT(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsNext(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsNextT(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsAffects(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsAffectsT(token, declared_var_names)) {
		result = true;
		return result;
	}
	return result;
}

string QueryRules::GetRelRefType(string token, STRING_STRING_MAP declared_var_names) {
	string type = "none";
	if (IsModifiesP(token, declared_var_names)) {
		type = TYPE_COND_MODIFIES_P;
		// cout << "It is ModifiesP" << endl;
		return type;
	}
	if (IsModifiesS(token, declared_var_names)) {
		type = TYPE_COND_MODIFIES_S;
		// cout << "It is ModifiesS" << endl;
		return type;
	}
	if (IsUsesP(token, declared_var_names)) {
		type = TYPE_COND_USES_P;
		return type;
	}
	if (IsUsesS(token, declared_var_names)) {
		type = TYPE_COND_USES_S;
		return type;
	}
	if (IsCalls(token, declared_var_names)) {
		type = TYPE_COND_CALLS;
		return type;
	}
	if (IsCallsT(token, declared_var_names)) {
		type = TYPE_COND_CALLS_T;
		return type;
	}
	if (IsParent(token, declared_var_names)) {
		type = TYPE_COND_PARENT;
		return type;
	}
	if (IsParentT(token, declared_var_names)) {
		type = TYPE_COND_PARENT_T;
		return type;
	}
	if (IsFollows(token, declared_var_names)) {
		type = TYPE_COND_FOLLOWS;
		return type;
	}
	if (IsFollowsT(token, declared_var_names)) {
		type = TYPE_COND_FOLLOWS_T;
		return type;
	}
	if (IsNext(token, declared_var_names)) {
		type = TYPE_COND_NEXT;
		return type;
	}
	if (IsNextT(token, declared_var_names)) {
		type = TYPE_COND_NEXT_T;
		return type;
	}
	if (IsAffects(token, declared_var_names)) {
		type = TYPE_COND_AFFECTS;
		return type;
	}
	if (IsAffectsT(token, declared_var_names)) {
		type = TYPE_COND_AFFECTS_T;
		return type;
	}
	return type;
}


bool QueryRules::IsModifiesP(string token, STRING_STRING_MAP declared_var_names) {
	/*
		Token here should be something like Modifies (p, "x")
		Trailing and leading whitespace should be trimmed before passing in
	*/
	bool result = true;
	string temp_token = token;
	string modifies_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&modifies_token);
	string first_arg;
	string second_arg;
	if (modifies_token.compare(TYPE_COND_MODIFIES) != 0) {
		result = false;
		return result;
	}
	// Erase Modifies away
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.length() != 0) {
		if (temp_token.front() != '(' || temp_token.back() != ')') {
			result = false;
			return result;
		}
	}
	else {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef , cannot be underscore
	if (first_arg.compare("_") == 0) {
		result = false;
		return result;
	}
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
		if (first_arg_type.compare(TYPE_DESIGN_ENTITY_VARIABLE) == 0) {
			result = false;
			return result;
		}
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef cannot be procedure
	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
		if (second_arg_type.compare(TYPE_DESIGN_ENTITY_PROCEDURE) == 0) {
			result = false;
			return result;
		}
	}
	if (!IsEntRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsModifiesS(string token, STRING_STRING_MAP declared_var_names) {
	/*
		Token here should be something like Modifies (s, "x")
		Trailing and leading whitespace should be trimmed before passing in
	*/
	bool result = true;
	string temp_token = token;
	string modifies_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&modifies_token);
	string first_arg;
	string second_arg;
	if (modifies_token.compare(TYPE_COND_MODIFIES) != 0) {
		result = false;
		return result;
	}
	// Erase Modifies away
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef , cannot be underscore
	if (first_arg.compare("_") == 0) {
		result = false;
		return result;
	}
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	// first arg of modifiesS cannot be print
	if (first_arg_type.compare(TYPE_DESIGN_ENTITY_PRINT) == 0) {
		result = false;
		return result;
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef and cannot be procedure
	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
		if (second_arg_type.compare(TYPE_DESIGN_ENTITY_PROCEDURE) == 0) {
			result = false;
			return result;
		}
	}
	if (!IsEntRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsUsesP(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string uses_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&uses_token);
	string first_arg;
	string second_arg;
	if (uses_token.compare(TYPE_COND_USES) != 0) {
		result = false;
		return result;
	}
	// Erase Uses away
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef , cannot be underscore
	if (first_arg.compare("_") == 0) {
		result = false;
		return result;
	}
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
		if (first_arg_type.compare(TYPE_DESIGN_ENTITY_VARIABLE) == 0) {
			result = false;
			return result;
		}
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef 
	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
		if (second_arg_type.compare(TYPE_DESIGN_ENTITY_PROCEDURE) == 0) {
			result = false;
			return result;
		}
	}
	if (!IsEntRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsUsesS(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string uses_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&uses_token);
	string first_arg;
	string second_arg;
	if (uses_token.compare(TYPE_COND_USES) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef , cannot be underscore
	if (first_arg.compare("_") == 0) {
		result = false;
		return result;
	}
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	// first arg cannot be read
	if (first_arg_type.compare(TYPE_DESIGN_ENTITY_READ) == 0) {
		result = false;
		return result;
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef & cannot be procedure
	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
		if (second_arg_type.compare(TYPE_DESIGN_ENTITY_PROCEDURE) == 0) {
			result = false;
			return result;
		}
	}
	if (!IsEntRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsCalls(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string calls_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&calls_token);
	string first_arg;
	string second_arg;
	if (calls_token.compare(TYPE_COND_CALLS) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
		if (first_arg_type.compare(TYPE_DESIGN_ENTITY_VARIABLE) == 0) {
			result = false;
			return result;
		}
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
		if (second_arg_type.compare(TYPE_DESIGN_ENTITY_VARIABLE) == 0) {
			result = false;
			return result;
		}
	}
	if (!IsEntRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsCallsT(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string calls_star_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&calls_star_token);
	string first_arg;
	string second_arg;
	if (calls_star_token.compare(TYPE_COND_CALLS_STAR) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
		if (first_arg_type.compare(TYPE_DESIGN_ENTITY_VARIABLE) == 0) {
			result = false;
			return result;
		}
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
		if (second_arg_type.compare(TYPE_DESIGN_ENTITY_VARIABLE) == 0) {
			result = false;
			return result;
		}
	}
	if (!IsEntRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsParent(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string parent_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&parent_token);
	string first_arg;
	string second_arg;
	if (parent_token.compare(TYPE_COND_PARENT) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsStmtRef(second_arg, second_arg_type) && !IsLineRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsParentT(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string parent_star_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&parent_star_token);
	string first_arg;
	string second_arg;
	if (parent_star_token.compare(TYPE_COND_PARENT_STAR) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsStmtRef(second_arg, second_arg_type) && !IsLineRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsFollows(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string follows_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&follows_token);
	string first_arg;
	string second_arg;
	if (follows_token.compare(TYPE_COND_FOLLOWS) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;

	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}

	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsStmtRef(second_arg, second_arg_type) && !IsLineRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsFollowsT(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string follows_star_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&follows_star_token);
	string first_arg;
	string second_arg;
	if (follows_star_token.compare(TYPE_COND_FOLLOWS_STAR) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsStmtRef(second_arg, second_arg_type) && !IsLineRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsNext(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string next_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&next_token);
	string first_arg;
	string second_arg;
	if (next_token.compare(TYPE_COND_NEXT) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsLineRef(first_arg, first_arg_type) && !IsStmtRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsLineRef(second_arg, second_arg_type) && !IsStmtRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsNextT(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string next_star_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&next_star_token);
	string first_arg;
	string second_arg;
	if (next_star_token.compare(TYPE_COND_NEXT_STAR) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsLineRef(first_arg, first_arg_type) && !IsStmtRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsLineRef(second_arg, second_arg_type) && !IsStmtRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsAffects(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string parent_star_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&parent_star_token);
	string first_arg;
	string second_arg;
	if (parent_star_token.compare(TYPE_COND_AFFECTS) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
		if (first_arg_type.compare(TYPE_DESIGN_ENTITY_ASSIGN) != 0 && first_arg_type.compare(TYPE_DESIGN_ENTITY_STMT) != 0
			&& first_arg_type.compare(TYPE_DESIGN_ENTITY_PROG_LINE) != 0) {
			result = false;
			return result;
		}
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
		if (second_arg_type.compare(TYPE_DESIGN_ENTITY_ASSIGN) != 0 && second_arg_type.compare(TYPE_DESIGN_ENTITY_STMT) != 0
			&& second_arg_type.compare(TYPE_DESIGN_ENTITY_PROG_LINE) != 0) {
			result = false;
			return result;
		}
	}
	if (!IsStmtRef(second_arg, second_arg_type) && !IsLineRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsAffectsT(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string parent_star_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&parent_star_token);
	string first_arg;
	string second_arg;
	if (parent_star_token.compare(TYPE_COND_AFFECTS_STAR) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
		if (first_arg_type.compare(TYPE_DESIGN_ENTITY_ASSIGN) != 0 && first_arg_type.compare(TYPE_DESIGN_ENTITY_STMT) != 0
			&& first_arg_type.compare(TYPE_DESIGN_ENTITY_PROG_LINE) != 0) {
			result = false;
			return result;
		}
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	string second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
		if (second_arg_type.compare(TYPE_DESIGN_ENTITY_ASSIGN) != 0 && second_arg_type.compare(TYPE_DESIGN_ENTITY_STMT) != 0
			&& second_arg_type.compare(TYPE_DESIGN_ENTITY_PROG_LINE) != 0) {
			result = false;
			return result;
		}
	}
	if (!IsStmtRef(second_arg, second_arg_type) && !IsLineRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

/*
Pattern clause rules here
Select a pattern a ( _ , �v + x * y + z * t�)
*/

bool QueryRules::IsPatternCond(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	bool first_passed = false;
	while (temp_token.find_first_not_of(' ') != string::npos && !temp_token.empty()) {
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		/* if (first_passed) {
			string and_token = temp_token.substr(0, temp_token.find_first_of(" "));
			// cout << "AND TOKEN:" << and_token << endl;
			if (and_token.compare("and") != 0) {
				result = false;
				return result;
			}
			temp_token.erase(0, temp_token.find_first_of(" "));
		}*/
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		string pattern_token = temp_token;
		// cout << "patterntoken:" << pattern_token << endl;
		if (!IsPattern(pattern_token, declared_var_names)) {
			result = false;
			return result;
		}
		temp_token.erase(0, temp_token.length());
		first_passed = true;
	}
	return result;
}

bool QueryRules::IsPattern(string token, STRING_STRING_MAP declared_var_names) {
	bool result = false;
	if (IsAssign(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsSynWhile(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsSynIf(token, declared_var_names)) {
		result = true;
		return result;
	}
	return result;
}

bool QueryRules::IsAssign(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string syn_assign_token;
	string syn_assign_token_type = "none";
	syn_assign_token = temp_token.substr(0, temp_token.find_first_of("("));
	// cout << "synassigntoken:" << syn_assign_token << endl;
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&syn_assign_token);
	// syn-assign must be of type assign
	if (declared_var_names.count(syn_assign_token) != 0) {
		// syn-assign token is declared
		syn_assign_token_type = declared_var_names.at(syn_assign_token);
	}
	// cout << "synassign tt:" << syn_assign_token_type << endl;
	if (syn_assign_token_type != TYPE_DESIGN_ENTITY_ASSIGN) {
		result = false;
		return result;
	}
	string first_arg;
	string second_arg;
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);

	if (first_arg.empty()) {
		result = false;
		return result;
	}

	if (second_arg.empty()) {
		result = false;
		return result;
	}

	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
		if (first_arg_type.compare(TYPE_DESIGN_ENTITY_PROCEDURE) == 0) {
			result = false;
			return result;
		}
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	if (!IsExpressionSpec(second_arg)) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsExpressionSpec(string token) {
	bool result = false;
	string temp_token = token;
	if (temp_token.compare("_") == 0) {
		result = true;
		return result;
	}
	if (temp_token.front() == '\"' && temp_token.back() == '\"') {
		if (temp_token.length() <= 2) {
			result = false;
			return result;
		}
		temp_token = temp_token.substr(1, temp_token.length() - 2);
		if (IsExpr(temp_token)) {
			result = true;
			return result;
		}
	}
	if (temp_token.front() == '_' && temp_token.back() == '_') {
		if (temp_token.length() <= 4) {
			result = false;
			return result;
		}
		temp_token = temp_token.substr(1, temp_token.length() - 2);
		// cout << "TEMP TOKEN IS:" << temp_token << "|" << endl;
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		if (temp_token.front() == '\"' && temp_token.back() == '\"') {
			temp_token = temp_token.substr(1, temp_token.length() - 2);
			if (IsExpr(temp_token)) {
				result = true;
				return result;
			}
		}
	}
	return result;
}

bool QueryRules::IsExpr(string token) {
	bool result = true;
	string expression_token = token;
	string empty_expr_check = token;

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&empty_expr_check);

	if (empty_expr_check.empty()) {
		result = false;
		return result;
	}

	int open_bracket_count = 0;
	bool prev_was_operator = false;
	bool prev_was_open_bracket = false;
	bool prev_is_term = false;
	bool prev_is_whitespace = false;
	bool prev_is_zero = false;



	if (IsOperator(expression_token.front()) || IsOperator(expression_token.back())) {
		result = false;
		return result;
	}

	for (char const& c : expression_token) {
		if (c == ' ') {
			prev_is_whitespace = true;
			prev_is_zero = false;
			continue;
		}
		if (c == '(') {
			if (prev_is_term) {
				result = false;
				return result;
			}
			open_bracket_count++;
			prev_was_open_bracket = true;
			prev_is_term = false;
			prev_is_whitespace = false;
			prev_is_zero = false;
			continue;
		}
		else if (c == ')') {
			if (open_bracket_count <= 0 || prev_was_operator) {
				result = false;
				return result;
			}
			prev_is_term = false;
			prev_is_whitespace = false;
			open_bracket_count--;
			prev_is_zero = false;
		}
		else if (IsOperator(c)) {
			if (prev_was_operator || prev_was_open_bracket) {
				result = false;
				return result;
			}
			prev_was_operator = true;
			prev_is_term = false;
			prev_is_whitespace = false;
			prev_is_zero = false;
			continue;
		}
		else if (!isalpha(c) && !isdigit(c)) {
			result = false;
			return result;
		}
		else if (isalpha(c) || isdigit(c)) {
			if (prev_is_zero && isdigit(c)) {
				// to catch e.g. 009. 09 should be error too right.
				result = false;
				return result;
			}
			if (prev_is_whitespace && prev_is_term) {
				// to catch like "10 10"
				result = false;
				return result;
			}
			else {
				prev_is_term = true;
				prev_is_whitespace = false;
				if (c == '0') {
					prev_is_zero = true;
				}
				else {
					prev_is_zero = false;
				}
			}
		}
		prev_was_operator = false;
		prev_was_open_bracket = false;
	}

	if (prev_was_operator || prev_was_open_bracket) {
		result = false;
		return result;
	}
	if (open_bracket_count != 0) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsOperator(char c) {
	bool result = false;

	switch (c) {
	case '+':
		result = true;
	case '/':
		result = true;
	case '-':
		result = true;
	case '%':
		result = true;
	case '*':
		result = true;
	}

	return result;
}

bool QueryRules::IsTerm(string token) {
	bool result = false;
	string temp_token = token;
	if (IsFactor(token)) {
		result = true;
		return result;
	}
	regex operator_regex("[^*/%]+");
	smatch sm;
	regex_search(temp_token, sm, operator_regex);
	int pos = sm.position();
	// cout << "POs:" << pos << endl;
	string term_token = temp_token.substr(0, pos);
	string factor_token = temp_token.substr(pos + 1, temp_token.length() - 1);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&term_token);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&factor_token);
	// cout << "TermToken:" << term_token << endl;
	// cout << "factorToken:" << factor_token << endl;

	return result;
}

bool QueryRules::IsFactor(string token) {
	bool result = false;
	if (IsVarName(token)) {
		result = true;
		return result;
	}
	if (IsConstValue(token)) {
		result = true;
		return result;
	}
	// if expr
	return result;
}

bool QueryRules::IsVarName(string token) {
	return IsName(token);
}

bool QueryRules::IsConstValue(string token) {
	return IsInteger(token);
}

bool QueryRules::IsSynIf(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string syn_if_token;
	string syn_if_token_type = "none";
	syn_if_token = temp_token.substr(0, temp_token.find_first_of("("));
	// cout << "synassigntoken:" << syn_assign_token << endl;
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&syn_if_token);
	// syn-assign must be of type assign
	if (declared_var_names.count(syn_if_token) != 0) {
		// syn-assign token is declared
		syn_if_token_type = declared_var_names.at(syn_if_token);
	}
	if (syn_if_token_type != TYPE_DESIGN_ENTITY_IF) {
		result = false;
		return result;
	}
	string first_arg;
	string second_arg;
	string third_arg;
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	third_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&third_arg);

	if (first_arg.empty()) {
		result = false;
		return result;
	}

	if (second_arg.empty()) {
		result = false;
		return result;
	}

	if (third_arg.empty()) {
		result = false;
		return result;
	}

	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// cout << "3rd arg:" << third_arg << "|" << endl;
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
		// first arg cannot be proc.
		if (first_arg_type.compare(TYPE_DESIGN_ENTITY_PROCEDURE) == 0) {
			result = false; 
			return result;
		}
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	if (second_arg.compare("_") != 0) {
		result = false;
		return result;
	}
	if (third_arg.compare("_") != 0) {
		result = false;
		return result;
	}
	return result;
}

bool QueryRules::IsSynWhile(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string syn_while_token;
	string syn_while_token_type = "none";
	syn_while_token = temp_token.substr(0, temp_token.find_first_of("("));
	// cout << "synassigntoken:" << syn_assign_token << endl;
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&syn_while_token);
	// syn-assign must be of type assign
	if (declared_var_names.count(syn_while_token) != 0) {
		// syn-assign token is declared
		syn_while_token_type = declared_var_names.at(syn_while_token);
	}
	if (syn_while_token_type != TYPE_DESIGN_ENTITY_WHILE) {
		result = false;
		return result;
	}
	string first_arg;
	string second_arg;
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);

	if (first_arg.empty()) {
		result = false;
		return result;
	}

	if (second_arg.empty()) {
		result = false;
		return result;
	}
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// cout << "3rd arg:" << third_arg << "|" << endl;
	string first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
		if (first_arg_type.compare(TYPE_DESIGN_ENTITY_PROCEDURE) == 0) {
			result = false;
			return result;
		}
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	if (second_arg.compare("_") != 0) {
		result = false;
		return result;
	}
	return result;
}