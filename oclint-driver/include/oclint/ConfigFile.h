#ifndef OCLINT_CONFIGFILE_H
#define OCLINT_CONFIGFILE_H

#include "llvm/Support/MemoryBuffer.h"

#import <vector>

namespace oclint
{
namespace option
{

struct RuleConfigurationPair
{
    llvm::StringRef key;
    llvm::StringRef value;
};

struct ConfigFile
{
    std::string path;
    llvm::OwningPtr<llvm::MemoryBuffer> buffer;
    std::vector<llvm::StringRef> rules;
    std::vector<llvm::StringRef> disableRules;
    std::vector<llvm::StringRef> rulePaths;
    std::vector<RuleConfigurationPair> ruleConfigurations;
};

/** Read all config files in order of ascending priority. */
std::vector<ConfigFile> readConfigFiles();

} // end namespace option
} // end namespace oclint

#endif
