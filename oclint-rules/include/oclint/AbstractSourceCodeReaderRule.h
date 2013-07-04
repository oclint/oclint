#ifndef OCLINT_ABSTRACTSOURCECODEREADERRULE_H
#define OCLINT_ABSTRACTSOURCECODEREADERRULE_H

#include <clang/AST/AST.h>

#include "oclint/RuleBase.h"
#include "oclint/helper/SuppressHelper.h"

namespace oclint
{

class AbstractSourceCodeReaderRule : public RuleBase
{
protected:
    virtual void apply()
    {
        clang::SourceManager *sourceManager = &_carrier->getSourceManager();

        clang::FileID mainFileID = sourceManager->getMainFileID();
        llvm::StringRef mainFileStringRef = sourceManager->getBufferData(mainFileID);

        llvm::StringRef remaining = mainFileStringRef;
        int currentLineNumber = 1;
        while (remaining.size() > 0)
        {
            std::pair<llvm::StringRef, llvm::StringRef> splitPair = remaining.split('\n');
            llvm::StringRef currentLine = splitPair.first;
            eachLine(currentLineNumber++, currentLine.str());
            remaining = splitPair.second;
        }
    }

    void addViolation(int startLine, int startColumn,
        int endLine, int endColumn, RuleBase *rule, const std::string& message = "")
    {
        if (!shouldSuppress(startLine, *_carrier->getASTContext()))
        {
            clang::SourceManager *sourceManager = &_carrier->getSourceManager();

            clang::FileID mainFileID = sourceManager->getMainFileID();
            clang::SourceLocation startOfMainFile = sourceManager->getLocForStartOfFile(mainFileID);
            llvm::StringRef filePath = sourceManager->getFilename(startOfMainFile);

            _carrier->addViolation(filePath.str(),
                startLine, startColumn, endLine, endColumn, rule, message);
        }
    }

public:
    virtual ~AbstractSourceCodeReaderRule() {}

    virtual void eachLine(int lineNumber, std::string line) = 0;
};

} // end namespace oclint

#endif
