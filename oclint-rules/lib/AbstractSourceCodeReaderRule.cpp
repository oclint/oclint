#include "oclint/AbstractSourceCodeReaderRule.h"

#include <clang/AST/AST.h>
#include <clang/Basic/SourceManager.h>

#include "oclint/helper/SuppressHelper.h"

namespace oclint
{

/*virtual*/
AbstractSourceCodeReaderRule::~AbstractSourceCodeReaderRule() {}

/*virtual*/
void AbstractSourceCodeReaderRule::apply()
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

void AbstractSourceCodeReaderRule::addViolation(int startLine, int startColumn,
    int endLine, int endColumn, RuleBase *rule, const std::string& message)
{
    if (!shouldSuppress(startLine, *_carrier->getASTContext(), rule))
    {
        clang::SourceManager *sourceManager = &_carrier->getSourceManager();

        clang::FileID mainFileID = sourceManager->getMainFileID();
        clang::SourceLocation startOfMainFile = sourceManager->getLocForStartOfFile(mainFileID);
        llvm::StringRef filePath = sourceManager->getFilename(startOfMainFile);

        _carrier->addViolation(filePath.str(),
            startLine, startColumn, endLine, endColumn, rule, message);
    }
}

} // end namespace oclint
