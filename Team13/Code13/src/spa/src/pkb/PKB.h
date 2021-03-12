#pragma once

#ifndef _PKB_H_
#define _PKB_H_

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include <CFG.h>
#include <CFGManager.h>
#include <CustomTypes.h>
#include <DataManager.h>
#include <DesignExtractor.h>
#include <PatternManager.h>
#include <RelationManager.h>

using namespace std;

class TNode;

class PKB {
public:
    PKB() {
    }
    class AffectsManager {
        static STMT_IDX_SET affects_computed_set_;
        static STMT_IDX_SET inverse_affects_computed_set_;
        static AFFECTS_TABLE affects_table_;
        static INVERSE_AFFECTS_TABLE inverse_affects_table_;
        static STMT_IDX_SET all_affects_keys_;
        static STMT_IDX_SET all_inverse_affects_keys_;
        static STMT_STMT_PAIR_LIST all_affects_;

        static STMT_IDX_SET affects_star_computed_set_;
        static STMT_IDX_SET inverse_affects_star_computed_set_;
        static AFFECTS_STAR_TABLE affects_star_table_;
        static INVERSE_AFFECTS_STAR_TABLE inverse_affects_star_table_;
        static STMT_STMT_PAIR_LIST all_affects_star_;
        
    public:
        bool IsAffects(STMT_IDX a1, STMT_IDX a2);
        bool IsAffectsStar(STMT_IDX a1, STMT_IDX a2);

        STMT_IDX_SET GetAffects(STMT_IDX a);
        STMT_IDX_SET GetInverseAffects(STMT_IDX a);
        STMT_STMT_PAIR_LIST GetAllAffects();
        STMT_STMT_PAIR_LIST GetAllAffectsWithSameSynonyms();

        STMT_IDX_SET GetAffectsStar(STMT_IDX a);
        STMT_IDX_SET GetInverseAffectsStar(STMT_IDX a);
        STMT_STMT_PAIR_LIST GetAllAffectsStar();
        STMT_STMT_PAIR_LIST GetAllAffectsStarWithSameSynonyms();

        void ClearCache();

    private:
        void RecursiveGetAffects(STMT_IDX node, VAR_NAME lhs_var, STMT_IDX_SET& visited, STMT_IDX_SET& result);
        void RecursiveGetInverseAffects(STMT_IDX node, VAR_NAME rhs_var, INVERSE_AFFECTS_VISITED_SET& visited, STMT_IDX_SET& result);
        STMT_IDX_SET GetAllAffectsKeys();
        STMT_IDX_SET GetAllInverseAffectsKeys();
        void FillAffectsTable();
        void FillAffectsStarTable();
        STMT_STMT_PAIR_LIST GetAllPairsWithSameSynonyms(STMT_STMT_RELATION_TABLE table);
    };

    void SetASTRoot(TNode* root);
    void SetCFG(CFG& cfg);

    TNode GetASTRoot();
    DataManager GetDataManager();
    RelationManager GetRelationManager();
    PatternManager GetPatternManager();
    CFGManager GetCFGManager();
    AffectsManager GetAffectsManager();
    CFG& GetCFG();

    void ClearCache();

    STMT_IDX_SET FilterStmtTypes(STMT_IDX_SET unfiltered, STATEMENT_TYPE type);
    STMT_STMT_PAIR_LIST FilterStmtTypes(STMT_STMT_PAIR_LIST unfiltered, STATEMENT_TYPE type1, STATEMENT_TYPE type2);
    STMT_VAR_PAIR_LIST FilterStmtTypes(STMT_VAR_PAIR_LIST unfiltered, STATEMENT_TYPE type);

protected:
    static DataManager data_manager_;
    static RelationManager relation_manager_;
    static PatternManager pattern_manager_;
    static CFGManager cfg_manager_;
    static AffectsManager affects_manager_;
    static TNode ast_;
    
};

class ASTRootTypeUnmatchException: public exception {
    virtual const char* what() const throw()
    {
        return "AST root node type can only be program!";
    }
};


#endif
