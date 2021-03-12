#include <queue>
#include <string>


#include <PKB.h>
#include <TNode.h>

DataManager PKB::data_manager_;
RelationManager PKB::relation_manager_;
PatternManager PKB::pattern_manager_;
CFGManager PKB::cfg_manager_;
PKB::AffectsManager PKB::affects_manager_;
TNode PKB::ast_;
DataManager PKB::GetDataManager() {
    return data_manager_;
}
RelationManager PKB::GetRelationManager() {
    return relation_manager_;
}
PatternManager PKB::GetPatternManager() {
    return pattern_manager_;
}
void PKB::SetASTRoot(TNode* root) {
    if (root->GetNodeType() != TNode::NODE_TYPE::program) {
        throw ASTRootTypeUnmatchException();
    }
    ast_ = *root;
}

TNode PKB::GetASTRoot() {
    return ast_;
}
CFG& PKB::GetCFG() {
    return cfg_manager_.GetCFG();
}
void PKB::SetCFG(CFG& cfg) {
    cfg_manager_.SetCFG(cfg);
}
CFGManager PKB::GetCFGManager() {
    return cfg_manager_;
}
PKB::AffectsManager PKB::GetAffectsManager() {
    return affects_manager_;
}
void PKB::ClearCache() {
    cfg_manager_.ClearCache();
    affects_manager_.ClearCache();
}

STMT_IDX_SET PKB::AffectsManager::affects_computed_set_;
STMT_IDX_SET PKB::AffectsManager::inverse_affects_computed_set_;
AFFECTS_TABLE PKB::AffectsManager::affects_table_;
INVERSE_AFFECTS_TABLE PKB::AffectsManager::inverse_affects_table_;
STMT_IDX_SET PKB::AffectsManager::all_affects_keys_;
STMT_IDX_SET PKB::AffectsManager::all_inverse_affects_keys_;
STMT_STMT_PAIR_LIST PKB::AffectsManager::all_affects_;

STMT_IDX_SET PKB::AffectsManager::affects_star_computed_set_;
STMT_IDX_SET PKB::AffectsManager::inverse_affects_star_computed_set_;
AFFECTS_STAR_TABLE PKB::AffectsManager::affects_star_table_;
INVERSE_AFFECTS_STAR_TABLE PKB::AffectsManager::inverse_affects_star_table_;
STMT_STMT_PAIR_LIST PKB::AffectsManager::all_affects_star_;


STMT_IDX_SET PKB::FilterStmtTypes(STMT_IDX_SET unfiltered, STATEMENT_TYPE type) {
    auto result = STMT_IDX_SET();
    if (type == STATEMENT_TYPE::allStatement) {
        return unfiltered;
    }
    for (auto s : unfiltered) {
        if (data_manager_.IsStmt(type, s)) {
            result.insert(s);
        }
    }
    return result;
}
STMT_STMT_PAIR_LIST PKB::FilterStmtTypes(STMT_STMT_PAIR_LIST unfiltered, STATEMENT_TYPE type1, STATEMENT_TYPE type2) {
    if (type1 == STATEMENT_TYPE::allStatement && type2 == STATEMENT_TYPE::allStatement) {
        return unfiltered;
    }
    auto result = STMT_STMT_PAIR_LIST();
    if (type1 == STATEMENT_TYPE::allStatement) {
        for (auto pair : unfiltered) {
            if (data_manager_.IsStmt(type2, pair.s2)) {
                result.push_back(pair);
            }
        }

    } else if (type2 == STATEMENT_TYPE::allStatement) {
        for (auto pair : unfiltered) {
            if (data_manager_.IsStmt(type1, pair.s1)) {
                result.push_back(pair);
            }
        }
    } else {
        for (auto pair : unfiltered) {
            if (data_manager_.IsStmt(type1, pair.s1) && data_manager_.IsStmt(type2, pair.s2)) {
                result.push_back(pair);
            }
        }
    }
    return result;
}
STMT_VAR_PAIR_LIST PKB::FilterStmtTypes(STMT_VAR_PAIR_LIST unfiltered, STATEMENT_TYPE type) {
    if (type == STATEMENT_TYPE::allStatement) {
        return unfiltered;
    }
    auto result = STMT_VAR_PAIR_LIST();
    for (auto pair : unfiltered) {
        if (data_manager_.IsStmt(type, pair.s)) {
            result.push_back(pair);
        }
    }
    return result;
}

STMT_STMT_PAIR_LIST PKB::AffectsManager::GetAllAffects() {
    FillAffectsTable();
    return all_affects_;
}

STMT_STMT_PAIR_LIST PKB::AffectsManager::GetAllAffectsWithSameSynonyms() {
    FillAffectsTable();
    return GetAllPairsWithSameSynonyms(affects_table_);
}

