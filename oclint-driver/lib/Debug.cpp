#include "oclint/Debug.h"

#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

void oclint::debug::emit(const char *message)
{
    DEBUG(llvm::dbgs() << message);
}

void oclint::debug::emitLine(const char *message)
{
    DEBUG(llvm::dbgs() << message << "\n");
}
