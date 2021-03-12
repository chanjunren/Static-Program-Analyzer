//
// Created by Xu Lin on 11/9/20.
//
#include <cstdio>
#include <iostream>
#include <string>
#include "RelationManager.h"
using namespace std;

FOLLOWS_TABLE* RelationManager::follows_table_ = new FOLLOWS_TABLE();
FOLLOWS_STAR_TABLE* RelationManager::follows_star_table_ = new FOLLOWS_STAR_TABLE();
INVERSE_FOLLOWS_TABLE* RelationManager::inverse_follows_table_ = new INVERSE_FOLLOWS_TABLE();
INVERSE_FOLLOWS_STAR_TABLE* RelationManager::inverse_follows_star_table_ = new INVERSE_FOLLOWS_STAR_TABLE();
PARENT_TABLE* RelationManager::parent_table_ = new PARENT_TABLE();
PARENT_STAR_TABLE* RelationManager::parent_star_table_ = new PARENT_STAR_TABLE();
INVERSE_PARENT_TABLE* RelationManager::inverse_parent_table_ = new INVERSE_PARENT_TABLE();
INVERSE_PARENT_STAR_TABLE* RelationManager::inverse_parent_star_table_ = new INVERSE_PARENT_STAR_TABLE();
STMT_USES_TABLE* RelationManager::stmt_uses_table_ = new STMT_USES_TABLE();
PROC_USES_TABLE* RelationManager::proc_uses_table_ = new PROC_USES_TABLE();
INVERSE_STMT_USES_TABLE* RelationManager::inverse_stmt_uses_table_ = new INVERSE_STMT_USES_TABLE();
INVERSE_PROC_USES_TABLE* RelationManager::inverse_proc_uses_table_ = new INVERSE_PROC_USES_TABLE();
STMT_MODIFIES_TABLE* RelationManager::stmt_modifies_table_ = new STMT_MODIFIES_TABLE();
PROC_MODIFIES_TABLE* RelationManager::proc_modifies_table_ = new PROC_MODIFIES_TABLE();
INVERSE_STMT_MODIFIES_TABLE* RelationManager::inverse_stmt_modifies_table_ = new INVERSE_STMT_MODIFIES_TABLE();
INVERSE_PROC_MODIFIES_TABLE* RelationManager::inverse_proc_modifies_table_ = new INVERSE_PROC_MODIFIES_TABLE();
CALLS_TABLE* RelationManager::calls_table_ = new CALLS_TABLE();
INVERSE_CALLS_TABLE* RelationManager::inverse_calls_table_ = new INVERSE_CALLS_TABLE();
CALLS_STAR_TABLE* RelationManager::calls_star_table_ = new CALLS_TABLE();
INVERSE_CALLS_STAR_TABLE* RelationManager::inverse_calls_star_table_ = new INVERSE_CALLS_TABLE();

STMT_STMT_PAIR_LIST* RelationManager::all_follows_ = new STMT_STMT_PAIR_LIST();
STMT_STMT_PAIR_LIST* RelationManager::all_follows_star_ = new STMT_STMT_PAIR_LIST();
STMT_STMT_PAIR_LIST* RelationManager::all_parent_ = new STMT_STMT_PAIR_LIST();
STMT_STMT_PAIR_LIST* RelationManager::all_parent_star_ = new STMT_STMT_PAIR_LIST();
STMT_VAR_PAIR_LIST* RelationManager::all_stmt_uses_ = new STMT_VAR_PAIR_LIST();
PROC_VAR_PAIR_LIST* RelationManager::all_proc_uses_ = new PROC_VAR_PAIR_LIST();
STMT_VAR_PAIR_LIST* RelationManager::all_stmt_modifies_ = new STMT_VAR_PAIR_LIST();
PROC_VAR_PAIR_LIST* RelationManager::all_proc_modifies_ = new PROC_VAR_PAIR_LIST();
PROC_PROC_PAIR_LIST* RelationManager::all_calls_ = new PROC_PROC_PAIR_LIST();
PROC_PROC_PAIR_LIST* RelationManager::all_calls_star_ = new PROC_PROC_PAIR_LIST();

