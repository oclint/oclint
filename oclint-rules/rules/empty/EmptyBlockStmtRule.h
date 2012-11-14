#ifndef OCLINT_EMPTYBLOCKSTMTRULE_H
#define OCLINT_EMPTYBLOCKSTMTRULE_H

class EmptyBlockStmtRule {
protected:
    bool isLexicalEmpty(Stmt *stmt)
    {
        CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(stmt);
        return isa<NullStmt>(stmt) || (compoundStmt && compoundStmt->body_empty());
    }
};

#endif
