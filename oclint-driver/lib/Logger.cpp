#include "oclint/Logger.h"

#include "oclint/Options.h"

llvm::raw_ostream &oclint::logger::verboseStream(bool debugFlag)
{
    if (debugFlag || oclint::option::enableVerbose())
    {
        return llvm::outs();
    }
    return llvm::nulls();
}
