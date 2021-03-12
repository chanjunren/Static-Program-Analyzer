#ifndef DATA_TABLES_H
#define DATA_TABLES_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <CustomTypes.h>

class CallsTable {
private:
    std::unordered_map<STMT_IDX, PROC_NAME> data_;
    std::unordered_map<PROC_NAME, STMT_IDX_SET> inverse_data_;
public:
    bool Add(PROC_NAME pname, STMT_IDX s);
    PROC_NAME GetCalledBy(STMT_IDX s);
    STMT_IDX_SET GetInverseCalledBy(PROC_NAME p);
};

class ProcedureTable {
private:
    PROC_NAME_SET data_;
public:
    ProcedureTable() {
        data_ =  PROC_NAME_SET();
    }
    bool Add(PROC_NAME p);
    PROC_NAME_SET GetAll();
};

class StatementTable {
#define _ALL_ 0
#define _ASSIGN_ 1
#define _READ_ 2
#define _PRINT_ 3
#define _CALL_ 4
#define _WHILE_ 5
#define _IF_ 6
#define _size_ 7
private:
    std::vector<STMT_IDX_SET> stmt_data_;
    std::unordered_map<PROC_NAME, STMT_IDX_SET> proc_to_assign_stmt_data_;
    int ConvertStmtType(STATEMENT_TYPE type);
public:
    StatementTable() {
        for (int i = 0; i < _size_; i++) {
            stmt_data_.push_back(STMT_IDX_SET());
        }
    }
    bool Add(STATEMENT_TYPE t, STMT_IDX s);
    STMT_IDX_SET GetAll(STATEMENT_TYPE t);
    STMT_IDX_SET GetAll();
    bool Add(PROC_NAME p, STMT_IDX s);
    STMT_IDX_SET GetAll(PROC_NAME p);
    bool IsAssignStmt(STMT_IDX s);
    bool IsCallStmt(STMT_IDX s);
    bool IsReadStmt(STMT_IDX s);

    bool IsStmt(STATEMENT_TYPE t, STMT_IDX s);

};

class VariableTable {
private:
    VAR_NAME_SET data_;
public:
    VariableTable() {
        data_ = VAR_NAME_SET();
    }
    bool Add(VAR_NAME v);
    VAR_NAME_SET GetAll();
};

class ConstTable {
private:
    INT_SET data_;
public:
    ConstTable() {
        data_ = INT_SET();
    }
    bool Add(CONST_VALUE cv);
    INT_SET GetAll();
};

#endif // DATA_TABLES_H