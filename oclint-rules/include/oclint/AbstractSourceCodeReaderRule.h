#ifndef OCLINT_ABSTRACTSOURCECODEREADERRULE_H
#define OCLINT_ABSTRACTSOURCECODEREADERRULE_H

#include "oclint/RuleBase.h"

class AbstractSourceCodeReaderRule : public RuleBase
{
protected:
    virtual void apply()
    {
        SourceManager *sourceManager = &_carrier->astContext()->getSourceManager();
        FileID mainFileID = sourceManager->getMainFileID();

        SourceLocation startOfMainFile = sourceManager->getLocForStartOfFile(mainFileID);
        StringRef filePath = sourceManager->getFilename(startOfMainFile);

        StringRef mainFileStringRef = sourceManager->getBufferData(mainFileID);
        StringRef remaining = mainFileStringRef;
        int currentLineNumber = 1;
        while (remaining.size() > 0)
        {
            pair<StringRef, StringRef> splitPair = remaining.split('\n');
            StringRef currentLine = splitPair.first;
            eachLine(currentLineNumber++, currentLine.str(), filePath.str());
            remaining = splitPair.second;
        }
    }

    void addViolation(int startLine, int startColumn,
        int endLine, int endColumn, RuleBase *rule, const string& message = "")
    {
        SourceManager *sourceManager = &_carrier->astContext()->getSourceManager();
        FileID mainFileID = sourceManager->getMainFileID();

        SourceLocation startOfMainFile = sourceManager->getLocForStartOfFile(mainFileID);
        StringRef filePath = sourceManager->getFilename(startOfMainFile); // TODO: Duplicated code

        _carrier->addViolation(filePath.str(),
            startLine, startColumn, endLine, endColumn, rule, message);
    }

public:
    virtual ~AbstractSourceCodeReaderRule() {}

    virtual void eachLine(int lineNumber, string line, string filePath) = 0;
};

#endif
