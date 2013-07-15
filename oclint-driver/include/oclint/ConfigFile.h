#ifndef OCLINT_CONFIGFILE_H
#define OCLINT_CONFIGFILE_H

#include "llvm/ADT/Optional.h"
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

enum tristate {FALSE, TRUE, UNDEFINED};

class ConfigFile
{
private:
    std::string _path;
    llvm::OwningPtr<llvm::MemoryBuffer> _buffer;
    std::vector<llvm::StringRef> _rules;
    std::vector<llvm::StringRef> _disableRules;
    std::vector<llvm::StringRef> _rulePaths;
    std::vector<RuleConfigurationPair> _ruleConfigurations;
    llvm::StringRef _output;
    llvm::StringRef _reportType;
    int _maxP1;
    int _maxP2;
    int _maxP3;
    tristate _clangChecker;

public:
    explicit ConfigFile(const std::string &path);

    const std::string &path() const;
    const std::vector<llvm::StringRef> &rules() const;
    const std::vector<llvm::StringRef> &disableRules() const;
    const std::vector<llvm::StringRef> &rulePaths() const;
    const std::vector<RuleConfigurationPair> &ruleConfigurations() const;
    llvm::Optional<std::string> output() const;
    llvm::Optional<std::string> reportType() const;
    llvm::Optional<int> maxP1() const;
    llvm::Optional<int> maxP2() const;
    llvm::Optional<int> maxP3() const;
    llvm::Optional<bool> clangChecker() const;

    void mapping(llvm::yaml::IO& io);
};

/** Read all config files in order of ascending priority. */
std::vector<ConfigFile> readConfigFiles();

} // end namespace option
} // end namespace oclint

#endif