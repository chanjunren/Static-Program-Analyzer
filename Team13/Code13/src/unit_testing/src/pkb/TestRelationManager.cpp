//
// Created by Xu Lin on 14/9/20.
//

#include "catch.hpp"
#include "PKB.h"
#include "CustomTypes.h"
#include "RelationManager.h"

TEST_CASE("PKB RelationManager initialisation check") {
    PKB pkb = PKB();
    RelationManager relation_manager = pkb.GetRelationManager();
}

TEST_CASE("RelationManager::AddFollows(STMT_IDX s1, STMT_IDX s2) unit test") {

    std::cout << "hey" << std::endl;
    PKB pkb = PKB();
    RelationManager relation_manager = pkb.GetRelationManager();
    relation_manager.AddFollows(1, 2);
    relation_manager.IsFollows(1, 2);
    REQUIRE(relation_manager.IsFollows(1, 2));
    REQUIRE(relation_manager.IsFollowsStar(1, 2));
}

TEST_CASE("RelationManager::IsFollows(STMT_IDX s1, STMT_IDX s2) unit test") {
    PKB pkb = PKB();
    RelationManager relation_manager = pkb.GetRelationManager();
    relation_manager.AddFollows(1, 2);
    REQUIRE(relation_manager.IsFollows(1, 2));
    REQUIRE(relation_manager.IsFollows(-1, -1));
    REQUIRE(relation_manager.IsFollows(-1, 2));
    REQUIRE(relation_manager.IsFollows(1, -1));
}

TEST_CASE("RelationManager::IsFollowsStar(STMT_IDX s1, STMT_IDX s2) unit test") {
    PKB* pkb = new PKB();
    RelationManager relation_manager = pkb->GetRelationManager();
    relation_manager.AddFollows(1, 10);
    relation_manager.AddFollowsStar(1, 20);
    REQUIRE(relation_manager.IsFollowsStar(-1, -1));
    REQUIRE(relation_manager.IsFollowsStar(1, 10));
    REQUIRE(relation_manager.IsFollowsStar(-1, 10));
    REQUIRE(relation_manager.IsFollowsStar(1, -1));
    REQUIRE(relation_manager.IsFollowsStar(1, 20));
    REQUIRE(relation_manager.IsFollowsStar(-1, 20));
    pkb = nullptr;
}

TEST_CASE("RelationManager::GetAllFollows() unit test") {
    PKB* pkb = new PKB();
    RelationManager relation_manager = pkb->GetRelationManager();
    STMT_STMT_PAIR p1 = {1, 2};
    STMT_STMT_PAIR p2 = {2, 3};
    relation_manager.AddFollows(p1.s1, p1.s2);
    relation_manager.AddFollows(p2.s1, p2.s2);
    STMT_STMT_PAIR_LIST all_follows = relation_manager.GetAllFollows();
    bool res1, res2;
    for (STMT_STMT_PAIR p: all_follows) {
        if (p.s1 == p1.s1 && p.s2 == p1.s2) {
            res1 = true;
        }
        if (p.s1 == p2.s1 && p.s2 == p2.s2) {
            res2 = true;
        }
    }
    STMT_STMT_PAIR_LIST all_follows_star = relation_manager.GetAllFollowsStars();
    for (STMT_STMT_PAIR p: all_follows_star) {
        std::cout << p.s1 << ", " << p.s2 << std::endl;
    }
    REQUIRE(res1);
    REQUIRE(res2);
    REQUIRE(all_follows.size() == 2);
    pkb = nullptr;
}

TEST_CASE("RelationManager::GetAllFollowsStar() unit test") {
    PKB* pkb = new PKB();
    RelationManager relation_manager = pkb->GetRelationManager();
    STMT_STMT_PAIR p1 = {1, 2}, p2 = {2, 3}, p3 = {1, 5};
    relation_manager.AddFollows(1, 2);
    relation_manager.AddFollows(2, 3);
    relation_manager.AddFollowsStar(1, 5);
    STMT_STMT_PAIR_LIST all_follows_star = relation_manager.GetAllFollowsStars();
    for (STMT_STMT_PAIR p: all_follows_star) {
        std::cout << p.s1 << ", " << p.s2 << std::endl;
    }
    pkb = nullptr;
}

TEST_CASE("RelationManager::GetFollows(STMT_IDX s) unit test") {
    PKB* pkb = new PKB();
    RelationManager relation_manager = pkb->GetRelationManager();
    relation_manager.AddFollows(1, 2);
    relation_manager.AddFollows(1, 5);
    STMT_IDX_SET set1 = relation_manager.GetFollows(1);
    REQUIRE(set1.size() == 2);
    REQUIRE(set1.find(2) != set1.end());
    REQUIRE(set1.find(5) != set1.end());
    STMT_IDX_SET set_wildcard = relation_manager.GetFollows(-1);
    for (STMT_IDX s: set_wildcard) {
        std::cout << s << std::endl;
    }
    REQUIRE(!set_wildcard.empty());
    REQUIRE(set_wildcard.find(1) == set_wildcard.end());
    REQUIRE(set_wildcard.find(5) != set_wildcard.end());
    pkb = nullptr;
}

TEST_CASE("RelationManager::GetInverseFollows(STMT_IDX s) unit test") {
    PKB* pkb = new PKB();
    RelationManager relation_manager = pkb->GetRelationManager();
    STMT_STMT_PAIR p1, p2;
    relation_manager.AddFollows(2, 5);
    relation_manager.AddFollows(3, 5);
    STMT_IDX_SET set = relation_manager.GetInverseFollows(5);
    REQUIRE(set.size() == 2);
    REQUIRE(set.find(2) != set.end());
    REQUIRE(set.find(3) != set.end());
    STMT_IDX_SET set_wildcard = relation_manager.GetInverseFollows(-1);
    REQUIRE(!set_wildcard.empty());
    REQUIRE(set_wildcard.find(5) == set_wildcard.end());
    REQUIRE(set_wildcard.find(2) != set_wildcard.end());
    REQUIRE(set_wildcard.find(3) != set_wildcard.end());
    pkb = nullptr;
}

TEST_CASE("RelationManager::GetInverseFollowsStar(STMT_IDX s) unit test") {
    PKB* pkb = new PKB();
    RelationManager relation_manager = pkb->GetRelationManager();
    relation_manager.AddFollows(2, 5);
    relation_manager.AddFollows(3, 5);
    relation_manager.AddFollows(4, 5);
    STMT_IDX_SET set = relation_manager.GetInverseFollowsStars(5);
    REQUIRE(set.size() == 3);
    REQUIRE(set.find(2) != set.end());
    REQUIRE(set.find(3) != set.end());
    REQUIRE(set.find(4) != set.end());
    pkb = nullptr;
}

TEST_CASE("RelationManager::AddParent(STMT_IDX s1, STMT_IDX s2) unit test") {

}
