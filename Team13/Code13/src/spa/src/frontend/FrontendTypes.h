#ifndef _FRONTENDTYPES_H_
#define _FRONTENDTYPES_H_

#include <vector>
#include <map>
#include <utility>

#include "Token.h"
#include "CFG.h"

using namespace std;

#define PROC_KEYWORD = "procedure"
#define TYPE_STMT_IF_THEN "then"
#define TYPE_STMT_IF_ELSE "else"

#define TYPE_PUNC_OPEN_PARAN "("
#define TYPE_PUNC_CLOSED_PARAN ")"
#define TYPE_PUNC_SEMICOLON ";"
#define TYPE_PUNC_OPEN_BRACKET "{"
#define TYPE_PUNC_CLOSED_BRACKET "}"
#define TYPE_EQUALS "="

typedef vector<string> STMT_NAME_LIST;
typedef vector<Token> TOKEN_LIST;
typedef vector<string> STR_LIST;
typedef vector<CFG*> CFG_LIST;

typedef string DFS_NODE;

typedef map<DFS_NODE, vector<DFS_NODE>> PROC_ADJ_LIST;

enum ExpressionType {
	_if,
	_while,
	_assign
};

#endif