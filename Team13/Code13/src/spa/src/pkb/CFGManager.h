#ifndef CFGMANAGER_H
#define CFGMANAGER_H

#include <CFG.h>
class CFGManager {
private:
    CFG cfg_;
    CFG_ADJACENCY_LIST next_star_cache_;
    CFG_ADJACENCY_LIST inverse_next_star_cache_;
    STMT_STMT_PAIR_LIST all_next_star_edge_cache_;
    STMT_IDX_SET* DFS(CFG_ADJACENCY_LIST& table, STMT_IDX s);
    void AddAllInverseNext();
    void InsertCache(CFG_ADJACENCY_LIST &table, STMT_STMT_PAIR_LIST &edges, STMT_IDX_SET *result, STMT_IDX s);
    void InsertCache(CFG_ADJACENCY_LIST &table, STMT_IDX_SET *result, STMT_IDX s);
    STMT_STMT_PAIR_LIST GetAllPairsWithSameSynonyms(CFG_ADJACENCY_LIST& adjacency_list);
public:
    void SetCFG(CFG& cfg);
    CFG& GetCFG();
    STMT_IDX_SET GetNext(STMT_IDX s);
    STMT_IDX_SET GetNextStar(STMT_IDX s);
    STMT_IDX_SET GetInverseNext(STMT_IDX s);
    STMT_IDX_SET GetInverseNextStar(STMT_IDX s);

    bool IsNext(STMT_IDX s1, STMT_IDX s2);
    bool IsNextStar(STMT_IDX s1, STMT_IDX s2);

    STMT_STMT_PAIR_LIST GetAllNext();
    STMT_STMT_PAIR_LIST GetAllNextWithSameSynonyms();
    STMT_STMT_PAIR_LIST GetAllNextStar();
    STMT_STMT_PAIR_LIST GetAllNextStarWithSameSynonyms();

    void FillNextStarTable();
    void ClearCache();

};

#endif // CFGMANAGER_H