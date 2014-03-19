#include "oclint/Logger.h"

#include <llvm/Support/Debug.h>

#include "oclint/Options.h"

#ifndef NDEBUG

llvm::raw_ostream &oclint::logger::debugStream()
{
    if (llvm::DebugFlag)
    {
        return llvm::dbgs();
    }
    return llvm::nulls();
}

#endif

llvm::raw_ostream &oclint::logger::verboseStream()
{
#ifndef NDEBUG
    if (llvm::DebugFlag || oclint::option::enableVerbose())
#else
    if (oclint::option::enableVerbose())
#endif
    {
        return llvm::outs();
    }
    return llvm::nulls();
}
