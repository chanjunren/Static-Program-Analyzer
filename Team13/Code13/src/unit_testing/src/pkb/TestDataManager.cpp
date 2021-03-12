//
// Created by Xu Lin on 22/9/20.
//
#include <PKB.h>
#include "catch.hpp"

TEST_CASE("Test StatementTable GetAll") {
    PKB* pkb = new PKB();
    DataManager dm = pkb->GetDataManager();
    dm.AddStatement(STATEMENT_TYPE::assignStatement, 1);
    dm.AddStatement(STATEMENT_TYPE::assignStatement, 2);
    dm.AddStatement(STATEMENT_TYPE::assignStatement, 3);
    dm.AddStatement(STATEMENT_TYPE::callStatement, 4);
    STMT_IDX_SET set = dm.GetAllStatements(STATEMENT_TYPE::assignStatement);
    for (auto s: set) {
        std::cout << "" << s << std::endl;
    }
}