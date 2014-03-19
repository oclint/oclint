#ifndef OCLINT_LOGGER_H
#define OCLINT_LOGGER_H

#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

namespace oclint
{
namespace logger
{
    llvm::raw_ostream &verboseStream(bool debugFlag = false);
} // end namespace logger
} // end namespace oclint

#ifndef NDEBUG

#define LOG_DEBUG(MSG) DEBUG(llvm::dbgs() << MSG)
#define LOG_DEBUG_LINE(MSG) LOG_DEBUG(MSG << "\n")

#define LOG_VERBOSE(MSG) oclint::logger::verboseStream(llvm::DebugFlag) << MSG

#else

#define LOG_DEBUG(MSG)
#define LOG_DEBUG_LINE(MSG)

#define LOG_VERBOSE(MSG) oclint::logger::verboseStream() << MSG

#endif

#define LOG_VERBOSE_LINE(MSG) LOG_VERBOSE(MSG << "\n")

#endif
