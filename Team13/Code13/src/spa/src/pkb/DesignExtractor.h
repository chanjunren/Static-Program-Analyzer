#ifndef DESIGN_EXTRACTOR_H
#define DESIGN_EXTRACTOR_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "CustomTypes.h"
#include "PatternManager.h"
#include "PKB.h"
#include "TNode.h"


using namespace std;

class DesignExtractor {
public:
    static bool ExtractFollows(RelationManager manager, TNode root);
    static bool ExtractParent(RelationManager manager, TNode root);
    static bool ExtractModifies(RelationManager manager, TNode root);
    static bool ExtractUses(RelationManager manager, TNode root);
    static bool ExtractCalls(RelationManager manager, TNode root);
    static bool ExtractData(DataManager manager, TNode root);
    static bool ExtractAssignPattern(PatternManager manager, TNode root);
    static bool ExtractContainerPattern(PatternManager manager, TNode root);
    static bool ExtractAssignStmtInProcs(DataManager manager, TNode root);

private:
    static bool ExtractParentRecursive(RelationManager manager, TNode root, STMT_IDX_LIST parents);
    static list<pair <PROC_NAME, STMT_IDX> > ExtractUsesRecursive(RelationManager manager, TNode root, STMT_IDX_LIST parents, PROC_NAME proc);
    static VAR_NAME_LIST ExtractVarsFromExpr(TNode expr);
};

#endif