bool PKB::AffectsManager::IsAffects(STMT_IDX a1, STMT_IDX a2) {
    //If both a1 and a2 are negative values, return true if exists any Affects relation, else return false
    if (a1 < 0 && a2 < 0) {
        if (!affects_table_.empty()) {
            return true;
        }
        STMT_IDX_SET assign_stmts = data_manager_.GetAllStatements(STATEMENT_TYPE::assignStatement);
        for (auto a : assign_stmts) {
            if (affects_computed_set_.find(a) == affects_computed_set_.end()) {
                auto result = GetAffects(a);
                if (!result.empty()) {
                    return true;
                }
            }
        }
        return false;
    }
    //If a1 < 0 but a2 > 0, return true if exists any Affects(_, a2) relation
    if (a1 < 0) {
        if (inverse_affects_table_.find(a2) != inverse_affects_table_.end()) {
            return true;
        }
        if (!GetInverseAffects(a2).empty()) {
            return true;
        }
        return false;
    }
    //If a1 > 0 and a2 < 0, return true if exists any Affects(a1, _) relation
    if (a2 < 0) {
        if (affects_table_.find(a1) != affects_table_.end()) {
            return true;
        }
        if (!GetAffects(a1).empty()) {
            return true;
        }
        return false;
    }
    auto result = GetAffects(a1);
    if (result.find(a2) != result.end()) {
        return true;
    }
    return false;
}
STMT_IDX_SET PKB::AffectsManager::GetAffects(STMT_IDX a) {
    //debug
    if (PKB_DEBUG) {
        std::cout << "PKB: GetAffects(" << a << ")" << std::endl;
    }
    if (a < 0) {
        return GetAllInverseAffectsKeys();
    }
    if (!data_manager_.IsAssignStmt(a)) {
        return STMT_IDX_SET();
    }
    if (affects_table_.find(a) != affects_table_.end()) {
        return *affects_table_.at(a);
    }
    if (affects_computed_set_.find(a) != affects_computed_set_.end()) {
        return STMT_IDX_SET();
    }  
    auto lhs_var = *(relation_manager_.GetStmtModifies(a).begin());
    auto visited = new STMT_IDX_SET();
    auto result = new STMT_IDX_SET();
    auto neighbors = cfg_manager_.GetNext(a);
    for (auto neighbor : neighbors) {
        RecursiveGetAffects(neighbor, lhs_var, *visited, *result);
    }
    if (result->size() > 0) {
        affects_table_.insert({ a, result });
        for (auto a1 : *result) {
            all_affects_.push_back({ a, a1 });
        }
    }
    //debug

    if (PKB_DEBUG) {
        std::cout << "PKB: printing result from GetAffects(" << a << std::endl;
        for (auto e : *result) {
            std::cout << "" << e << std::endl;
        }
    }
    affects_computed_set_.insert(a);
    return *result;
}

STMT_IDX_SET PKB::AffectsManager::GetInverseAffects(STMT_IDX a) {
    //debug

    if (PKB_DEBUG) {
        std::cout << "PKB:GetInverseAffects(" << a << ")" << std::endl;
    }
    if (a < 0) {
        return GetAllAffectsKeys();
    }
    if (!data_manager_.IsAssignStmt(a)) {
        return STMT_IDX_SET();
    }
    if (inverse_affects_table_.find(a) != inverse_affects_table_.end()) {
        return *inverse_affects_table_.at(a);
    }
    if (inverse_affects_computed_set_.find(a) != inverse_affects_computed_set_.end()) {
        return STMT_IDX_SET();
    }
    auto rhs_vars = relation_manager_.GetStmtUses(a);
    auto visited = new INVERSE_AFFECTS_VISITED_SET();
    auto result = new STMT_IDX_SET();
    auto neighbors = cfg_manager_.GetInverseNext(a);
    for (auto rhs_var : rhs_vars) {
        //debug

        if (PKB_DEBUG) {
            std::cout << "PKB: GetInverseAffects: rhs_var = " << rhs_var << std::endl;
        }
        if (visited->find(rhs_var) == visited->end()) {
            visited->insert({ rhs_var, new STMT_IDX_SET() });
        }
        for (auto neighbor : neighbors) {
            RecursiveGetInverseAffects(neighbor, rhs_var, *visited, *result);
        }
    }
    if (result->size() > 0) {
        inverse_affects_table_.insert({ a, result });
    }
    //debug
    if (PKB_DEBUG) {
        std::cout << "PKB: printing result from GetInverseAffects(" << a << std::endl;
        for (auto e : *result) {
            std::cout << "" << e << std::endl;
        }
    }
    inverse_affects_computed_set_.insert(a);
    return *result;
}

