//
// Created by Xu Lin on 9/10/20.
//
#include <iostream>
#include <CFGManager.h>
#include "catch.hpp"

TEST_CASE("Unit Test for SetCFG(CFG cfg)") {
    auto cfg = CFG();
    cfg.AddEdge(1, 2);
    cfg.AddEdge(1, 3);
    auto manager = CFGManager();
    manager.SetCFG(cfg);
    REQUIRE(manager.GetNext(1).size() == 2);
}

TEST_CASE("Unit Test for SetInverseEdges") {
    auto cfg = CFG();
    cfg.AddEdge(1, 2);
    cfg.AddEdge(1, 3);
    auto manager = CFGManager();
    manager.SetCFG(cfg);
    REQUIRE(manager.GetInverseNext(2).size() == 1);
    REQUIRE(manager.GetInverseNext(3).size() == 1);
    REQUIRE(manager.GetInverseNext(1).size() == 0);
}

/*TEST_CASE("Unit Test for GetNext(STMT_IDX s)") {
    auto cfg = CFG();
    cfg.AddEdge(1, 2);
    cfg.AddEdge(1, 3);
    cfg.AddEdge(2, 6);
    auto manager = CFGManager();
    manager.SetCFG(cfg);
    REQUIRE(manager.GetNext(1).size() == 2);
    REQUIRE(manager.GetNext(-1).size() == 3);
    REQUIRE(manager.GetNext(-1).find(6) != manager.GetNext(-1).end());
}*/

TEST_CASE("Unit Test for GetInverseNext(STMT_IDX s)") {
    auto cfg = CFG();
    cfg.AddEdge(1, 2);
    cfg.AddEdge(1, 3);
    cfg.AddEdge(2, 6);
    auto manager = CFGManager();
    manager.SetCFG(cfg);
    REQUIRE(manager.GetInverseNext(1).size() == 0);
    REQUIRE(manager.GetInverseNext(2).size() == 1);
    REQUIRE(manager.GetInverseNext(3).size() == 1);
    REQUIRE(manager.GetInverseNext(-1).size() == 2);
}

TEST_CASE("Unit Test for GetNextStar(STMT_IDX s)") {
    auto cfg = CFG();
    cfg.AddEdge(1, 2);
    cfg.AddEdge(1, 3);
    cfg.AddEdge(2, 6);
    cfg.AddEdge(3, 9);
    cfg.AddEdge(9, 10);
    cfg.AddEdge(10, 3);
    cfg.AddEdge(10, 1);
    auto manager = CFGManager();
    manager.SetCFG(cfg);
    auto set = manager.GetNextStar(1);
    REQUIRE(manager.GetNextStar(1).size() == 6);
    REQUIRE(manager.GetNextStar(-1).size() == 6);
    REQUIRE(manager.GetNextStar(2).size() == 1);
    REQUIRE(manager.GetNextStar(6).size() == 0);
    REQUIRE(manager.GetNextStar(10).size() == 6);
}

/*TEST_CASE("Unit Test for GetInverseNextStar(STMT_IDX s)") {
    auto cfg = CFG();
    cfg.AddEdge(1, 2);
    cfg.AddEdge(1, 3);
    cfg.AddEdge(2, 6);
    cfg.AddEdge(3, 9);
    cfg.AddEdge(9, 10);
    cfg.AddEdge(10, 3);
    cfg.AddEdge(10, 1);
    auto manager = CFGManager();
    manager.SetCFG(cfg);
    auto set = manager.GetInverseNextStar(1);
    REQUIRE(manager.GetInverseNextStar(1).size() == 4);
    REQUIRE(manager.GetInverseNextStar(6).size() == 5);
    REQUIRE(manager.GetInverseNextStar(-1).size() == 5);
    REQUIRE(manager.GetInverseNextStar(-1).find(6) == manager.GetInverseNextStar(-1).end());
}*/

TEST_CASE("Unit Test for IsNext(STMT_IDX s1, STMT_IDX s2)") {
    auto cfg = CFG();
    cfg.AddEdge(1, 2);
    cfg.AddEdge(1, 3);
    cfg.AddEdge(2, 6);
    cfg.AddEdge(3, 9);
    cfg.AddEdge(9, 10);
    cfg.AddEdge(10, 3);
    cfg.AddEdge(10, 1);
    auto manager = CFGManager();
    manager.SetCFG(cfg);
    REQUIRE(manager.IsNext(-1, -1));
    REQUIRE(!manager.IsNext(1, 1));
    REQUIRE(manager.IsNext(3, 9));
    REQUIRE(manager.IsNext(1, -1));
    REQUIRE(!manager.IsNext(6, -1));
    REQUIRE(manager.IsNext(-1, 6));
}

TEST_CASE("Unit Test for IsNextStar(STMT_IDX s1, STMT_IDX s2)") {
    auto cfg = CFG();
    cfg.AddEdge(1, 2);
    cfg.AddEdge(1, 3);
    cfg.AddEdge(2, 6);
    cfg.AddEdge(3, 9);
    cfg.AddEdge(9, 10);
    cfg.AddEdge(10, 3);
    cfg.AddEdge(10, 1);
    auto manager = CFGManager();
    manager.SetCFG(cfg);
    REQUIRE(manager.IsNextStar(-1, -1));
    REQUIRE(manager.IsNextStar(1, 1));
    REQUIRE(manager.IsNextStar(3, 2));
    REQUIRE(manager.IsNextStar(1, -1));
    REQUIRE(!manager.IsNextStar(6, -1));
    REQUIRE(manager.IsNextStar(-1, 6));
}

TEST_CASE("Unit Test for GetAllNext()") {
    auto cfg = CFG();
    cfg.AddEdge(1, 2);
    cfg.AddEdge(1, 3);
    cfg.AddEdge(2, 6);
    cfg.AddEdge(3, 9);
    cfg.AddEdge(9, 10);
    cfg.AddEdge(10, 3);
    cfg.AddEdge(10, 1);
    auto manager = CFGManager();
    manager.SetCFG(cfg);
    REQUIRE(manager.GetAllNext().size() == 7);
    auto set = manager.GetAllNext();
    for (auto entry: set) {
        std::cout << "s1: " << entry.s1 << ", s2: " << entry.s2 << std::endl;
    }
}

TEST_CASE("Unit Test for GetAllNextStar()") {
    auto cfg = CFG();
    cfg.AddEdge(1, 2);
    cfg.AddEdge(1, 3);
    cfg.AddEdge(2, 6);
    cfg.AddEdge(3, 9);
    cfg.AddEdge(9, 10);
    cfg.AddEdge(10, 3);
    cfg.AddEdge(10, 1);
    auto manager = CFGManager();
    manager.SetCFG(cfg);
    auto set = manager.GetAllNextStar();
    for (auto entry: set) {
        std::cout << "s1: " << entry.s1 << ", s2: " << entry.s2 << std::endl;
    }
}