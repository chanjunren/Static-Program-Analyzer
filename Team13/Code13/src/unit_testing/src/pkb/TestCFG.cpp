//
// Created by Xu Lin on 5/10/20.
//

#include <CFG.h>
#include "catch.hpp"

TEST_CASE("Unit Test for AddEdge()") {
    CFG *cfg = new CFG();
    cfg->AddEdge(1, 2);
    REQUIRE(cfg->HasEdge(1, 2));
}

TEST_CASE("Unit Test for AddInverseEdge()") {
    CFG *cfg = new CFG();
    cfg->AddInverseEdge(1, 2);
    REQUIRE(cfg->HasInverseEdge(1, 2));
    cfg->AddInverseEdge(1, 3);
    REQUIRE(cfg->HasInverseEdge(1, 3));
    REQUIRE(cfg->GetAllInverseNeighbors(1).size() == 2);
}

TEST_CASE("Unit Test for RemoveEdge(STMT_IDX s1, STMT_IDX s2)") {
    CFG *cfg = new CFG();
    cfg->AddEdge(1, 2);
    cfg->AddEdge(1, 3);
    cfg->RemoveEdge(1, 2);
    REQUIRE(cfg->GetAllNeighbors(1).size() == 1);
    cfg->RemoveEdge(1, 3);
    REQUIRE(cfg->GetAdjacencyList().empty());
    REQUIRE(!cfg->RemoveEdge(1, 2));
}