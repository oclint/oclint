#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

#include "oclint/Debug.h"

void oclint::debug::emit(const char *message)
{
    DEBUG(llvm::dbgs() << message);
}