STMT_IDX_SET* RelationManager::all_follows_keys_ = new STMT_IDX_SET();
STMT_IDX_SET* RelationManager::all_inverse_follows_keys_ = new STMT_IDX_SET();
STMT_IDX_SET* RelationManager::all_follows_star_keys_ = new STMT_IDX_SET();
STMT_IDX_SET* RelationManager::all_inverse_follows_star_keys_ = new STMT_IDX_SET();
STMT_IDX_SET* RelationManager::all_parent_keys_ = new STMT_IDX_SET();
STMT_IDX_SET* RelationManager::all_inverse_parent_keys_ = new STMT_IDX_SET();
STMT_IDX_SET* RelationManager::all_parent_star_keys_ = new STMT_IDX_SET();
STMT_IDX_SET* RelationManager::all_inverse_parent_star_keys_ = new STMT_IDX_SET();
STMT_IDX_SET* RelationManager::all_stmt_uses_keys_ = new STMT_IDX_SET();
VAR_NAME_SET* RelationManager::all_inverse_stmt_uses_keys_ = new VAR_NAME_SET();
PROC_NAME_SET* RelationManager::all_proc_uses_keys_ = new PROC_NAME_SET();
VAR_NAME_SET* RelationManager::all_inverse_proc_uses_keys_ = new VAR_NAME_SET();
STMT_IDX_SET* RelationManager::all_stmt_modifies_keys_ = new STMT_IDX_SET();
VAR_NAME_SET* RelationManager::all_inverse_stmt_modifies_keys_ = new VAR_NAME_SET();
PROC_NAME_SET* RelationManager::all_proc_modifies_keys_ = new PROC_NAME_SET();
VAR_NAME_SET* RelationManager::all_inverse_proc_modifies_keys_ = new VAR_NAME_SET();
PROC_NAME_SET* RelationManager::all_calls_keys_ = new PROC_NAME_SET();
PROC_NAME_SET* RelationManager::all_inverse_calls_keys_ = new PROC_NAME_SET();
PROC_NAME_SET* RelationManager::all_calls_star_keys_ = new PROC_NAME_SET();
PROC_NAME_SET* RelationManager::all_inverse_calls_star_keys_ = new PROC_NAME_SET();

bool RelationManager::AddFollows(STMT_IDX s1, STMT_IDX s2) {
    bool can_add_follows =  InsertStmtStmtRelation(follows_table_, s1, s2);
    bool can_add_inverse_follows = InsertStmtStmtRelation(inverse_follows_table_, s2, s1);
    bool can_add_follows_star = InsertStmtStmtRelation(follows_star_table_, s1, s2);
    bool can_add_inverse_follows_star = InsertStmtStmtRelation(inverse_follows_star_table_, s2, s1);
    if (can_add_follows) {
        InsertStmtStmtTuple(all_follows_, s1, s2);
    }
    if (can_add_follows_star) {
        InsertStmtStmtTuple(all_follows_star_, s1, s2);
    }
    InsertStmtKey(all_follows_keys_, s1);
    InsertStmtKey(all_inverse_follows_keys_, s2);
    InsertStmtKey(all_follows_star_keys_, s1);
    InsertStmtKey(all_inverse_follows_star_keys_, s2);
    return can_add_follows && can_add_inverse_follows && can_add_follows_star && can_add_inverse_follows_star;
}

