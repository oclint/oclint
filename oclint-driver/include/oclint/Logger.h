#ifndef OCLINT_LOGGER_H
#define OCLINT_LOGGER_H

#include "llvm/Support/Debug.h"

#ifndef NDEBUG

#define LOG_DEBUG(MSG) DEBUG(llvm::dbgs() << MSG)
#define LOG_DEBUG_LINE(MSG) LOG_DEBUG(MSG << "\n")

#else

#define LOG_DEBUG(MSG)
#define LOG_DEBUG_LINE(MSG)

#endif

#endif
