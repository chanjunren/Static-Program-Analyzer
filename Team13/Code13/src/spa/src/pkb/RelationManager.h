#ifndef RELATION_MANAGER_H
#define RELATION_MANAGER_H

#include <unordered_map>
#include <CustomTypes.h>

class RelationManager {
private:
    static FOLLOWS_TABLE* follows_table_;
    static FOLLOWS_STAR_TABLE* follows_star_table_;
    static INVERSE_FOLLOWS_TABLE* inverse_follows_table_;
    static INVERSE_FOLLOWS_STAR_TABLE* inverse_follows_star_table_;
    static PARENT_TABLE* parent_table_;
    static PARENT_STAR_TABLE* parent_star_table_;
    static INVERSE_PARENT_TABLE* inverse_parent_table_;
    static INVERSE_PARENT_STAR_TABLE* inverse_parent_star_table_;
    static STMT_USES_TABLE* stmt_uses_table_;
    static PROC_USES_TABLE* proc_uses_table_;
    static INVERSE_STMT_USES_TABLE* inverse_stmt_uses_table_;
    static INVERSE_PROC_USES_TABLE* inverse_proc_uses_table_;
    static STMT_MODIFIES_TABLE* stmt_modifies_table_;
    static PROC_MODIFIES_TABLE* proc_modifies_table_;
    static INVERSE_STMT_MODIFIES_TABLE* inverse_stmt_modifies_table_;
    static INVERSE_PROC_MODIFIES_TABLE* inverse_proc_modifies_table_;
    static CALLS_TABLE* calls_table_;
    static CALLS_STAR_TABLE* calls_star_table_;
    static INVERSE_CALLS_TABLE* inverse_calls_table_;
    static INVERSE_CALLS_STAR_TABLE* inverse_calls_star_table_;


    static STMT_STMT_PAIR_LIST* all_follows_;
    static STMT_STMT_PAIR_LIST* all_follows_star_;
    static STMT_STMT_PAIR_LIST* all_parent_;
    static STMT_STMT_PAIR_LIST* all_parent_star_;
    static STMT_VAR_PAIR_LIST* all_stmt_uses_;
    static PROC_VAR_PAIR_LIST* all_proc_uses_;
    static STMT_VAR_PAIR_LIST* all_stmt_modifies_;
    static PROC_VAR_PAIR_LIST* all_proc_modifies_;
    static PROC_PROC_PAIR_LIST* all_calls_;
    static PROC_PROC_PAIR_LIST* all_calls_star_;

    static STMT_IDX_SET* all_follows_keys_;
    static STMT_IDX_SET* all_inverse_follows_keys_;
    static STMT_IDX_SET* all_follows_star_keys_;
    static STMT_IDX_SET* all_inverse_follows_star_keys_;
    static STMT_IDX_SET* all_parent_keys_;
    static STMT_IDX_SET* all_inverse_parent_keys_;
    static STMT_IDX_SET* all_parent_star_keys_;
    static STMT_IDX_SET* all_inverse_parent_star_keys_;
    static STMT_IDX_SET* all_stmt_uses_keys_;
    static VAR_NAME_SET* all_inverse_stmt_uses_keys_;
    static PROC_NAME_SET* all_proc_uses_keys_;
    static VAR_NAME_SET* all_inverse_proc_uses_keys_;
    static STMT_IDX_SET* all_stmt_modifies_keys_;
    static VAR_NAME_SET* all_inverse_stmt_modifies_keys_;
    static PROC_NAME_SET* all_proc_modifies_keys_;
    static VAR_NAME_SET* all_inverse_proc_modifies_keys_;
    static PROC_NAME_SET* all_calls_keys_;
    static PROC_NAME_SET* all_inverse_calls_keys_;
    static PROC_NAME_SET* all_calls_star_keys_;
    static PROC_NAME_SET* all_inverse_calls_star_keys_;

    bool InsertStmtStmtRelation(STMT_STMT_RELATION_TABLE* set, STMT_IDX s1, STMT_IDX s2);
    bool InsertStmtVarRelation(STMT_VAR_RELATION_TABLE* set, STMT_IDX s, VAR_NAME v);
    bool InsertProcVarRelation(PROC_VAR_RELATION_TABLE* set, PROC_NAME p, VAR_NAME v);
    bool InsertVarStmtRelation(VAR_STMT_RELATION_TABLE* set, VAR_NAME v, STMT_IDX s);
    bool InsertVarProcRelation(VAR_PROC_RELATION_TABLE* set, VAR_NAME v, PROC_NAME p);
    bool InsertProcProcRelation(PROC_PROC_RELATION_TABLE* set, PROC_NAME p1, PROC_NAME p2);

    void InsertStmtStmtTuple(STMT_STMT_PAIR_LIST* set, STMT_IDX s1, STMT_IDX s2);
    void InsertStmtVarTuple(STMT_VAR_PAIR_LIST* set, STMT_IDX s, VAR_NAME v);
    void InsertProcVarTuple(PROC_VAR_PAIR_LIST* set, PROC_NAME p, VAR_NAME v);
    void InsertProcProcTuple(PROC_PROC_PAIR_LIST* set, PROC_NAME p1, PROC_NAME p2);

    bool InsertStmtKey(STMT_IDX_SET* set, STMT_IDX s);
    bool InsertProcKey(PROC_NAME_SET* set, PROC_NAME p);
    bool InsertVarKey(VAR_NAME_SET* set, VAR_NAME v);