bool RelationManager::AddFollowsStar(STMT_IDX s1, STMT_IDX s2) {
    bool can_add_follows_star = InsertStmtStmtRelation(follows_star_table_, s1, s2);
    bool can_add_inverse_follows_star = InsertStmtStmtRelation(inverse_follows_star_table_, s2, s1);
    if (can_add_follows_star) {
        InsertStmtStmtTuple(all_follows_star_, s1, s2);
    }
    InsertStmtKey(all_follows_star_keys_, s1);
    InsertStmtKey(all_inverse_follows_star_keys_, s2);
    return can_add_follows_star && can_add_inverse_follows_star;
}

bool RelationManager::AddParent(STMT_IDX s1, STMT_IDX s2) {
    bool can_add_parent = InsertStmtStmtRelation(parent_table_, s1, s2);
    bool can_add_inverse_parent = InsertStmtStmtRelation(inverse_parent_table_, s2, s1);
    bool can_add_parent_star = InsertStmtStmtRelation(parent_star_table_, s1, s2);
    bool can_add_inverse_parent_star = InsertStmtStmtRelation(inverse_parent_star_table_, s2, s1);
    if (can_add_parent) {
        InsertStmtStmtTuple(all_parent_, s1, s2);
    }
    if (can_add_parent_star) {
        InsertStmtStmtTuple(all_parent_star_, s1, s2);
    }
    InsertStmtKey(all_parent_keys_, s1);
    InsertStmtKey(all_inverse_parent_keys_, s2);
    InsertStmtKey(all_parent_star_keys_, s1);
    InsertStmtKey(all_inverse_parent_star_keys_, s2);
    return can_add_parent && can_add_parent_star && can_add_inverse_parent && can_add_inverse_parent_star;
}
bool RelationManager::AddParentStar(STMT_IDX s1, STMT_IDX s2) {
    bool can_add_parent_star = InsertStmtStmtRelation(parent_star_table_, s1, s2);
    bool can_add_inverse_parent_star = InsertStmtStmtRelation(inverse_parent_star_table_, s2, s1);
    if (can_add_parent_star) {
        InsertStmtStmtTuple(all_parent_star_, s1, s2);
    }
    InsertStmtKey(all_parent_star_keys_, s1);
    InsertStmtKey(all_inverse_parent_star_keys_, s2);
    return can_add_parent_star && can_add_inverse_parent_star;
}
bool RelationManager::AddStmtUses(STMT_IDX s, VAR_NAME v) {
    bool can_add_uses = InsertStmtVarRelation(stmt_uses_table_, s, v);
    bool can_add_inverse_uses = InsertVarStmtRelation(inverse_stmt_uses_table_, v, s);
    if (can_add_uses) {
        InsertStmtVarTuple(all_stmt_uses_, s, v);
    }
    InsertStmtKey(all_stmt_uses_keys_, s);
    InsertVarKey(all_inverse_stmt_uses_keys_, v);
    return can_add_inverse_uses && can_add_uses;
}
bool RelationManager::AddProcUses(PROC_NAME p, VAR_NAME v) {
    bool can_add_uses = InsertProcVarRelation(proc_uses_table_, p, v);
    bool can_add_inverse_uses = InsertVarProcRelation(inverse_proc_uses_table_, v, p);
    if (can_add_uses) {
        InsertProcVarTuple(all_proc_uses_, p, v);
    }
    InsertProcKey(all_proc_uses_keys_, p);
    InsertVarKey(all_inverse_proc_uses_keys_, v);
    return can_add_uses && can_add_inverse_uses;
}

