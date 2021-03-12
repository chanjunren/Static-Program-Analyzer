#include <unordered_map>
#include <unordered_set>

#include "CustomTypes.h"
#include "DesignExtractor.h"

bool DesignExtractor::ExtractFollows(RelationManager manager, TNode root) {
    vector<TNode*> children = root.GetChildrenVector();

    if (root.GetNodeType() == TNode::stmtList) {
        int size = children.size();
        for (int i = 0; i < size - 1; i++) {
            TNode* child = children.at(i);
            TNode* next = children.at(i + 1);
            manager.AddFollows(child->GetStmtIndex(), next->GetStmtIndex());
            for (int j = i + 1; j < size; j++) {
                next = children.at(j);
                manager.AddFollowsStar(child->GetStmtIndex(), next->GetStmtIndex());
            }
        }
    }
    
    if (children.empty()) {
        return true;
    }
    else {
        for (TNode* child : children) {
            ExtractFollows(manager, *child);
        }
    }
    return true;
}

bool DesignExtractor::ExtractParentRecursive(RelationManager manager, TNode root, STMT_IDX_LIST parents) {
    vector<TNode*> children = root.GetChildrenVector();
    if (children.empty()) {
        return true;
    }

    STMT_IDX_LIST new_parents(parents);

    // while
    if (root.GetNodeType() == TNode::whileStmt) {
        // extract parent for all stmtlst
        vector<TNode*> stmtlst = children.at(1)->GetChildrenVector();
        for (TNode* stmt: stmtlst) {
            manager.AddParent(root.GetStmtIndex(), stmt->GetStmtIndex());
            manager.AddParentStar(root.GetStmtIndex(), stmt->GetStmtIndex());
            for (STMT_IDX parent : parents) {
                manager.AddParentStar(parent, stmt->GetStmtIndex());
            }
        }

        new_parents.push_back(root.GetStmtIndex());

    // if
    } else if (root.GetNodeType() == TNode::ifStmt){
        vector<TNode*> iflst = children.at(1)->GetChildrenVector();
        vector<TNode*> elselst = children.at(2)->GetChildrenVector();

        for (TNode* stmt : iflst) {
            manager.AddParent(root.GetStmtIndex(), stmt->GetStmtIndex());
            manager.AddParentStar(root.GetStmtIndex(), stmt->GetStmtIndex());
            for (STMT_IDX parent : parents) {
                manager.AddParentStar(parent, stmt->GetStmtIndex());
            }
        }

        for (TNode* stmt : elselst) {
            manager.AddParent(root.GetStmtIndex(), stmt->GetStmtIndex());
            manager.AddParentStar(root.GetStmtIndex(), stmt->GetStmtIndex());
            for (STMT_IDX parent : parents) {
                manager.AddParentStar(parent, stmt->GetStmtIndex());
            }
        }

        new_parents.push_back(root.GetStmtIndex());
    }

    for (TNode* child : children) {
        ExtractParentRecursive(manager, *child, new_parents);
    }

    return true;
}

bool DesignExtractor::ExtractParent(RelationManager manager, TNode root) {
    STMT_IDX_LIST vec;

    return ExtractParentRecursive(manager, root, vec);
}

VAR_NAME_LIST DesignExtractor::ExtractVarsFromExpr(TNode expr) {
    vector<TNode*> children = expr.GetChildrenVector();
    VAR_NAME_LIST result;

    if (expr.GetNodeType() == TNode::varName) {
        result.push_back(*expr.GetName());
    }

    if(children.empty()) {
        return result;
    }

    for (TNode* child : children) {
        if (child->GetNodeType() == TNode::varName) {
            result.push_back(*child->GetName());
        }
        else {
            VAR_NAME_LIST list = ExtractVarsFromExpr(*child);
            for (VAR_NAME v : list) {
                result.push_back(v);
            }
        }
    }

    return result;
}

