#ifndef CFG_H
#define CFG_H

#include <unordered_map>
#include <CustomTypes.h>

class CFG {
private:
    CFG_ADJACENCY_LIST data_;
    CFG_ADJACENCY_LIST inverse_data_;

    STMT_IDX_SET data_keys_;
    STMT_IDX_SET inverse_data_keys_;

    STMT_STMT_PAIR_LIST all_edges_;

    bool AddEdgeUtil(CFG_ADJACENCY_LIST &table, STMT_STMT_PAIR_LIST& pairs, STMT_IDX s1, STMT_IDX s2);
    bool AddEdgeUtil(CFG_ADJACENCY_LIST &table, STMT_IDX s1, STMT_IDX s2);
    bool AddKeyUtil(STMT_IDX_SET& keys, STMT_IDX s);
    bool HasEdgeUtil(CFG_ADJACENCY_LIST& table, STMT_IDX s1, STMT_IDX s2);
    bool AddKey(STMT_IDX s);
    bool AddInverseKey(STMT_IDX s);
public:
    CFG_ADJACENCY_LIST& GetAdjacencyList();
    CFG_ADJACENCY_LIST& GetInverseAdjacencyList();
    STMT_IDX_SET GetAllAdjacencyListKeys();
    STMT_IDX_SET GetAllInverseAdjacencyListKeys();
    bool AddEdge(STMT_IDX s1, STMT_IDX s2);
    bool RemoveEdge(STMT_IDX s1, STMT_IDX s2);
    bool AddInverseEdge(STMT_IDX s1, STMT_IDX s2);
    STMT_IDX_SET GetAllNeighbors(STMT_IDX s1);
    STMT_IDX_SET GetAllInverseNeighbors(STMT_IDX s1);
    bool HasEdge(STMT_IDX s1, STMT_IDX s2);
    bool HasInverseEdge(STMT_IDX s1, STMT_IDX s2);
    STMT_STMT_PAIR_LIST GetAllEdeges();
    bool AddAllNextPairs();
};


#endif // CFG_H