bool RelationManager::AddStmtModifies(STMT_IDX s, VAR_NAME v) {
    bool can_add_modifies = InsertStmtVarRelation(stmt_modifies_table_, s, v);
    bool can_add_inverse_modifies = InsertVarStmtRelation(inverse_stmt_modifies_table_, v, s);
    if (can_add_modifies) {
        InsertStmtVarTuple(all_stmt_modifies_, s, v);
    }
    InsertStmtKey(all_stmt_modifies_keys_, s);
    InsertVarKey(all_inverse_stmt_modifies_keys_, v);
    return can_add_modifies && can_add_inverse_modifies;
}
bool RelationManager::AddProcModifies(PROC_NAME p, VAR_NAME v) {
    bool can_add_modifies = InsertProcVarRelation(proc_modifies_table_, p, v);
    bool can_add_inverse_modifies = InsertVarProcRelation(inverse_proc_modifies_table_, v, p);
    if (can_add_modifies) {
        InsertProcVarTuple(all_proc_modifies_, p, v);
    }
    InsertProcKey(all_proc_modifies_keys_, p);
    InsertVarKey(all_inverse_proc_modifies_keys_, v);
    return can_add_modifies && can_add_inverse_modifies;
}
bool RelationManager::AddCalls(PROC_NAME p1, PROC_NAME p2) {
    bool can_add_calls = InsertProcProcRelation(calls_table_, p1, p2);
    bool can_add_inverse_calls = InsertProcProcRelation(inverse_calls_table_, p2, p1);
    bool can_add_calls_star = InsertProcProcRelation(calls_star_table_, p1, p2);
    bool can_add_inverse_calls_star = InsertProcProcRelation(inverse_calls_star_table_, p2, p1);
    if (can_add_calls) {
        InsertProcProcTuple(all_calls_, p1, p2);
    }
    if (can_add_calls_star) {
        InsertProcProcTuple(all_calls_star_, p1, p2);
    }
    InsertProcKey(all_calls_keys_, p1);
    InsertProcKey(all_inverse_calls_keys_, p2);
    InsertProcKey(all_calls_star_keys_, p1);
    InsertProcKey(all_inverse_calls_star_keys_, p2);
    return can_add_calls && can_add_inverse_calls && can_add_calls_star && can_add_inverse_calls_star;
}

bool RelationManager::AddCallsStar(PROC_NAME p1, PROC_NAME p2) {
    bool can_add_calls_star = InsertProcProcRelation(calls_star_table_, p1, p2);
    bool can_add_inverse_calls_star = InsertProcProcRelation(inverse_calls_star_table_, p2, p1);
    if (can_add_calls_star) {
        InsertProcProcTuple(all_calls_star_, p1, p2);
    }
    InsertProcKey(all_calls_star_keys_, p1);
    InsertProcKey(all_inverse_calls_star_keys_, p2);
    return can_add_calls_star && can_add_inverse_calls_star;
}

bool RelationManager::IsFollows(STMT_IDX s1, STMT_IDX s2) {
    return CheckStmtStmtRelation(follows_table_, inverse_follows_table_, s1, s2);
}
bool RelationManager::IsFollowsStar(STMT_IDX s1, STMT_IDX s2) {
    return CheckStmtStmtRelation(follows_star_table_, inverse_follows_star_table_, s1, s2);
}
bool RelationManager::IsParent(STMT_IDX s1, STMT_IDX s2) {
    return CheckStmtStmtRelation(parent_table_, inverse_parent_table_, s1, s2);
}
bool RelationManager::IsParentStar(STMT_IDX s1, STMT_IDX s2) {
    return CheckStmtStmtRelation(parent_star_table_, inverse_parent_star_table_, s1, s2);
}
bool RelationManager::IsStmtUses(STMT_IDX s, VAR_NAME v) {
    return CheckStmtVarRelation(stmt_uses_table_, inverse_stmt_uses_table_, s, v);
}
bool RelationManager::IsProcUses(PROC_NAME p, VAR_NAME v) {
    return CheckProcVarRelation(proc_uses_table_, inverse_proc_uses_table_, p, v);
}
bool RelationManager::IsStmtModifies(STMT_IDX s, VAR_NAME v) {
    return CheckStmtVarRelation(stmt_modifies_table_, inverse_stmt_modifies_table_, s, v);
}
bool RelationManager::IsProcModifies(PROC_NAME p, VAR_NAME v) {
    return CheckProcVarRelation(proc_modifies_table_, inverse_proc_modifies_table_, p, v);
}
bool RelationManager::IsCalls(PROC_NAME p1, PROC_NAME p2) {
    return CheckProcProcRelation(calls_table_, inverse_calls_table_, p1, p2);
}
bool RelationManager::IsCallsStar(PROC_NAME p1, PROC_NAME p2) {
    return CheckProcProcRelation(calls_star_table_, inverse_calls_star_table_, p1, p2);
}