// returns the list of procs called by this proc
list<pair <PROC_NAME, STMT_IDX> > DesignExtractor::ExtractUsesRecursive(RelationManager manager, TNode root, STMT_IDX_LIST parents, PROC_NAME proc) {
    vector<TNode*> children = root.GetChildrenVector();
    list<pair <PROC_NAME, STMT_IDX> > lst;
    
    if (children.empty()) {
        return lst;
    }

    vector<STMT_IDX> new_parents(parents);

    if (root.GetNodeType() == TNode::callStmt) {
        lst.push_back(make_pair(*(root.GetChildrenVector().at(0)->GetName()), root.GetStmtIndex()));
    }
    // assignment
    else if (root.GetNodeType() == TNode::assignStmt) {
        // get list of variables used
        TNode* expr = children.at(1);
        VAR_NAME_LIST vars = ExtractVarsFromExpr(*expr);
        for (VAR_NAME var : vars) {
            manager.AddStmtUses(root.GetStmtIndex(), var);
            for (STMT_IDX parent : parents) {
                manager.AddStmtUses(parent, var);
            }
            manager.AddProcUses(proc, var);
        }
    }
    else if (root.GetNodeType() == TNode::printStmt){
        TNode* varName = children.at(0);
        VAR_NAME* var = varName->GetName();
        manager.AddStmtUses(root.GetStmtIndex(), *var);
        for (STMT_IDX parent : parents) {
            manager.AddStmtUses(parent, *var);
        }
        manager.AddProcUses(proc, *var);
    }
    else if (root.GetNodeType() == TNode::whileStmt) {
        TNode* expr = children.at(0);
        VAR_NAME_LIST vars = ExtractVarsFromExpr(*expr);
        for (VAR_NAME var : vars) {
            manager.AddStmtUses(root.GetStmtIndex(), var);
            for (STMT_IDX parent : parents) {
                manager.AddStmtUses(parent, var);
            }
            manager.AddProcUses(proc, var);
        }
        new_parents.push_back(root.GetStmtIndex());
    }
    else if (root.GetNodeType() == TNode::ifStmt) {
        TNode* expr = children.at(0);
        VAR_NAME_LIST vars = ExtractVarsFromExpr(*expr);
        for (VAR_NAME var : vars) {
            manager.AddStmtUses(root.GetStmtIndex(), var);
            for (STMT_IDX parent : parents) {
                manager.AddStmtUses(parent, var);
            }
            manager.AddProcUses(proc, var);
        }
        new_parents.push_back(root.GetStmtIndex());
    }

    for (TNode* child : children) {
        list<pair <PROC_NAME, STMT_IDX> > new_lst = ExtractUsesRecursive(manager, *child, new_parents, proc);
        lst.splice(lst.end(), new_lst);
    }

    return lst;
}

bool DesignExtractor::ExtractUses(RelationManager manager, TNode program) {
    vector<TNode*> procs = program.GetChildrenVector();
    unordered_map < PROC_NAME, list<pair <PROC_NAME, STMT_IDX> > > proc_calls;

    for (TNode* proc: procs) {
        vector<STMT_IDX> vec;
        PROC_NAME* procName = (proc->GetChildrenVector()).at(0)->GetName();
        list<pair <PROC_NAME, STMT_IDX> > calls = ExtractUsesRecursive(manager, *proc, vec, *procName);
        proc_calls.insert(make_pair(*procName, calls));
    }

    for (pair < PROC_NAME, list<pair <PROC_NAME, STMT_IDX> > > call : proc_calls) {
        PROC_NAME caller = call.first;
        list<pair <PROC_NAME, STMT_IDX> > callees = call.second;

        for (pair <PROC_NAME, STMT_IDX> callee : callees) {               // Each proc called
            PROC_NAME proc_name = callee.first;
            STMT_IDX called_line = callee.second;
            PROC_NAME_SET all_indirects = manager.GetCallsStars(callee.first);
            all_indirects.insert(callee.first);

            for (PROC_NAME indirect : all_indirects) {
                // printf("INDIRECT: ", indirect);
                VAR_NAME_SET uses = manager.GetProcUses(indirect);
                STMT_IDX_SET parents = manager.GetInverseParentStars(callee.second);
                for (VAR_NAME use: uses) {
                    manager.AddStmtUses(callee.second, use);
                    manager.AddProcUses(caller, use);
                    for (STMT_IDX s : parents) {
                        manager.AddStmtUses(s, use);
                    }
                }
            }
        }
    }

    return true;
}

