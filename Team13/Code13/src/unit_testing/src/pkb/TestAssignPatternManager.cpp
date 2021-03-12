//
// Created by Xu Lin on 22/9/20.
//


#include <PKB.h>
#include "catch.hpp"
#include "CustomTypes.h"
#include "TestUtils.h"

TEST_CASE("Test AddAssignPattern(STMT_IDX s, VAR_NAME v, TNode root) with single assignment") {
    PKB* pkb = new PKB();
    PatternManager manager = pkb->GetPatternManager();
    TNode* prog = new TNode(TNode::NODE_TYPE::program, "SIMPLE");
    TNode* proc = new TNode(TNode::NODE_TYPE::procedure);
    TNode* proc_name = new TNode(TNode::NODE_TYPE::procName, "main");
    TNode* stmt_list = new TNode(TNode::NODE_TYPE::stmtList);
    TNode* assign1 = new TNode(TNode::NODE_TYPE::assignStmt, 1);
    TNode* var1 = new TNode(TNode::NODE_TYPE::varName, "var1");
    TNode* expr1 = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
    TNode* var2 = new TNode(TNode::NODE_TYPE::varName, "var2");
    TNode* const1 = new TNode(TNode::NODE_TYPE::constValue, 5.0);

    prog->AddChild(proc);
    proc->AddChild(proc_name);
    proc->AddChild(stmt_list);
    stmt_list->AddChild(assign1);
    assign1->AddChild(var1);
    assign1->AddChild(expr1);
    expr1->AddChild(var2);
    expr1->AddChild(const1);
    proc->SetParent(prog);
    proc_name->SetParent(proc);
    stmt_list->SetParent(proc);
    assign1->SetParent(stmt_list);
    var1->SetParent(assign1);
    expr1->SetParent(assign1);
    var2->SetParent(expr1);
    const1->SetParent(expr1);

    manager.AddAssignPattern(1, "var1", *assign1);

    STMT_IDX_SET set =  manager.GetAssignWithSubPattern("var1", "5");
    REQUIRE(!set.empty());
    REQUIRE(set.size() == 1);
    REQUIRE(set.find(1) != set.end());
}

TEST_CASE("Test GetAssignWithSubPattern(STMT_IDX s, VAR_NAME v, TNode root) with two assignment") {
    PKB* pkb = new PKB();
    PatternManager manager = pkb->GetPatternManager();
    TNode* prog = new TNode(TNode::NODE_TYPE::program, "SIMPLE");
    TNode* proc = new TNode(TNode::NODE_TYPE::procedure);
    TNode* proc_name = new TNode(TNode::NODE_TYPE::procName, "main");
    TNode* stmt_list = new TNode(TNode::NODE_TYPE::stmtList);
    TNode* assign1 = new TNode(TNode::NODE_TYPE::assignStmt, 1);
    TNode* var1 = new TNode(TNode::NODE_TYPE::varName, "var1");
    TNode* expr1 = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
    TNode* var2 = new TNode(TNode::NODE_TYPE::varName, "var2");
    TNode* const1 = new TNode(TNode::NODE_TYPE::constValue, 5.0);
    TNode* assign2 = new TNode(TNode::NODE_TYPE::assignStmt, 2);
    TNode* var3 = new TNode(TNode::NODE_TYPE::varName, "var1");
    TNode* expr2 = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
    TNode* const2 = new TNode(TNode::NODE_TYPE::constValue, 1.0);
    TNode* expr3 = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::times);
    TNode* const4 = new TNode(TNode::NODE_TYPE::constValue, 2.0);
    TNode* var4 = new TNode(TNode::NODE_TYPE::varName, "var1");

    prog->AddChild(proc);
    proc->AddChild(proc_name);
    proc->AddChild(stmt_list);
    stmt_list->AddChild(assign1);
    stmt_list->AddChild(assign2);
    assign1->AddChild(var1);
    assign1->AddChild(expr1);
    expr1->AddChild(var2);
    expr1->AddChild(const1);
    assign2->AddChild(var3);
    assign2->AddChild(expr2);
    expr2->AddChild(expr3);
    expr2->AddChild(const2);
    expr3->AddChild(var4);
    expr3->AddChild(const4);
    proc->SetParent(prog);
    proc_name->SetParent(proc);
    stmt_list->SetParent(proc);
    assign1->SetParent(stmt_list);
    var1->SetParent(assign1);
    expr1->SetParent(assign1);
    var2->SetParent(expr1);
    const1->SetParent(expr1);
    assign2->SetParent(stmt_list);
    var3->SetParent(assign2);
    expr2->SetParent(assign2);
    expr3->SetParent(expr2);
    const2->SetParent(expr3);
    var4->SetParent(expr3);
    const4->SetParent(expr3);
    manager.AddAssignPattern(1, "var1", *expr1);
    manager.AddAssignPattern(2, "var1", *expr2);


    STMT_IDX_SET set1 = manager.GetAssignWithSubPattern("", "");
    REQUIRE(set1.size() == 2);
    REQUIRE(set1.find(1) != set1.end());
    REQUIRE(set1.find(2) != set1.end());

    STMT_IDX_SET set2 = manager.GetAssignWithSubPattern("var1", "");
    REQUIRE(set2.size() == 2);
    REQUIRE(set2.find(1) != set2.end());
    REQUIRE(set2.find(2) != set2.end());

    STMT_IDX_SET set3 = manager.GetAssignWithSubPattern("", "2");
    REQUIRE(set3.size() == 1);
    REQUIRE(set3.find(2) != set3.end());

    STMT_IDX_SET set4 = manager.GetAssignWithSubPattern("var1", "var2");
    REQUIRE(set4.size() == 1);

}

