#ifndef OCLINT_ABSTRACTEMPTYBLOCKSTMTRULE_H
#define OCLINT_ABSTRACTEMPTYBLOCKSTMTRULE_H

template <typename T>
class AbstractEmptyBlockStmtRule : public oclint::AbstractASTVisitorRule<T>
{
protected:
    bool isLexicalEmpty(clang::Stmt *stmt)
    {
        clang::CompoundStmt *compoundStmt = clang::dyn_cast_or_null<clang::CompoundStmt>(stmt);
        return clang::isa<clang::NullStmt>(stmt) || (compoundStmt && compoundStmt->body_empty());
    }

    bool checkLexicalEmptyStmt(clang::Stmt *stmt, oclint::RuleBase *rule)
    {
        if (stmt && isLexicalEmpty(stmt))
        {
            oclint::AbstractASTVisitorRule<T>::addViolation(stmt, rule);
        }

        return true;
    }
};

#endif