// returns the list of procs called by this proc
list<pair <PROC_NAME, STMT_IDX> > ExtractModifiesRecursive(RelationManager manager, TNode root, vector<STMT_IDX> parents, PROC_NAME proc) {
    vector<TNode*> children = root.GetChildrenVector();
    list<pair <PROC_NAME, STMT_IDX> > lst;

    if (children.empty()) {
        return lst;
    }

    vector<STMT_IDX> new_parents(parents);

    if (root.GetNodeType() == TNode::callStmt) {
        lst.push_back(make_pair(*(root.GetChildrenVector().at(0)->GetName()), root.GetStmtIndex()));
    }
    // assignment: if left side
    else if (root.GetNodeType() == TNode::assignStmt) {
        // get list of variables used
        TNode* varName = children.at(0);
        VAR_NAME* var = varName->GetName();
        manager.AddStmtModifies(root.GetStmtIndex(), *var);
        for (STMT_IDX parent : parents) {
            manager.AddStmtModifies(parent, *var);
        }
        // cout << proc << " mods " << *var << "\n";
        manager.AddProcModifies(proc, *var);
    }
    else if (root.GetNodeType() == TNode::readStmt) {
        TNode* varName = children.at(0);
        VAR_NAME* var = varName->GetName();
        manager.AddStmtModifies(root.GetStmtIndex(), *var);
        for (STMT_IDX parent : parents) {
            manager.AddStmtModifies(parent, *var);
        }
        // cout << proc << " mods " << *var << "\n";
        manager.AddProcModifies(proc, *var);
    }
    else if (root.GetNodeType() == TNode::whileStmt) {
        new_parents.push_back(root.GetStmtIndex());
    }
    else if (root.GetNodeType() == TNode::ifStmt) {
        new_parents.push_back(root.GetStmtIndex());
    }

    for (TNode* child : children) {
        list<pair <PROC_NAME, STMT_IDX> > new_lst = ExtractModifiesRecursive(manager, *child, new_parents, proc);
        lst.splice(lst.end(), new_lst);
    }

    return lst;
}

bool DesignExtractor::ExtractModifies(RelationManager manager, TNode program) {
    vector<TNode*> procs = program.GetChildrenVector();
    unordered_map < PROC_NAME, list<pair <PROC_NAME, STMT_IDX> > > proc_calls;

    for (TNode* proc: procs) {
        vector<STMT_IDX> vec;
        PROC_NAME* procName = (proc->GetChildrenVector()).at(0)->GetName();
        list<pair <PROC_NAME, STMT_IDX> > calls = ExtractModifiesRecursive(manager, *proc, vec, *procName);
        proc_calls.insert(make_pair(*procName, calls));
    }

    for (pair < PROC_NAME, list<pair <PROC_NAME, STMT_IDX> > > call : proc_calls) {
        PROC_NAME caller = call.first;
        list<pair <PROC_NAME, STMT_IDX> > callees = call.second;

        for (pair <PROC_NAME, STMT_IDX> callee : callees) {               // Each proc called
            PROC_NAME proc_name = callee.first;
            STMT_IDX called_line = callee.second;
            PROC_NAME_SET all_indirects = manager.GetCallsStars(callee.first);
            all_indirects.insert(callee.first);

            for (PROC_NAME indirect : all_indirects) {
                // printf("INDIRECT: ", indirect);
                VAR_NAME_SET modifies = manager.GetProcModifies(indirect);
                STMT_IDX_SET parents = manager.GetInverseParentStars(callee.second);
                for (VAR_NAME mod : modifies) {
                    manager.AddStmtModifies(callee.second, mod);
                    manager.AddProcModifies(caller, mod);
                    for (STMT_IDX s : parents) {
                        manager.AddStmtModifies(s, mod);
                    }
                }
            }
        }
    }

    return true;
}

// returns the list of procs called by this proc
list<PROC_NAME> ExtractCallsRecursive(TNode root) {
    vector<TNode*> children = root.GetChildrenVector();
    list<PROC_NAME> lst;

    if (children.empty()) {
        return lst;
    }

    if (root.GetNodeType() == TNode::callStmt) {
        lst.push_back(*(root.GetChildrenVector().at(0)->GetName()));
    }

    for (TNode* child : children) {
        list<PROC_NAME> new_lst = ExtractCallsRecursive(*child);
        lst.splice(lst.end(), new_lst);
    }

    return lst;
}

bool DesignExtractor::ExtractCalls(RelationManager manager, TNode program) {
    vector<TNode*> procs = program.GetChildrenVector();
    unordered_map < PROC_NAME, list<PROC_NAME> > proc_calls;                                     

    for (TNode* proc : procs) {
        vector<STMT_IDX> vec;
        PROC_NAME* procName = (proc->GetChildrenVector()).at(0)->GetName();
        list<PROC_NAME> calls = ExtractCallsRecursive(*proc);
        proc_calls.insert(make_pair(*procName, calls));
    }

    // Do BFS for every proc call to extract call stars
    for (pair < PROC_NAME, list<PROC_NAME> > call : proc_calls) {
        PROC_NAME caller = call.first;
        list<PROC_NAME> callees = call.second;
        queue<PROC_NAME> frontier;
        unordered_set<PROC_NAME> explored;

        for (PROC_NAME callee : callees) {
            manager.AddCalls(caller, callee);
            manager.AddCallsStar(caller, callee);
            frontier.push(callee); 
        }

        explored.insert(caller);
        while (!frontier.empty()) {
            PROC_NAME to_explore = frontier.front();

            if (explored.count(to_explore) == 1) { // Already explored this node
                frontier.pop();
                continue;
            }

            frontier.pop();
            explored.insert(to_explore);

            auto iterator = proc_calls.find(to_explore);
            if (iterator == proc_calls.end()) { // NOTHING to explore for this node
                continue;
            }

            callees = iterator->second;
            for (PROC_NAME callee : callees) {
                manager.AddCallsStar(caller, callee);
                if (explored.count(callee) == 0) {
                    frontier.push(callee);
                }
            }
            
        }
    }

    return true;
}