bool PKB::AffectsManager::IsAffectsStar(STMT_IDX a1, STMT_IDX a2) {
    //If both a1 and a2 are negative values, return true if exists any Affects relation, else return false
    if (a1 < 0 && a2 < 0) {
        if (!affects_star_table_.empty() || !affects_table_.empty()) {
            return true;
        }
        STMT_IDX_SET assign_stmts = data_manager_.GetAllStatements(STATEMENT_TYPE::assignStatement);
        for (auto a : assign_stmts) {
            if (affects_computed_set_.find(a) == affects_computed_set_.end()) {
                auto result = GetAffects(a);
                if (!result.empty()) {
                    return true;
                }
            }
        }
        return false;
    }
    //If a1 < 0 but a2 > 0, return true if exists any Affects(_, a2) relation
    if (a1 < 0) {
        if (inverse_affects_star_table_.find(a2) != inverse_affects_star_table_.end()) {
            return true;
        }
        if (!GetInverseAffectsStar(a2).empty()) {
            return true;
        }
        return false;
    }
    //If a1 > 0 and a2 < 0, return true if exists any Affects(a1, _) relation
    if (a2 < 0) {
        if (affects_star_table_.find(a1) != affects_star_table_.end()) {
            return true;
        }
        if (!GetAffectsStar(a1).empty()) {
            return true;
        }
        return false;
    }
    auto result = GetAffectsStar(a1);
    if (result.find(a2) != result.end()) {
        return true;
    }
    return false;
}