TEST_CASE("Test GetAssignWithFullPattern(VAR_NAME v, EXPRESSION e) with full pattern") {
    PKB* pkb = new PKB();
    PatternManager manager = pkb->GetPatternManager();
    TNode* const_val = new TNode(TNode::NODE_TYPE::constValue, 1.0);
    manager.AddAssignPattern(1, "var1", *const_val);
    STMT_IDX_SET set1 = manager.GetAssignWithFullPattern("", "1");
    REQUIRE(set1.size() == 1);
}


TEST_CASE("Test GetAssignWithSubPattern(VAR_NAME v, EXPRESSION e) with full pattern") {
    PKB* pkb = new PKB();
    PatternManager manager = pkb->GetPatternManager();
    TNode* const_val = new TNode(TNode::NODE_TYPE::constValue, 1.0);
    manager.AddAssignPattern(1, "var1", *const_val);
    STMT_IDX_SET set1 = manager.GetAssignWithSubPattern("", "1");
    REQUIRE(set1.size() == 1);
}

TEST_CASE("Test GetAssignStmtVarPairWithSubPattern(VAR_NAME v, EXPRESSION e)") {
    PKB* pkb = new PKB();
    PatternManager manager = pkb->GetPatternManager();
    TNode* expr1 = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
    TNode* var_x = new TNode(TNode::NODE_TYPE::varName, "var_x");
    TNode* const1 = new TNode(TNode::NODE_TYPE::constValue, 5.0);

    TNode* expr2 = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
    TNode* const2 = new TNode(TNode::NODE_TYPE::constValue, 1.0);
    TNode* expr2_1 = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::times);
    TNode* const2_1 = new TNode(TNode::NODE_TYPE::constValue, 2.0);
    TNode* var_x_2 = new TNode(TNode::NODE_TYPE::varName, "var_x");

    expr1->AddChild(var_x);
    expr1->AddChild(const1);

    expr2->AddChild(expr2_1);
    expr2->AddChild(const2);
    expr2_1->AddChild(var_x_2);
    expr2_1->AddChild(const2_1);

    var_x->SetParent(expr1);
    const1->SetParent(expr1);
    expr2_1->SetParent(expr2);
    const2->SetParent(expr2_1);
    var_x_2->SetParent(expr2_1);
    const2_1->SetParent(expr2_1);
    manager.AddAssignPattern(1, "var1", *expr1);
    manager.AddAssignPattern(2, "var1", *expr2);

    auto list1 =  manager.GetAssignStmtVarPairWithSubPattern("", "var_x");
    REQUIRE(list1.size() == 2);

    auto list2 = manager.GetAssignStmtVarPairWithSubPattern("", "var_y");
    REQUIRE(list2.empty());

    auto list3 = manager.GetAssignStmtVarPairWithSubPattern("", "6");
    REQUIRE(list3.empty());

    auto list4 = manager.GetAssignStmtVarPairWithSubPattern("", "1");
    REQUIRE(list4.size() == 1);
}

TEST_CASE("Test GetAssignStmtVarPairWithFullPattern(VAR_NAME v, EXPRESSION e)") {
    PKB* pkb = new PKB();
    PatternManager manager = pkb->GetPatternManager();
    TNode* expr3 = new TNode(TNode::NODE_TYPE::varName, "var_x");
    manager.AddAssignPattern(2, "var1", *expr3);
    auto list1 = manager.GetAssignStmtVarPairWithFullPattern("", "var_x");
    REQUIRE(list1.size() == 1);
    auto list2 = manager.GetAssignStmtVarPairWithFullPattern("", "uiowe");
    REQUIRE(list2.size() == 0);
}