STMT_IDX_SET RelationManager::GetFollows(STMT_IDX s) {
    return GetStmtStmtRelationVal(follows_table_, all_inverse_follows_keys_, s);
}
STMT_IDX_SET RelationManager::GetInverseFollows(STMT_IDX s) {
    return GetStmtStmtRelationVal(inverse_follows_table_, all_follows_keys_, s);
}
STMT_IDX_SET RelationManager::GetFollowsStars(STMT_IDX s) {
    return GetStmtStmtRelationVal(follows_star_table_, all_inverse_follows_star_keys_, s);
}
STMT_IDX_SET RelationManager::GetInverseFollowsStars(STMT_IDX s) {
    return GetStmtStmtRelationVal(inverse_follows_star_table_, all_follows_star_keys_, s);
}
STMT_IDX_SET RelationManager::GetParents(STMT_IDX s) {
    return GetStmtStmtRelationVal(parent_table_, all_inverse_parent_keys_, s);
}
STMT_IDX_SET RelationManager::GetInverseParents(STMT_IDX s) {
    return GetStmtStmtRelationVal(inverse_parent_table_, all_parent_keys_, s);
}
STMT_IDX_SET RelationManager::GetParentStars(STMT_IDX s) {
    return GetStmtStmtRelationVal(parent_star_table_, all_inverse_parent_star_keys_, s);
}
STMT_IDX_SET RelationManager::GetInverseParentStars(STMT_IDX s) {
    return GetStmtStmtRelationVal(inverse_parent_star_table_, all_parent_star_keys_, s);
}
VAR_NAME_SET RelationManager::GetStmtUses(STMT_IDX s) {
    return GetStmtVarRelationVal(stmt_uses_table_, all_inverse_stmt_uses_keys_, s);
}
STMT_IDX_SET RelationManager::GetInverseStmtUses(VAR_NAME v) {
    return GetVarStmtRelationVal(inverse_stmt_uses_table_, all_stmt_uses_keys_, v);
}
VAR_NAME_SET RelationManager::GetProcUses(PROC_NAME p) {
    return GetProcVarRelationVal(proc_uses_table_, all_inverse_proc_uses_keys_, p);
}
PROC_NAME_SET  RelationManager::GetInverseProcUses(VAR_NAME v) {
    return GetVarProcRelationVal(inverse_proc_uses_table_, all_proc_uses_keys_, v);
}
VAR_NAME_SET RelationManager::GetStmtModifies(STMT_IDX s) {
    return GetStmtVarRelationVal(stmt_modifies_table_, all_inverse_stmt_modifies_keys_, s);
}
STMT_IDX_SET RelationManager::GetInverseStmtModifies(VAR_NAME v) {
    return GetVarStmtRelationVal(inverse_stmt_modifies_table_, all_stmt_modifies_keys_, v);
}
VAR_NAME_SET RelationManager::GetProcModifies(PROC_NAME p) {
    return GetProcVarRelationVal(proc_modifies_table_, all_inverse_proc_modifies_keys_, p);
}
PROC_NAME_SET  RelationManager::GetInverseProcModifies(VAR_NAME v) {
    return GetVarProcRelationVal(inverse_proc_modifies_table_, all_proc_modifies_keys_, v);
}
PROC_NAME_SET RelationManager::GetCalls(PROC_NAME p) {
    return GetProcProcRelationVal(calls_table_, all_inverse_calls_keys_, p);
}
PROC_NAME_SET RelationManager::GetInverseCalls(PROC_NAME p) {
    return GetProcProcRelationVal(inverse_calls_table_, all_calls_keys_, p);
}
PROC_NAME_SET RelationManager::GetCallsStars(PROC_NAME p) {
    return GetProcProcRelationVal(calls_star_table_, all_inverse_calls_star_keys_, p);
}
PROC_NAME_SET RelationManager::GetInverseCallsStars(PROC_NAME p) {
    return GetProcProcRelationVal(inverse_calls_star_table_, all_calls_star_keys_, p);
}

