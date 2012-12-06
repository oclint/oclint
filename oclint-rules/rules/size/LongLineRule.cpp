#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

class LongLineRule : public RuleBase
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "long line";
    }

    virtual const int priority() const
    {
        return 3;
    }

    virtual void apply()
    {
        int threshold = ruleConfiguration("LONG_LINE", 100);
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
            int currentLineSize = currentLine.size();
            if (currentLineSize > threshold)
            {
                string description = "Line with " + intToString(currentLineSize) +
                    " characters exceeds limit of " + intToString(threshold);
                addViolation(filePath.str(), currentLineNumber, 1,
                    currentLineNumber, currentLineSize, this, description);
            }
            remaining = splitPair.second;
            currentLineNumber++;
        }
    }
};

RuleSet LongLineRule::rules(new LongLineRule());
