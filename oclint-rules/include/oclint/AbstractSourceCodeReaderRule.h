#ifndef OCLINT_ABSTRACTSOURCECODEREADERRULE_H
#define OCLINT_ABSTRACTSOURCECODEREADERRULE_H

#include "oclint/RuleBase.h"

class AbstractSourceCodeReaderRule : public RuleBase
{
protected:
    virtual void apply()
    {
        SourceManager *sourceManager = &_astContext->getSourceManager();
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

public:
    virtual ~AbstractSourceCodeReaderRule() {}

    virtual void eachLine(int lineNumber, string line, string filePath) = 0;
};

#endif
