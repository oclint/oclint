#ifndef OCLINT_CONFIGFILE_H
#define OCLINT_CONFIGFILE_H

#include <vector>

#include <llvm/ADT/Optional.h>
#include <llvm/Support/YAMLTraits.h>
#include <llvm/Support/MemoryBuffer.h>

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
    const llvm::StringRef &key() const;
    const llvm::StringRef &value() const;

    void mapping(llvm::yaml::IO& io);
};

enum TriState {FALSE, TRUE, UNDEFINED};

class ConfigFile
{
private:
    std::string _path;
    std::vector<llvm::StringRef> _rules;
    std::vector<llvm::StringRef> _disableRules;
    std::vector<llvm::StringRef> _rulePaths;
    std::vector<RuleConfigurationPair> _ruleConfigurations;
    llvm::StringRef _output;
    llvm::StringRef _reportType;
    int _maxP1;
    int _maxP2;
    int _maxP3;
    TriState _clangChecker = UNDEFINED;
    TriState _allowDuplicatedViolations = UNDEFINED;

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
    llvm::Optional<bool> allowDuplicatedViolations() const;

    void mapping(llvm::yaml::IO& io);
};

} // end namespace option
} // end namespace oclint

#endif
