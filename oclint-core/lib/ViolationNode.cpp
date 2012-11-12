#include <clang/AST/AST.h>

#include "oclint/ViolationNode.h"

ViolationNode* makeViolationNode(Decl *decl)
{
    ViolationNode *node = new ViolationNode();
    node->type = DECL;
    node->node = decl;
    return node;
}

ViolationNode* makeViolationNode(Stmt *stmt)
{
    ViolationNode *node = new ViolationNode();
    node->type = STMT;
    node->node = stmt;
    return node;
}
