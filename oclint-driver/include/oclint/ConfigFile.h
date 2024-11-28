#ifndef OCLINT_CONFIGFILE_H
#define OCLINT_CONFIGFILE_H

#include <vector>

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
    TriState _enableGlobalAnalysis = UNDEFINED;

public:
    explicit ConfigFile(const std::string &path);

    const std::string &path() const;
    const std::vector<llvm::StringRef> &rules() const;
    const std::vector<llvm::StringRef> &disableRules() const;
    const std::vector<llvm::StringRef> &rulePaths() const;
    const std::vector<RuleConfigurationPair> &ruleConfigurations() const;
    std::optional<std::string> output() const;
    std::optional<std::string> reportType() const;
    std::optional<int> maxP1() const;
    std::optional<int> maxP2() const;
    std::optional<int> maxP3() const;
    std::optional<bool> clangChecker() const;
    std::optional<bool> allowDuplicatedViolations() const;
    std::optional<bool> enableGlobalAnalysis() const;

    void mapping(llvm::yaml::IO& io);
};

} // end namespace option
} // end namespace oclint

#endif
