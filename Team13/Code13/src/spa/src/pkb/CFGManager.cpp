//
// Created by Xu Lin on 9/10/20.
//
#include <stack>
#include <iostream>
#include "CFGManager.h"
void CFGManager::SetCFG(CFG& cfg) {
    cfg_ = cfg;
    cfg_.AddAllNextPairs();
    AddAllInverseNext();
}

void CFGManager::AddAllInverseNext() {
    auto graph = cfg_.GetAdjacencyList();
    for (auto& element: graph) {
        auto node = element.first;
        auto neighbors = element.second;
        for (auto neighbor: *neighbors) {
            cfg_.AddInverseEdge(neighbor, node);
        }
    }
}

STMT_IDX_SET CFGManager::GetNext(STMT_IDX s) {
    if (s < 0) {
        return cfg_.GetAllInverseAdjacencyListKeys();
    }
    return cfg_.GetAllNeighbors(s);
}
STMT_IDX_SET CFGManager::GetNextStar(STMT_IDX s) {
    if (s < 0) {
        return cfg_.GetAllInverseAdjacencyListKeys();
    }
    STMT_IDX_SET *result = nullptr;
    auto cache_iter = next_star_cache_.find(s);
    if (cache_iter == next_star_cache_.end()) {
        result = DFS(cfg_.GetAdjacencyList(), s);
        InsertCache(next_star_cache_, all_next_star_edge_cache_, result, s);
    } else {
        result = cache_iter->second;
    }
    //debug
//    std::cout << "DFS next star" << std::endl;
//    for (auto element: *result) {
//        std::cout << element << " ";
//    }
    return *result;
}
STMT_IDX_SET CFGManager::GetInverseNext(STMT_IDX s) {
    if (s < 0) {
        return cfg_.GetAllAdjacencyListKeys();
    }
    return cfg_.GetAllInverseNeighbors(s);
}
STMT_IDX_SET CFGManager::GetInverseNextStar(STMT_IDX s) {
    if (s < 0) {
        return cfg_.GetAllAdjacencyListKeys();
    }
    STMT_IDX_SET *result = nullptr;
    auto cache_iter = inverse_next_star_cache_.find(s);
    if (cache_iter == inverse_next_star_cache_.end()) {
        result = DFS(cfg_.GetInverseAdjacencyList(), s);
        InsertCache(inverse_next_star_cache_, result, s);
    } else {
        result = cache_iter->second;
    }
    //debug
//    std::cout << "DFS inverse next star" << std::endl;
//    for (auto element: *result) {
//        std::cout << element << " ";
//    }
    return *result;
}

bool CFGManager::IsNext(STMT_IDX s1, STMT_IDX s2) {
    auto table = cfg_.GetAdjacencyList();
    auto inverse_table = cfg_.GetInverseAdjacencyList();
    if (s1 < 0 && s2 < 0) {
        return !table.empty();
    }
    if (s1 < 0) {
        auto result = inverse_table.find(s2);
        return result != inverse_table.end() && !result->second->empty();
    }
    if (s2 < 0) {
        auto result = table.find(s1);
        return result != table.end() && !result->second->empty();
    }
    auto iter = table.find(s1);
    return iter != table.end() && !iter->second->empty() && iter->second->find(s2) != iter->second->end();
}
bool CFGManager::IsNextStar(STMT_IDX s1, STMT_IDX s2) {
    auto table = cfg_.GetAdjacencyList();
    auto inverse_table = cfg_.GetInverseAdjacencyList();
    if (s1 < 0 && s2 < 0) {
        return !table.empty();
    }
    if (s1 < 0) {
        auto result = inverse_table.find(s2);
        return result != inverse_table.end() && !result->second->empty();
    }
    if (s2 < 0) {
        auto result = table.find(s1);
        return result != table.end() && !result->second->empty();
    }
    auto cache_iter = next_star_cache_.find(s1);
    if (cache_iter != next_star_cache_.end()) {
        return !cache_iter->second->empty() && cache_iter->second->find(s2) != cache_iter->second->end();
    } else {
        auto next_star_of_s1 = GetNextStar(s1);
        return next_star_of_s1.find(s2) != next_star_of_s1.end();
    }
}

