#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <iostream>
#include <cstdlib>
#include <unordered_set>
#include <CustomTypes.h>
#include "DataTables.h"

class DataManager {
protected:
    static ConstTable const_table_;
    static StatementTable stmt_table_;
    static VariableTable var_table_;
    static ProcedureTable proc_table_;
    static CallsTable calls_table_;

public:
    DataManager() {
    }
    ConstTable GetConstTable();
    StatementTable GetStmtTable();
    VariableTable GetVarTable();
    ProcedureTable GetProcTable();

    bool AddStatement(STATEMENT_TYPE t, STMT_IDX s);
    bool AddAssignStatement(PROC_NAME p, STMT_IDX s);
    bool AddVariable(VAR_NAME v);
    bool AddProcedure(PROC_NAME p);
    bool AddConstant(CONST_VALUE c);
    bool AddCall(PROC_NAME p, STMT_IDX s);

    STMT_IDX_SET GetAllStatements(STATEMENT_TYPE t);
    STMT_IDX_SET GetAllStatements();
    STMT_IDX_SET GetAllAssignStatements(PROC_NAME p);
    VAR_NAME_SET GetAllVariables();
    PROC_NAME_SET GetAllProcedures();
    INT_SET GetAllConstants();
    PROC_NAME GetCalledByStmt(STMT_IDX s);
    STMT_IDX_SET GetInverseCalledBy(PROC_NAME p);

    bool IsAssignStmt(STMT_IDX s);
    bool IsCallStmt(STMT_IDX s);
    bool IsReadStmt(STMT_IDX s);

    bool IsStmt(STATEMENT_TYPE type, STMT_IDX s);
};

#endif // DATA_MANAGER_H