STMT_IDX_SET PKB::AffectsManager::GetAffectsStar(STMT_IDX a) {
    if (a < 0) {
        return GetAllInverseAffectsKeys();
    }
    if (affects_star_table_.find(a) != affects_star_table_.end()) {
        return *affects_star_table_.at(a);
    }
    if (affects_star_computed_set_.find(a) != affects_star_computed_set_.end()) {
        return STMT_IDX_SET();
    }
    auto result = new STMT_IDX_SET();
    auto q = std::queue<STMT_IDX>();
    auto visited = STMT_IDX_SET();
    q.push(a);
    visited.insert(a);
    while (!q.empty()) {
        auto node = q.front();
        q.pop();
        auto neighbors = GetAffects(node);
        for (auto neighbor : neighbors) {
            result->insert(neighbor);
            if (visited.find(neighbor) == visited.end()) {
                q.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }
    affects_star_table_.insert({ a, result });
    affects_star_computed_set_.insert(a);
    return *result;
}
STMT_IDX_SET PKB::AffectsManager::GetInverseAffectsStar(STMT_IDX a) {
    if (a < 0) {
        return GetAllAffectsKeys();
    }
    if (inverse_affects_star_table_.find(a) != inverse_affects_star_table_.end()) {
        return *inverse_affects_star_table_.at(a);
    }
    if (inverse_affects_star_computed_set_.find(a) != inverse_affects_star_computed_set_.end()) {
        return STMT_IDX_SET();
    }
    auto result = new STMT_IDX_SET();
    auto q = std::queue<STMT_IDX>();
    auto visited = STMT_IDX_SET();
    q.push(a);
    visited.insert(a);
    while (!q.empty()) {
        auto node = q.front();
        q.pop();
        auto neighbors = GetInverseAffects(node);
        for (auto neighbor : neighbors) {
            result->insert(neighbor);
            if (visited.find(neighbor) == visited.end()) {
                q.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }
    inverse_affects_star_table_.insert({ a, result });
    affects_star_computed_set_.insert(a);
    return *result;
}

STMT_STMT_PAIR_LIST PKB::AffectsManager::GetAllAffectsStar() {
    if (!all_affects_star_.empty()) {
        return all_affects_star_;
    }
    FillAffectsStarTable();
    for (auto pair : affects_star_table_) {
        auto a1 = pair.first;
        for (auto a2 : *(pair.second)) {
            all_affects_star_.push_back({ a1, a2 });
        }
    }
    return all_affects_star_;
}
STMT_STMT_PAIR_LIST PKB::AffectsManager::GetAllAffectsStarWithSameSynonyms() {
    FillAffectsStarTable();
    return GetAllPairsWithSameSynonyms(affects_star_table_);
}
void PKB::AffectsManager::RecursiveGetAffects(STMT_IDX node, VAR_NAME lhs_var, STMT_IDX_SET& visited, STMT_IDX_SET& result) {
    if (visited.find(node) != visited.end()) {
        return;
    }
    //mark as visited
    visited.insert(node);
    //If the node is an assign statement and it uses lhs_var, the statement is affected
    if (data_manager_.IsAssignStmt(node) && relation_manager_.IsStmtUses(node, lhs_var)) {
        result.insert(node);
    }
    if ((data_manager_.IsAssignStmt(node) || data_manager_.IsReadStmt(node)) && relation_manager_.IsStmtModifies(node, lhs_var)) {
        return;
    }
    //If this is an assign statement and it modifies the variable, terminate dfs on path
    if (data_manager_.IsCallStmt(node)) {
        auto procName = data_manager_.GetCalledByStmt(node);
        if (relation_manager_.IsProcModifies(procName, lhs_var)) {
            return;
        }
    }
    auto neighbors = cfg_manager_.GetNext(node);
    for (auto neighbor : neighbors) {
        RecursiveGetAffects(neighbor, lhs_var, visited, result);
    }
}
void PKB::AffectsManager::FillAffectsTable() {
    STMT_IDX_SET assign_stmts = data_manager_.GetAllStatements(STATEMENT_TYPE::assignStatement);
    for (auto a : assign_stmts) {
        if (affects_computed_set_.find(a) == affects_computed_set_.end()) {
            GetAffects(a);
        }
    }
}
void PKB::AffectsManager::FillAffectsStarTable() {
    STMT_IDX_SET assign_stmts = data_manager_.GetAllStatements(STATEMENT_TYPE::assignStatement);
    for (auto a : assign_stmts) {
        if (affects_star_computed_set_.find(a) == affects_star_computed_set_.end()) {
            GetAffectsStar(a);
        }
    }
}
void PKB::AffectsManager::RecursiveGetInverseAffects(STMT_IDX node, VAR_NAME rhs_var, INVERSE_AFFECTS_VISITED_SET& visited, STMT_IDX_SET& result) {
    //debug
    //std::cout << "PKB: RecursiveGetInverseAffects for node " << node << ", rhs_var " << rhs_var << std::endl;
    if (visited.at(rhs_var)->find(node) != visited.at(rhs_var)->end()) {
        return;
    }
    //mark as visited
    visited.at(rhs_var)->insert(node);
    //If the node is an assign statement and it modifies the rhs_var, then node is affected by this statement.
    if (data_manager_.IsAssignStmt(node) && relation_manager_.IsStmtModifies(node, rhs_var)) {
        result.insert(node);
        return;
    }
    if (data_manager_.IsReadStmt(node) && relation_manager_.IsStmtModifies(node, rhs_var)) {
        return;
    }
    //If this is an assign statement and it modifies the variable, terminate dfs on path
    if (data_manager_.IsCallStmt(node)) {
        auto procName = data_manager_.GetCalledByStmt(node);
        if (relation_manager_.IsProcModifies(procName, rhs_var)) {
            return;
        }
    }
    auto neighbors = cfg_manager_.GetInverseNext(node);
    for (auto neighbor : neighbors) {
        RecursiveGetInverseAffects(neighbor, rhs_var, visited, result);
    }
}
STMT_IDX_SET PKB::AffectsManager::GetAllAffectsKeys() {
    if (!all_affects_keys_.empty()) {
        return all_affects_keys_;
    }
    auto assign_stmts = data_manager_.GetAllStatements(STATEMENT_TYPE::assignStatement);
    for (auto assign : assign_stmts) {
        if (affects_computed_set_.find(assign) == affects_computed_set_.end()) {
            GetAffects(assign);
        }
        if (affects_table_.find(assign) != affects_table_.end()) {
            all_affects_keys_.insert(assign);
        }
    }
    return all_affects_keys_;
}
STMT_IDX_SET PKB::AffectsManager::GetAllInverseAffectsKeys() {
    if (!all_inverse_affects_keys_.empty()) {
        return all_inverse_affects_keys_;
    }
    auto assign_stmts = data_manager_.GetAllStatements(STATEMENT_TYPE::assignStatement);
    for (auto assign : assign_stmts) {
        if (inverse_affects_computed_set_.find(assign) == inverse_affects_computed_set_.end()) {
            GetInverseAffects(assign);
        }
        if (inverse_affects_table_.find(assign) != inverse_affects_table_.end()) {
            all_inverse_affects_keys_.insert(assign);
        }
    }
    return all_inverse_affects_keys_;
}
STMT_STMT_PAIR_LIST PKB::AffectsManager::GetAllPairsWithSameSynonyms(STMT_STMT_RELATION_TABLE table) {
    auto result = STMT_STMT_PAIR_LIST();
    for (auto iter : table) {
        auto a1 = iter.first;
        auto set = iter.second;
        if (set->find(a1) != set->end()) {
            result.push_back({ a1, a1 });
        }
    }
    return result;
}
void PKB::AffectsManager::ClearCache() {
    affects_computed_set_.clear();
    inverse_affects_computed_set_.clear();
    affects_table_.clear();
    inverse_affects_table_.clear();
    all_affects_keys_.clear();
    all_inverse_affects_keys_.clear();
    all_affects_.clear();
    affects_star_computed_set_.clear();
    inverse_affects_star_computed_set_.clear();
    affects_star_table_.clear();
    inverse_affects_star_table_.clear();
    all_affects_star_.clear();
}
