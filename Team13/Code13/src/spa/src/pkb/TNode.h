#ifndef _TNODE_H_
#define _TNODE_H_

#include <string>
#include <vector>
#include <list>
#include <CustomTypes.h>
#include "BTNode.h"

using namespace std;

class TNode {
public:
    enum NODE_TYPE {
        program,
        procedure,
        stmtList,
        readStmt,
        printStmt,
        callStmt,
        whileStmt,
        ifStmt,
        assignStmt,
        condExpr,
        relExpr,
        relFactor,
        expr,
        term,
        factor,
        varName,
        procName,
        constValue
    };

    enum OPERATOR {
        notOp,
        andOp,
        orOp,
        greater,
        geq,
        lesser,
        leq,
        equal,
        unequal,
        plus,
        minus,
        times,
        divide,
        mod
    };

    vector<string> operators_ = {
        "Not",
        "And",
        "Or",
        "Gt",
        "Gte",
        "Lt",
        "Lte",
        "Eq",
        "Neq",
        "Plus",
        "Minus",
        "Times",
        "Div",
        "Mod"
    };

private:
    NODE_TYPE type;
    TNode* parent;
    vector<TNode*> children;
    CONST_VALUE value;
    OPERATOR op;
    STMT_IDX index;
    string* name;

public:
    TNode(void);
	TNode(NODE_TYPE _type);
	TNode(NODE_TYPE _type, string _name); //var name or proc name node
	TNode(NODE_TYPE _type, CONST_VALUE _value); //const value node
	TNode(NODE_TYPE _type, OPERATOR _op); //operator node
	TNode(NODE_TYPE _type, STMT_IDX _idx); //statement node
	bool InsertChildAtHead(TNode* child);
	bool AddChild(TNode* child);
	bool SetParent(TNode* newParent);
	bool SetName(VAR_NAME *newName);
	bool SetValue(CONST_VALUE newValue);
	bool SetOp(OPERATOR newOp);
    NODE_TYPE GetNodeType();
    list<TNode*> GetChildrenList();
    vector<TNode*> GetChildrenVector();
    STMT_IDX GetStmtIndex();
    string GetSelectValue();
    OPERATOR GetOperator();
    string getData();
    string GetGUIData();
    TNode* GetParent();
    CONST_VALUE GetConstValue();
    string* GetName();
    string GetShortName();
    void Print(TNode* root);
};

#endif