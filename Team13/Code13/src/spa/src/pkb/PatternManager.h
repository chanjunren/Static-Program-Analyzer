#ifndef PATTERNMANAGER_H
#define PATTERNMANAGER_H

#include <cstdlib>
#include <stack>
#include <algorithm>
#include <CustomTypes.h>
#include <DataTables.h>
#include <RelationManager.h>
#include "CustomTypes.h"
#include "PQLCustomTypes.h"
#include "TNode.h"

typedef struct {
    STMT_IDX s;
    VAR_NAME v;
    TNode eroot;
}ASSIGN_PATTERN;

typedef struct {
    STMT_IDX s;
    TNode eroot;
}CONTAINER_PATTERN;

typedef std::vector<ASSIGN_PATTERN> ASSIGN_EXPRESSION_TABLE;
class AssignPatternTable {
private:
    ASSIGN_EXPRESSION_TABLE* data_ = new ASSIGN_EXPRESSION_TABLE();
public:
    ASSIGN_EXPRESSION_TABLE* GetData();
    void Add(STMT_IDX s, VAR_NAME v, TNode root);
};

typedef std::vector<CONTAINER_PATTERN> CONTAINER_EXPRESSION_TABLE;
class ContainerPatternTable {
private:
    CONTAINER_EXPRESSION_TABLE* data_ = new CONTAINER_EXPRESSION_TABLE();
public:
    CONTAINER_EXPRESSION_TABLE* GetData();
    void Add(STMT_IDX s, TNode root);
};
class PatternManager {
private:
    AssignPatternTable assign_pattern_table_;
    ContainerPatternTable if_pattern_table_;
    ContainerPatternTable while_pattern_table_;
    STMT_IDX_SET GetStmtOfMatchingAssignments(VAR_NAME v, EXPRESSION e, bool is_full, ASSIGN_EXPRESSION_TABLE* data);
    STMT_VAR_PAIR_LIST GetStmtVarPairOfMatchingAssignments(VAR_NAME v, EXPRESSION e, bool is_full, ASSIGN_EXPRESSION_TABLE* data);
    STMT_IDX_SET GetStmtOfMatchingContainers(VAR_NAME v, CONTAINER_EXPRESSION_TABLE* data);
    STMT_VAR_PAIR_LIST GetStmtVarPairOfMatchingContainers(VAR_NAME v, CONTAINER_EXPRESSION_TABLE* data);
    bool HasMatchingPattern(TNode root, TNode qroot, bool is_full);
    bool HasSubMatchingPattern(TNode root, TNode qroot);
    bool AreTwoTreesEqual(TNode root, TNode query_root);
    bool AreTwoNodesEqual(TNode root, TNode query_root);
    bool HasVariable(VAR_NAME v, TNode root);
    VAR_NAME_SET GetAllUsedVars(TNode root);
    TNode* ParseExpression(EXPRESSION s);
    void PrintTNode(TNode root);
public:
    void AddAssignPattern(STMT_IDX s, VAR_NAME v, TNode root);
    void AddIfPattern(STMT_IDX s, TNode root);
    void AddWhilePattern(STMT_IDX s, TNode root);
    STMT_IDX_SET GetAssignWithFullPattern(VAR_NAME v, EXPRESSION e);
    STMT_IDX_SET GetAssignWithSubPattern(VAR_NAME v, EXPRESSION e);
    STMT_VAR_PAIR_LIST GetAssignStmtVarPairWithFullPattern(VAR_NAME v, EXPRESSION e);
    STMT_VAR_PAIR_LIST GetAssignStmtVarPairWithSubPattern(VAR_NAME v, EXPRESSION e);
    STMT_IDX_SET GetIfWithPattern(VAR_NAME v);
    STMT_IDX_SET GetWhileWithPattern(VAR_NAME v);
    STMT_VAR_PAIR_LIST GetIfStmtVarPairWithPattern(VAR_NAME v);
    STMT_VAR_PAIR_LIST GetWhileStmtVarPairWithPattern(VAR_NAME v);
};

#endif // PATTERNMANAGER_H