    bool CheckStmtStmtRelation(STMT_STMT_RELATION_TABLE* set, STMT_STMT_RELATION_TABLE* inv_set, STMT_IDX s1, STMT_IDX s2);
    bool CheckStmtVarRelation(STMT_VAR_RELATION_TABLE* set, VAR_STMT_RELATION_TABLE* inv_set, STMT_IDX s, VAR_NAME v);
    bool CheckProcVarRelation(PROC_VAR_RELATION_TABLE* set, VAR_PROC_RELATION_TABLE* inv_set, PROC_NAME p, VAR_NAME v);
    bool CheckProcProcRelation(PROC_PROC_RELATION_TABLE* set, PROC_PROC_RELATION_TABLE *inv_set, PROC_NAME p1, PROC_NAME p2);
    bool CheckVarStmtRelation(VAR_STMT_RELATION_TABLE* set, VAR_NAME v, STMT_IDX s);
    bool CheckVarProcRelation(VAR_PROC_RELATION_TABLE* set, VAR_NAME v, PROC_NAME p);

    STMT_IDX_SET GetStmtStmtRelationVal(STMT_STMT_RELATION_TABLE* set, STMT_IDX_SET* stmt_keys, STMT_IDX s);
    VAR_NAME_SET GetStmtVarRelationVal(STMT_VAR_RELATION_TABLE* set, VAR_NAME_SET* var_keys, STMT_IDX s);
    VAR_NAME_SET GetProcVarRelationVal(PROC_VAR_RELATION_TABLE* set, VAR_NAME_SET* var_keys, PROC_NAME p);
    STMT_IDX_SET GetVarStmtRelationVal(VAR_STMT_RELATION_TABLE* set, STMT_IDX_SET* stmt_keys, VAR_NAME v);
    PROC_NAME_SET GetVarProcRelationVal(VAR_PROC_RELATION_TABLE* set, PROC_NAME_SET* proc_keys, VAR_NAME v);
    PROC_NAME_SET GetProcProcRelationVal(PROC_PROC_RELATION_TABLE* set, PROC_NAME_SET* proc_keys, PROC_NAME p);
public:
    bool AddFollows(STMT_IDX s1, STMT_IDX s2);
    bool AddFollowsStar(STMT_IDX s1, STMT_IDX s2);
    bool AddParent(STMT_IDX s1, STMT_IDX s2);
    bool AddParentStar(STMT_IDX s1, STMT_IDX s2);
    bool AddStmtUses(STMT_IDX s, VAR_NAME v);
    bool AddProcUses(PROC_NAME p, VAR_NAME v);
    bool AddStmtModifies(STMT_IDX s, VAR_NAME v);
    bool AddProcModifies(PROC_NAME p, VAR_NAME v);
    bool AddCalls(PROC_NAME p1, PROC_NAME p2);
    bool AddCallsStar(PROC_NAME p1, PROC_NAME p2);

    bool IsFollows(STMT_IDX s1, STMT_IDX s2);
    bool IsFollowsStar(STMT_IDX s1, STMT_IDX s2);
    bool IsParent(STMT_IDX s1, STMT_IDX s2);
    bool IsParentStar(STMT_IDX s1, STMT_IDX s2);
    bool IsStmtUses(STMT_IDX s, VAR_NAME v);
    bool IsProcUses(PROC_NAME p, VAR_NAME v);
    bool IsStmtModifies(STMT_IDX s, VAR_NAME v);
    bool IsProcModifies(PROC_NAME p, VAR_NAME v);
    bool IsCalls(PROC_NAME p1, PROC_NAME p2);
    bool IsCallsStar(PROC_NAME p1, PROC_NAME p2);

    STMT_IDX_SET GetFollows(STMT_IDX s);
    STMT_IDX_SET GetInverseFollows(STMT_IDX s);
    STMT_STMT_PAIR_LIST GetAllFollows();
    STMT_IDX_SET GetFollowsStars(STMT_IDX s);
    STMT_IDX_SET GetInverseFollowsStars(STMT_IDX s);
    STMT_STMT_PAIR_LIST GetAllFollowsStars();
    STMT_IDX_SET GetParents(STMT_IDX s);
    STMT_IDX_SET GetInverseParents(STMT_IDX s);
    STMT_STMT_PAIR_LIST GetAllParents();
    STMT_IDX_SET GetParentStars(STMT_IDX s);
    STMT_IDX_SET GetInverseParentStars(STMT_IDX s);
    STMT_STMT_PAIR_LIST GetAllParentStars();
    VAR_NAME_SET GetStmtUses(STMT_IDX s);
    STMT_IDX_SET GetInverseStmtUses(VAR_NAME v);
    STMT_VAR_PAIR_LIST GetAllStmtUses();
    VAR_NAME_SET GetProcUses(PROC_NAME p);
    PROC_NAME_SET GetInverseProcUses(VAR_NAME v);
    PROC_VAR_PAIR_LIST GetAllProcUses();
    VAR_NAME_SET GetStmtModifies(STMT_IDX s);
    STMT_IDX_SET GetInverseStmtModifies(VAR_NAME v);
    STMT_VAR_PAIR_LIST GetAllStmtModifies();
    VAR_NAME_SET GetProcModifies(PROC_NAME p);
    PROC_NAME_SET GetInverseProcModifies(VAR_NAME v);
    PROC_VAR_PAIR_LIST GetAllProcModifies();
    PROC_NAME_SET GetCalls(PROC_NAME p);
    PROC_NAME_SET GetInverseCalls(PROC_NAME p);
    PROC_PROC_PAIR_LIST GetAllCalls();
    PROC_NAME_SET GetCallsStars(PROC_NAME p);
    PROC_NAME_SET GetInverseCallsStars(PROC_NAME p);
    PROC_PROC_PAIR_LIST GetAllCallsStar();

};

#endif // RELATION_MANAGER_H