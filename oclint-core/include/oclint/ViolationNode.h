#ifndef OCLINT_VIOLATIONNODE_H
#define OCLINT_VIOLATIONNODE_H

namespace clang
{
    class Decl;
    class Stmt;
}

using namespace clang;

enum NodeType
{
    DECL,
    STMT
};

class ViolationNode
{
public:
    enum NodeType type;
    void *node;
};

ViolationNode* makeViolationNode(Decl *decl);
ViolationNode* makeViolationNode(Stmt *stmt);

#endif
