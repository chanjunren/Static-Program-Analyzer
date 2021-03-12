#ifndef _PQL_CUSTOMTYPES_H_
#define _PQL_CUSTOMTYPES_H_

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>

using namespace std;

// Activates debug prints
#define PQL_DEBUG false
// Activates debug prints with detailed printing (data structures)
#define PQL_DEBUG_PRINTING false
// Activates fake data for unit testing
#define PQL_UNIT_TESTING false
// Activates optimized evaluator
#define OPTIMIZED_EVALUATION true
// Actives timer
#define DEBUG_TIMER true

#define TYPE_COND_FOLLOWS "Follows"
#define TYPE_COND_FOLLOWS_T "FollowsT"
#define TYPE_COND_FOLLOWS_STAR "Follows*"
#define TYPE_COND_PARENT "Parent"
#define TYPE_COND_PARENT_T "ParentT"
#define TYPE_COND_PARENT_STAR "Parent*"
#define TYPE_COND_USES "Uses"
#define TYPE_COND_USES_S "UsesS"
#define TYPE_COND_USES_P "UsesP"
#define TYPE_COND_MODIFIES "Modifies"
#define TYPE_COND_MODIFIES_S "ModifiesS"
#define TYPE_COND_MODIFIES_P "ModifiesP"
#define TYPE_COND_PATTERN "pattern"
#define TYPE_COND_PATTERN_P "pattern_p"
#define TYPE_COND_PATTERN_F "pattern_f"

#define TYPE_COND_CALLS "Calls"
#define TYPE_COND_CALLS_STAR "Calls*"
#define TYPE_COND_CALLS_T "CallsT"
#define TYPE_COND_NEXT "Next"
#define TYPE_COND_NEXT_STAR "Next*"
#define TYPE_COND_NEXT_T "NextT"
#define TYPE_COND_AFFECTS "Affects"
#define TYPE_COND_AFFECTS_STAR "Affects*"
#define TYPE_COND_AFFECTS_T "AffectsT"

#define TYPE_DESIGN_ENTITY_STMT "stmt"
#define TYPE_DESIGN_ENTITY_READ "read"
#define TYPE_DESIGN_ENTITY_PRINT "print"
#define TYPE_DESIGN_ENTITY_CALL "call"
#define TYPE_DESIGN_ENTITY_WHILE "while"
#define TYPE_DESIGN_ENTITY_IF "if"
#define TYPE_DESIGN_ENTITY_ASSIGN "assign"
#define TYPE_DESIGN_ENTITY_VARIABLE "variable"
#define TYPE_DESIGN_ENTITY_CONSTANT "constant"
#define TYPE_DESIGN_ENTITY_PROG_LINE "prog_line"
#define TYPE_DESIGN_ENTITY_PROCEDURE "procedure"

#define TYPE_ATTRNAME_PROCNAME "procName"
#define TYPE_ATTRNAME_VARNAME "varName"
#define TYPE_ATTRNAME_VALUE "value"
#define TYPE_ATTRNAME_STMT "stmt#"

#define TYPE_WITH_ARGUMENT_INTEGER "integer"
#define TYPE_WITH_ARGUMENT_STRING "string"

#define TYPE_SUCH_THAT_CLAUSE "such that"
#define TYPE_WITH_CLAUSE "with"
#define TYPE_PATTERN_CLAUSE "pattern"
#define TYPE_SELECT_CLAUSE "Select"
#define TYPE_AND_CLAUSE "and"

typedef pair<string, string> STRING_PAIR;
typedef pair<int, int> INTEGER_PAIR;

typedef vector<string> STRING_LIST;
typedef vector<int> INTEGER_LIST;
typedef vector<STRING_LIST> STRINGLIST_LIST;
typedef vector<INTEGER_LIST> INTEGERLIST_LIST;
typedef vector<STRING_PAIR> STRINGPAIR_LIST;
typedef vector<INTEGER_PAIR> INTEGERPAIR_LIST;

typedef unordered_set<string> STRING_SET;
typedef unordered_set<int> INTEGER_SET;
typedef unordered_set<double> DOUBLE_SET;
typedef unordered_set<STRING_LIST*> STRINGLIST_SET;
typedef unordered_set<STRING_PAIR*> STRINGPAIR_SET;
typedef unordered_set<INTEGER_PAIR*> INTEGERPAIR_SET;

typedef unordered_map<string, string> STRING_STRING_MAP;
typedef unordered_map<string, int> STRING_INTEGER_MAP;
typedef unordered_map<string, STRING_LIST> STRING_STRINGLIST_MAP;
typedef unordered_map<string, STRING_SET> STRING_STRINGSET_MAP;
typedef unordered_map<string, STRINGLIST_LIST> STRING_STRINGLISTLIST_MAP;
typedef unordered_map<string, STRINGLIST_SET> STRING_STRINGLISTSET_MAP;
typedef unordered_map<STRING_LIST*, STRING_LIST> STRINGLIST_STRINGLIST_MAP;
typedef unordered_map<STRING_LIST*, STRINGLIST_SET> STRINGLIST_STRINGLISTSET_MAP;
typedef unordered_map<STRING_LIST*, STRING_SET> STRINGLIST_STRINGSET_MAP;
typedef unordered_map<STRING_SET*, STRINGLIST_SET> STRINGSET_STRINGLISTSET_MAP;
#endif