STMT_STMT_PAIR_LIST RelationManager::GetAllFollows() {
    return *all_follows_;
}
STMT_STMT_PAIR_LIST RelationManager::GetAllFollowsStars() {
    return *all_follows_star_;
}
STMT_STMT_PAIR_LIST RelationManager::GetAllParents() {
    return *all_parent_;
}
STMT_STMT_PAIR_LIST RelationManager::GetAllParentStars() {
    return *all_parent_star_;
}
STMT_VAR_PAIR_LIST RelationManager::GetAllStmtUses() {
    return *all_stmt_uses_;
}
PROC_VAR_PAIR_LIST RelationManager::GetAllProcUses() {
    return *all_proc_uses_;
}
STMT_VAR_PAIR_LIST RelationManager::GetAllStmtModifies() {
    return *all_stmt_modifies_;
}
PROC_VAR_PAIR_LIST RelationManager::GetAllProcModifies() {
    return *all_proc_modifies_;
}
PROC_PROC_PAIR_LIST RelationManager::GetAllCalls() {
    return *all_calls_;
}
PROC_PROC_PAIR_LIST RelationManager::GetAllCallsStar() {
    return *all_calls_star_;
}

bool RelationManager::InsertStmtStmtRelation(STMT_STMT_RELATION_TABLE* set, STMT_IDX s1, STMT_IDX s2) {
    auto iter = set->find(s1);
    if (iter == set->end()) {
        STMT_IDX_SET* stmt_set = new STMT_IDX_SET();
        bool res = set->insert({s1, stmt_set}).second && stmt_set->insert(s2).second;
        return res;
    } else {
        bool res = set->at(s1)->insert(s2).second;
        return res;
    }
}

bool RelationManager::InsertStmtVarRelation(STMT_VAR_RELATION_TABLE *set, STMT_IDX s, VAR_NAME v) {
    auto iter = set->find(s);
    if (iter == set->end()) {
        VAR_NAME_SET* var_name_set = new VAR_NAME_SET();
        return set->insert({s, var_name_set}).second && var_name_set->insert(v).second;
    } else {
        return set->at(s)->insert(v).second;
    }
}

bool RelationManager::InsertProcVarRelation(PROC_VAR_RELATION_TABLE *set, PROC_NAME p, VAR_NAME v) {
    auto iter = set->find(p);
    if (iter == set->end()) {
        VAR_NAME_SET* var_name_set = new VAR_NAME_SET();
        return set->insert({p, var_name_set}).second && var_name_set->insert(v).second;
    } else {
        return set->at(p)->insert(v).second;
    }
}

bool RelationManager::InsertVarStmtRelation(VAR_STMT_RELATION_TABLE *set, VAR_NAME v, STMT_IDX s) {
    auto iter = set->find(v);
    if (iter == set->end()) {
        STMT_IDX_SET* stmt_idx_set = new STMT_IDX_SET();
        return set->insert({v, stmt_idx_set}).second && stmt_idx_set->insert(s).second;
    } else {
        return set->at(v)->insert(s).second;
    }
}

bool RelationManager::InsertVarProcRelation(VAR_PROC_RELATION_TABLE *set, VAR_NAME v, PROC_NAME p) {
    auto iter = set->find(v);
    if (iter == set->end()) {
        PROC_NAME_SET* proc_name_set = new PROC_NAME_SET();
        return set->insert({v, proc_name_set}).second && proc_name_set->insert(p).second;
    } else {
        return set->at(v)->insert(p).second;
    }
}

