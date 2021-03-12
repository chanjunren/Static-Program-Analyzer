//
// Created by Xu Lin on 22/9/20.
//

#include "TestUtils.h"

TNode* testutils::ConstructProgTreeWithSingleAssignStmt() {
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
    return prog;
}
TNode *testutils::ConstructProgTreeWithTwoAssignStmt() {
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
    return prog;
}

