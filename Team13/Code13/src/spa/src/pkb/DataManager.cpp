//
// Created by Xu Lin on 12/9/20.
//

#include "DataManager.h"
ConstTable DataManager::const_table_ = ConstTable();
StatementTable DataManager::stmt_table_ = StatementTable();
VariableTable DataManager::var_table_ = VariableTable();
ProcedureTable DataManager::proc_table_ = ProcedureTable();
CallsTable DataManager::calls_table_ = CallsTable();

bool DataManager::AddCall(PROC_NAME p, STMT_IDX s) {
    return calls_table_.Add(p, s);
}

PROC_NAME DataManager::GetCalledByStmt(STMT_IDX s) {
    return calls_table_.GetCalledBy(s);
}

STMT_IDX_SET DataManager::GetInverseCalledBy(PROC_NAME p)
{
    return calls_table_.GetInverseCalledBy(p);
}

bool DataManager::AddStatement(STATEMENT_TYPE t, STMT_IDX s) {
    return stmt_table_.Add(t, s);
}
STMT_IDX_SET DataManager::GetAllStatements(STATEMENT_TYPE t) {
    return stmt_table_.GetAll(t);
}
STMT_IDX_SET DataManager::GetAllStatements() {
    return stmt_table_.GetAll();
}
bool DataManager::AddVariable(VAR_NAME v) {
    return var_table_.Add(v);
}
VAR_NAME_SET DataManager::GetAllVariables() {
    return var_table_.GetAll();
}
bool DataManager::AddProcedure(PROC_NAME p) {
    return proc_table_.Add(p);
}
PROC_NAME_SET DataManager::GetAllProcedures() {
    return proc_table_.GetAll();
}
bool DataManager::AddConstant(CONST_VALUE c) {
    return const_table_.Add(c);
}
INT_SET DataManager::GetAllConstants() {
    return const_table_.GetAll();
}
ConstTable DataManager::GetConstTable() {
    return const_table_;
}
StatementTable DataManager::GetStmtTable() {
    return stmt_table_;
}
VariableTable DataManager::GetVarTable() {
    return var_table_;
}
ProcedureTable DataManager::GetProcTable() {
    return proc_table_;
}
bool DataManager::AddAssignStatement(PROC_NAME p, STMT_IDX s) {
    return stmt_table_.Add(p, s);
}
STMT_IDX_SET DataManager::GetAllAssignStatements(PROC_NAME p) {
    return stmt_table_.GetAll(p);
}

bool DataManager::IsCallStmt(STMT_IDX s) {
    return stmt_table_.IsCallStmt(s);
}
bool DataManager::IsAssignStmt(STMT_IDX s) {
    return stmt_table_.IsAssignStmt(s);
}
bool DataManager::IsReadStmt(STMT_IDX s) {
    return stmt_table_.IsReadStmt(s);
}
bool DataManager::IsStmt(STATEMENT_TYPE t, STMT_IDX s) {
    return stmt_table_.IsStmt(t, s);
}