bool RelationManager::InsertProcProcRelation(VAR_PROC_RELATION_TABLE *set, PROC_NAME p1, PROC_NAME p2) {
    auto iter = set->find(p1);
    if (iter == set->end()) {
        PROC_NAME_SET* proc_name_set = new PROC_NAME_SET();
        return set->insert({p1, proc_name_set}).second && proc_name_set->insert(p2).second;
    } else {
        return set->at(p1)->insert(p2).second;
    }
}

bool RelationManager::CheckStmtStmtRelation(STMT_STMT_RELATION_TABLE *set, STMT_STMT_RELATION_TABLE *inv_set, STMT_IDX s1, STMT_IDX s2) {
    //If s1 and s2 are both wildcard, then just check if program contains such relation
    if (s1 < 0 && s2 < 0) {
        return !set->empty();
    }
    if (s1 < 0) {
        auto iter = inv_set->find(s2);
        return iter != inv_set->end() && !(iter->second->empty());
    }
    if (s2 < 0) {
        auto iter = set->find(s1);
        return iter != set->end() && !(iter->second->empty());
    }
    auto iter = set->find(s1);
    bool res = iter != set->end() && iter->second->find(s2) != iter->second->end();
    return res;
}
bool RelationManager::CheckStmtVarRelation(STMT_VAR_RELATION_TABLE *set, VAR_STMT_RELATION_TABLE *inv_set, STMT_IDX s, VAR_NAME v) {
    //If s and v are both wildcard, then just check if program contains such relation
    if (s < 0 && v.empty()) {
        return !set->empty();
    }
    if (s < 0) {
        auto iter = inv_set->find(v);
        return iter != inv_set->end() && !(iter->second->empty());
    }
    if (v.empty()) {
        auto iter = set->find(s);
        return iter != set->end() && !(iter->second->empty());
    }
    auto iter = set->find(s);
    return iter != set->end() && iter->second->find(v) != iter->second->end();
}
bool RelationManager::CheckProcVarRelation(PROC_VAR_RELATION_TABLE *set, VAR_PROC_RELATION_TABLE *inv_set, PROC_NAME p, VAR_NAME v) {
    //If p and v are both wildcard, then just check if program contains such relation
    if (p.empty() && v.empty()) {
        return !set->empty();
    }
    if (p.empty()) {
        auto iter = inv_set->find(v);
        return iter != inv_set->end() && !(iter->second->empty());
    }
    if (v.empty()) {
        auto iter = set->find(p);
        return iter != set->end() && !(iter->second->empty());
    }
    auto iter = set->find(p);
    return iter != set->end() && iter->second->find(v) != iter->second->end();
}

bool RelationManager::CheckProcProcRelation(PROC_PROC_RELATION_TABLE *set, PROC_PROC_RELATION_TABLE *inv_set, PROC_NAME p1, PROC_NAME p2) {
    //If p1 and p2 are both wildcard, then just check if program contains such relation
    if (p1.empty() && p2.empty()) {
        return !set->empty();
    }
    if (p1.empty()) {
        auto iter = inv_set->find(p2);
        return iter != inv_set->end() && !(iter->second->empty());
    }
    if (p2.empty()) {
        auto iter = set->find(p1);
        return iter != set->end() && !(iter->second->empty());
    }
    auto iter = set->find(p1);
    return iter != set->end() && iter->second->find(p2) != iter->second->end();
}

bool RelationManager::CheckVarStmtRelation(VAR_STMT_RELATION_TABLE *set, VAR_NAME v, STMT_IDX s) {
    auto iter = set->find(v);
    return iter != set->end() && iter->second->find(s) != iter->second->end();
}

