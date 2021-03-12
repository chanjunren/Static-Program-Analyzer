//
// Created by Xu Lin on 5/10/20.
//
#include <iostream>
#include "CFG.h"

using namespace std;
bool CFG::AddAllNextPairs() {
    for (auto e: data_) {
        auto node = e.first;
        auto neighbors = e.second;
        for (auto n: *neighbors) {
            all_edges_.push_back({node, n});
        }
    }
    return true;
}
bool CFG::AddEdge(STMT_IDX s1, STMT_IDX s2) {
    auto result = AddEdgeUtil(data_, s1, s2);
    if (result) {
        AddKey(s1);
    }
    return result;
}
bool CFG::AddInverseEdge(STMT_IDX s1, STMT_IDX s2) {
    auto result = AddEdgeUtil(inverse_data_, s1, s2);
    if (result) {
        AddInverseKey(s1);
    }
    return result;
}
bool CFG::AddKey(STMT_IDX s) {
    return AddKeyUtil(data_keys_, s);
}
bool CFG::AddInverseKey(STMT_IDX s) {
    return AddKeyUtil(inverse_data_keys_, s);
}
STMT_IDX_SET CFG::GetAllAdjacencyListKeys() {
    return data_keys_;
}
STMT_IDX_SET CFG::GetAllInverseAdjacencyListKeys() {
    return inverse_data_keys_;
}
STMT_IDX_SET CFG::GetAllNeighbors(STMT_IDX s1) {
    auto iter = data_.find(s1);
    if(iter != data_.end()) {
        return *(iter->second);
    }
    return STMT_IDX_SET();
}
STMT_IDX_SET CFG::GetAllInverseNeighbors(STMT_IDX s1) {
    auto iter = inverse_data_.find(s1);
    if(iter != inverse_data_.end()) {
        return *(iter->second);
    }
    return STMT_IDX_SET();
}
bool CFG::HasEdge(STMT_IDX s1, STMT_IDX s2) {
    return HasEdgeUtil(data_, s1, s2);
}
bool CFG::HasInverseEdge(STMT_IDX s1, STMT_IDX s2) {
    return HasEdgeUtil(inverse_data_, s1, s2);
}
bool CFG::RemoveEdge(STMT_IDX s1, STMT_IDX s2) {
    auto iter = data_.find(s1);
    if (iter != data_.end()) {
        auto result = iter->second->erase(s2);
        if (iter->second->empty()) {
            data_.erase(iter);
            return result && data_.find(s1) == data_.end();
        }
        return result;
    }
    return false;
}
CFG_ADJACENCY_LIST& CFG::GetAdjacencyList() {
    return data_;
}
CFG_ADJACENCY_LIST& CFG::GetInverseAdjacencyList() {
    return inverse_data_;
}
bool CFG::AddEdgeUtil(CFG_ADJACENCY_LIST &table, STMT_STMT_PAIR_LIST& pairs, STMT_IDX s1, STMT_IDX s2) {
    auto iter = table.find(s1);
    bool result = true;
    if (iter == table.end()) {
        auto set = new STMT_IDX_SET();
        result = set->insert(s2).second && table.insert({ s1, set }).second;
    } else {
        auto set = iter->second;
        result = set->insert(s2).second;
    }
    if (result) {
        STMT_STMT_PAIR pair = {s1, s2};
        pairs.push_back(pair);
    }
    return result;
}
bool CFG::AddEdgeUtil(CFG_ADJACENCY_LIST &table, STMT_IDX s1, STMT_IDX s2) {
    auto iter = table.find(s1);
    if (iter == table.end()) {
        auto set = new STMT_IDX_SET();
        return set->insert(s2).second && table.insert({ s1, set }).second;
    } else {
        auto set = iter->second;
        return set->insert(s2).second;
    }
}
bool CFG::HasEdgeUtil(CFG_ADJACENCY_LIST &table, STMT_IDX s1, STMT_IDX s2) {
    auto pair = table.find(s1);
    if (pair == table.end()) {
        return false;
    }
    return pair->second->find(s2) != pair->second->end();
}
bool CFG::AddKeyUtil(STMT_IDX_SET &keys, STMT_IDX s) {
    return keys.insert(s).second;
}
STMT_STMT_PAIR_LIST CFG::GetAllEdeges() {
    return all_edges_;
}