STMT_STMT_PAIR_LIST CFGManager::GetAllNext() {
    return cfg_.GetAllEdeges();
}
STMT_STMT_PAIR_LIST CFGManager::GetAllNextWithSameSynonyms() {
    auto result = STMT_STMT_PAIR_LIST();
    return GetAllPairsWithSameSynonyms(cfg_.GetAdjacencyList());
}
STMT_STMT_PAIR_LIST CFGManager::GetAllNextStar() {
    FillNextStarTable();
    return all_next_star_edge_cache_;
}

STMT_STMT_PAIR_LIST CFGManager::GetAllNextStarWithSameSynonyms() {
    FillNextStarTable();
    return GetAllPairsWithSameSynonyms(next_star_cache_);
}

STMT_IDX_SET* CFGManager::DFS(CFG_ADJACENCY_LIST& table, STMT_IDX s) {
    auto result = new STMT_IDX_SET();
    auto stack = std::stack<STMT_IDX>();
    auto value = table.find(s);
    if (value == table.end()) {
        return result;
    }
    auto neighbors = value->second;
    for (auto n: *neighbors) {
        stack.push(n);
        result->insert(n);
    }
    while (!stack.empty()){
        s = stack.top();
        stack.pop();
        value = table.find(s);
        if (value == table.end()) {
            continue;
        }
        neighbors = value->second;
        for (auto n: *neighbors) {
            //node has not been visited yet
            if (result->find(n) == result->end()) {
                stack.push(n);
                result->insert(n);
            }
        }
    }
    //debug
//    for (auto element: *result) {
//        std::cout << element << " ";
//    }
    return result;
}
void CFGManager::InsertCache(CFG_ADJACENCY_LIST &table, STMT_STMT_PAIR_LIST &edges, STMT_IDX_SET *result, STMT_IDX s) {
    if (result->empty()) {
        return;
    }
    auto value = table.find(s);
    if (value == table.end()) {
        if (table.insert({s, result}).second) {
            for (auto node: *result) {
                STMT_STMT_PAIR pair = {s, node};
                edges.push_back(pair);
            }
        }
    } else {
        for (auto node: *result) {
            if (table.at(s)->insert(node).second) {
                STMT_STMT_PAIR pair = {s, node};
                edges.push_back(pair);
            }
        }
    }
}
void CFGManager::InsertCache(CFG_ADJACENCY_LIST &table, STMT_IDX_SET *result, STMT_IDX s) {
    if (result->empty()) {
        return;
    }
    auto value = table.find(s);
    if (value == table.end()) {
        table.insert({s, result});
    } else {
        for (auto node: *result) {
            table.at(s)->insert(node);
        }
    }
}
CFG &CFGManager::GetCFG() {
    return cfg_;
}
void CFGManager::FillNextStarTable() {
    auto nodes = cfg_.GetAllAdjacencyListKeys();
    for (auto node : nodes) {
        auto cache_iter = next_star_cache_.find(node);
        if (cache_iter == next_star_cache_.end()) {
            auto next_star_of_node = GetNextStar(node);
        }
    }
}
STMT_STMT_PAIR_LIST CFGManager::GetAllPairsWithSameSynonyms(CFG_ADJACENCY_LIST& adjacency_list) {
    auto result = STMT_STMT_PAIR_LIST();
    for (auto iter : adjacency_list) {
        auto s1 = iter.first;
        auto set = iter.second;
        if (set->find(s1) != set->end()) {
            result.push_back({ s1, s1 });
        }
    }
    return result;
}
void CFGManager::ClearCache() {
    next_star_cache_.clear();
    inverse_next_star_cache_.clear();
    all_next_star_edge_cache_.clear();
}