bool RelationManager::CheckVarProcRelation(VAR_PROC_RELATION_TABLE *set, VAR_NAME v, PROC_NAME p) {
    auto iter = set->find(v);
    return iter != set->end() && iter->second->find(p) != iter->second->end();
}


STMT_IDX_SET RelationManager::GetStmtStmtRelationVal(STMT_STMT_RELATION_TABLE *set, STMT_IDX_SET *stmt_keys, STMT_IDX s) {
    if (s < 0) {
        return *stmt_keys;
    }
    auto iter = set->find(s);
    if (iter != set->end()) {
        return *(iter->second);
    }
    return STMT_IDX_SET();
}

VAR_NAME_SET RelationManager::GetStmtVarRelationVal(STMT_VAR_RELATION_TABLE *set, VAR_NAME_SET *var_keys, STMT_IDX s) {
    if (s < 0) {
        return *var_keys;
    }
    auto iter = set->find(s);
    if (iter != set->end())  {
        return *(iter->second);
    }
    return VAR_NAME_SET();
}
VAR_NAME_SET RelationManager::GetProcVarRelationVal(PROC_VAR_RELATION_TABLE *set, VAR_NAME_SET *var_keys, PROC_NAME p) {
    if (p.empty()) {
        return *var_keys;
    }
    auto iter = set->find(p);
    if (iter != set->end())  {
        return *(iter->second);
    }
    return VAR_NAME_SET();
}
STMT_IDX_SET RelationManager::GetVarStmtRelationVal(VAR_STMT_RELATION_TABLE *set, STMT_IDX_SET *stmt_keys, VAR_NAME v) {
    if (v.empty()) {
        return *stmt_keys;
    }
    auto iter = set->find(v);
    if (iter != set->end())  {
        return *(iter->second);
    }
    return STMT_IDX_SET();
}

PROC_NAME_SET RelationManager::GetVarProcRelationVal(VAR_PROC_RELATION_TABLE *set, PROC_NAME_SET *proc_keys, VAR_NAME v) {
    if (v.empty()) {
        return *proc_keys;
    }
    auto iter = set->find(v);
    if (iter != set->end())  {
        return *(iter->second);
    }
    return PROC_NAME_SET();
}

PROC_NAME_SET RelationManager::GetProcProcRelationVal(PROC_PROC_RELATION_TABLE *set, PROC_NAME_SET *proc_keys, PROC_NAME p) {
    if (p.empty()) {
        return *proc_keys;
    }
    auto iter = set->find(p);
    if (iter != set->end())  {
        return *(iter->second);
    }
    return PROC_NAME_SET();
}

void RelationManager::InsertStmtStmtTuple(STMT_STMT_PAIR_LIST *set, STMT_IDX s1, STMT_IDX s2) {
    STMT_STMT_PAIR pair = {s1, s2};
    set->push_back(pair);
}
void RelationManager::InsertStmtVarTuple(STMT_VAR_PAIR_LIST *set, STMT_IDX s, VAR_NAME v) {
    STMT_VAR_PAIR pair = {s, v};
    set->push_back(pair);
}
void RelationManager::InsertProcVarTuple(PROC_VAR_PAIR_LIST *set, PROC_NAME p, VAR_NAME v) {
    PROC_VAR_PAIR pair = {p, v};
    set->push_back(pair);
}
void RelationManager::InsertProcProcTuple(PROC_PROC_PAIR_LIST *set, PROC_NAME p1, PROC_NAME p2) {
    PROC_PROC_PAIR pair = {p1, p2};
    set->push_back(pair);
}
bool RelationManager::InsertStmtKey(STMT_IDX_SET *set, STMT_IDX s) {
    return set->insert(s).second;
}
bool RelationManager::InsertProcKey(PROC_NAME_SET *set, PROC_NAME p) {
    return set->insert(p).second;
}
bool RelationManager::InsertVarKey(VAR_NAME_SET *set, VAR_NAME v) {
    return set->insert(v).second;
}

















