#ifndef OCLINT_ABSTRACTEMPTYBLOCKSTMTRULE_H
#define OCLINT_ABSTRACTEMPTYBLOCKSTMTRULE_H

template <typename T>
class AbstractEmptyBlockStmtRule : public AbstractASTVisitorRule<T>
{
protected:
    bool isLexicalEmpty(Stmt *stmt)
    {
        CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(stmt);
        return isa<NullStmt>(stmt) || (compoundStmt && compoundStmt->body_empty());
    }

    bool checkLexicalEmptyStmt(Stmt *stmt, RuleBase *rule)
    {
        if (stmt && isLexicalEmpty(stmt))
        {
            AbstractASTVisitorRule<T>::addViolation(stmt, rule);
        }

        return true;
    }
};

#endif