bool DesignExtractor::ExtractData(DataManager manager, TNode root) {
    vector<TNode*> children = root.GetChildrenVector();
    switch (root.GetNodeType()) {
    case TNode::procedure:
        manager.AddProcedure(*(root.GetChildrenVector().at(0)->GetName()));
        break;
    case TNode::assignStmt:
        manager.AddStatement(assignStatement, root.GetStmtIndex());
        break;
    case TNode::readStmt:
        manager.AddStatement(readStatement, root.GetStmtIndex());
        break;
    case TNode::callStmt:
        manager.AddStatement(callStatement, root.GetStmtIndex());
        manager.AddCall(*(root.GetChildrenVector().at(0)->GetName()), root.GetStmtIndex());
        break;
    case TNode::ifStmt:
        manager.AddStatement(ifStatement, root.GetStmtIndex());
        break;
    case TNode::whileStmt:
        manager.AddStatement(whileStatement, root.GetStmtIndex());
        break;
    case TNode::printStmt:
        manager.AddStatement(printStatement, root.GetStmtIndex());
        break;
    case TNode::varName:
        manager.AddVariable(*(root.GetName()));
        break;
    case TNode::constValue:
        manager.AddConstant(root.GetConstValue());
        break;
    default:
        break;
    }
    for (TNode* child : children) {
        ExtractData(manager, *child);
    }
    return true;
}

bool DesignExtractor::ExtractAssignPattern(PatternManager manager, TNode root) {
    vector<TNode*> children = root.GetChildrenVector();
    if (root.GetNodeType() == TNode::assignStmt) {
        TNode* var_node = children.at(0);
        TNode* expr_node = children.at(1);
        manager.AddAssignPattern(root.GetStmtIndex(), *var_node->GetName(), *expr_node);
    } else {
        for (TNode* child : children) {
            ExtractAssignPattern(manager, *child);
        }
    }
    return true;
}
bool DesignExtractor::ExtractContainerPattern(PatternManager manager, TNode root) {
    vector<TNode*> children = root.GetChildrenVector();
    if (root.GetNodeType() == TNode::ifStmt) {
        TNode* condition_node = children.at(0);
        TNode* then_node = children.at(1);
        TNode* else_node = children.at(2);
        manager.AddIfPattern(root.GetStmtIndex(), *condition_node);
        ExtractContainerPattern(manager, *then_node);
        ExtractContainerPattern(manager, *else_node);
    } else if (root.GetNodeType() == TNode::whileStmt) {
        TNode* condition_node = children.at(0);
        TNode* stmt_list_node = children.at(1);
        manager.AddWhilePattern(root.GetStmtIndex(), *condition_node);
        ExtractContainerPattern(manager, *stmt_list_node);
    } else {
        for (auto child: children) {
            ExtractContainerPattern(manager, *child);
        }
    }
    return true;
}
bool DesignExtractor::ExtractAssignStmtInProcs(DataManager manager, TNode root) {
    if (root.GetNodeType() == TNode::NODE_TYPE::procedure) {
        vector<TNode*> children = root.GetChildrenVector();
        auto procName = children.at(0)->GetName();
        //Procedure nodes only have 2 children, first one is procName and the second one is stmtList
        TNode* stmtListNode = children.at(1);
        vector<TNode*> stmts = stmtListNode->GetChildrenVector();
        for (auto stmt : stmts) {
            if (stmt->GetNodeType() == TNode::NODE_TYPE::assignStmt) {
                manager.AddAssignStatement(*procName, stmt->GetStmtIndex());
            }
        }
    } else if (root.GetNodeType() == TNode::NODE_TYPE::program){
        vector<TNode*> children = root.GetChildrenVector();
        for (auto child : children) {
            ExtractAssignStmtInProcs(manager, *child);
        }
    }
    return true;
}

 