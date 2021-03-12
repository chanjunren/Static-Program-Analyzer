//
// Created by Xu Lin on 21/10/20.
//

#include "catch.hpp"
#include <PatternManager.h>
TEST_CASE("Unit test for AddIfPattern(STMT_IDX s, TNode root)") {
    PatternManager pm = PatternManager();
    /*
     * program looks like the following:
     * 1 if (x != 1 && y + z < 6) then {
     * 2    call Jail;
     * 3    x = x + 1;
     *   } else {
     * 4    x = x + 2;
     * 5    z = y + 3
     *   }
     * }
     */
    TNode* ifNode = new TNode(TNode::NODE_TYPE::ifStmt);
    //Conditional expression: x != 1 && y + z < 6
    TNode* condNode = new TNode(TNode::NODE_TYPE::condExpr, TNode::OPERATOR::andOp);
    //Left rel_expr of condNode, x != 1
    TNode* leftRelExprNode = new TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::unequal);
    leftRelExprNode->AddChild(new TNode(TNode::NODE_TYPE::varName, "x"));
    leftRelExprNode->AddChild(new TNode(TNode::NODE_TYPE::constValue, 1));
    //Right rel_expr of condNode, y + z < 6
    TNode* rightRelExprNode = new TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::lesser);
    TNode* rightRelRightExprNode = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
    rightRelRightExprNode->AddChild(new TNode(TNode::NODE_TYPE::varName, "y"));
    rightRelRightExprNode->AddChild(new TNode(TNode::NODE_TYPE::varName, "z"));
    rightRelExprNode->AddChild(rightRelRightExprNode);
    rightRelExprNode->AddChild(new TNode(TNode::NODE_TYPE::constValue, 6));
    condNode->AddChild(leftRelExprNode);
    condNode->AddChild(rightRelExprNode);
    TNode* thenNode = new TNode(TNode::NODE_TYPE::stmtList);
    //line2: call Jail;
    TNode* line2 = new TNode(TNode::NODE_TYPE::callStmt);
    line2->AddChild(new TNode(TNode::NODE_TYPE::procName, "Jail"));
    //line3: x = x + 1;
    TNode* line3 = new TNode(TNode::NODE_TYPE::assignStmt);
    line3->AddChild(new TNode(TNode::NODE_TYPE::varName, "x"));
    TNode* line3ExprNode = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
    line3ExprNode->AddChild(new TNode(TNode::NODE_TYPE::varName, "x"));
    line3ExprNode->AddChild(new TNode(TNode::NODE_TYPE::constValue, 1));
    thenNode->AddChild(line2);
    thenNode->AddChild(line3);

    TNode* elseNode = new TNode(TNode::NODE_TYPE::stmtList);
    //line4: x = x + 2
    TNode* line4 = new TNode(TNode::NODE_TYPE::assignStmt);
    line4->AddChild(new TNode(TNode::NODE_TYPE::varName, "x"));
    TNode* line4ExprNode = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
    line4ExprNode->AddChild(new TNode(TNode::NODE_TYPE::varName, "x"));
    line4ExprNode->AddChild(new TNode(TNode::NODE_TYPE::constValue, 2));
    //line5: z = y + 3
    TNode* line5 = new TNode(TNode::NODE_TYPE::assignStmt);
    line5->AddChild(new TNode(TNode::NODE_TYPE::varName, "z"));
    TNode* line5ExprNode = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
    line5ExprNode->AddChild(new TNode(TNode::NODE_TYPE::varName, "y"));
    line5ExprNode->AddChild(new TNode(TNode::NODE_TYPE::constValue, 3));
    elseNode->AddChild(line4);
    elseNode->AddChild(line5);
    ifNode->AddChild(condNode);
    ifNode->AddChild(thenNode);
    ifNode->AddChild(elseNode);

    TNode condNode2 = TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::equal);
    condNode2.AddChild(new TNode(TNode::NODE_TYPE::varName, "z"));
    condNode2.AddChild(new TNode(TNode::NODE_TYPE::varName, "y"));
    pm.AddIfPattern(1, *condNode);
    pm.AddIfPattern(9, condNode2);
    auto resultWithX = pm.GetIfWithPattern("x");
    REQUIRE(resultWithX.size() == 1);
    REQUIRE(resultWithX.find(1) != resultWithX.end());
    auto resultWithY = pm.GetIfWithPattern("y");
    REQUIRE(resultWithY.size() == 2);
    REQUIRE(resultWithY.find(9) != resultWithY.end());
}

TEST_CASE("Unit test for AddWhilePattern(STMT_IDX s, TNode root)") {
    //Conditional expression: x != 1 && y + z < 6
    TNode condNode = TNode(TNode::NODE_TYPE::condExpr, TNode::OPERATOR::andOp);
    //Left rel_expr of condNode, x != 1
    TNode* leftRelExprNode = new TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::unequal);
    leftRelExprNode->AddChild(new TNode(TNode::NODE_TYPE::varName, "x"));
    leftRelExprNode->AddChild(new TNode(TNode::NODE_TYPE::constValue, 1));
    //Right rel_expr of condNode, y + z < 6
    TNode* rightRelExprNode = new TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::lesser);
    TNode* rightRelRightExprNode = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
    rightRelRightExprNode->AddChild(new TNode(TNode::NODE_TYPE::varName, "y"));
    rightRelRightExprNode->AddChild(new TNode(TNode::NODE_TYPE::varName, "z"));
    rightRelExprNode->AddChild(rightRelRightExprNode);
    rightRelExprNode->AddChild(new TNode(TNode::NODE_TYPE::constValue, 6));
    condNode.AddChild(leftRelExprNode);
    condNode.AddChild(rightRelExprNode);
    //z == y
    TNode condNode2 = TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::equal);
    condNode2.AddChild(new TNode(TNode::NODE_TYPE::varName, "z"));
    condNode2.AddChild(new TNode(TNode::NODE_TYPE::varName, "y"));
    PatternManager pm = PatternManager();
    pm.AddWhilePattern(2, condNode);
    pm.AddWhilePattern(9, condNode2);
    auto resultWithX = pm.GetWhileWithPattern("x");
    REQUIRE(resultWithX.size() == 1);
    auto resultWithY = pm.GetWhileWithPattern("y");
    REQUIRE(resultWithY.size() == 2);
    REQUIRE(resultWithY.find(2) != resultWithY.end());
    REQUIRE(resultWithY.find(9) != resultWithY.end());
    auto pairResultWithX = pm.GetWhileStmtVarPairWithPattern("x");
    //debug
    for (auto pair: pairResultWithX) {
        std::cout << "" << pair.s << ", " << pair.v << std::endl;
    }
    auto pairResultWithY = pm.GetWhileStmtVarPairWithPattern("y");
    //debug
    for (auto pair: pairResultWithY) {
        std::cout << "" << pair.s << ", " << pair.v << std::endl;
    }
}

