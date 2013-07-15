#ifndef OCLINT_CONFIGFILE_H
#define OCLINT_CONFIGFILE_H

#include "llvm/Support/YAMLTraits.h"
#include "llvm/Support/MemoryBuffer.h"

#import <vector>

namespace oclint
{
namespace option
{

class RuleConfigurationPair
{
private:
    llvm::StringRef _key;
    llvm::StringRef _value;

public:
    const llvm::StringRef &key() const {return _key;}
    const llvm::StringRef &value() const {return _key;}

    void mapping(llvm::yaml::IO& io);
};

class ConfigFile
{
private:
    std::string _path;
    llvm::OwningPtr<llvm::MemoryBuffer> _buffer;
    std::vector<llvm::StringRef> _rules;
    std::vector<llvm::StringRef> _disableRules;
    std::vector<llvm::StringRef> _rulePaths;
    std::vector<RuleConfigurationPair> _ruleConfigurations;

public:
    explicit ConfigFile(const std::string &path);

    const std::string &path() const;
    const std::vector<llvm::StringRef> &rules() const;
    const std::vector<llvm::StringRef> &disableRules() const;
    const std::vector<llvm::StringRef> &rulePaths() const;
    const std::vector<RuleConfigurationPair> &ruleConfigurations() const;

    void mapping(llvm::yaml::IO& io);
};

/** Read all config files in order of ascending priority. */
std::vector<ConfigFile> readConfigFiles();

} // end namespace option
} // end namespace oclint

#endif
