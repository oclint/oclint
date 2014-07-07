#include "oclint/ConfigFile.h"

#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/YAMLParser.h>

#include "oclint/Logger.h"

using namespace oclint;
using namespace oclint::option;

/* ---------------------
   RuleConfigurationPair
   ------------------ */

LLVM_YAML_IS_SEQUENCE_VECTOR(RuleConfigurationPair)

namespace llvm {
namespace yaml {

template <>
struct MappingTraits<RuleConfigurationPair>
{
    static void mapping(IO& inputOutput, RuleConfigurationPair& ruleConfiguration)
    {
        ruleConfiguration.mapping(inputOutput);
    }
};

}
}

const llvm::StringRef &oclint::option::RuleConfigurationPair::key() const
{
    return _key;
}

const llvm::StringRef &oclint::option::RuleConfigurationPair::value() const
{
    return _value;
}

void oclint::option::RuleConfigurationPair::mapping(llvm::yaml::IO& inputOutput)
{
    inputOutput.mapOptional("key", _key);
    inputOutput.mapOptional("value", _value);
}

/* ----------
   ConfigFile
   ---------- */

LLVM_YAML_IS_SEQUENCE_VECTOR(llvm::StringRef)

namespace llvm {
namespace yaml {

template <>
struct MappingTraits<ConfigFile>
{
    static void mapping(IO& inputOutput, ConfigFile& config)
    {
        config.mapping(inputOutput);
    }
};

template <>
struct ScalarEnumerationTraits<TriState>
{
    static void enumeration(IO &inputOutput, TriState &value)
    {
        inputOutput.enumCase(value, "true", TriState::TRUE);
        inputOutput.enumCase(value, "false", TriState::FALSE);
    }
};

}
}

oclint::option::ConfigFile::ConfigFile(const std::string &path)
    : _path(path), _maxP1(INT_MIN), _maxP2(INT_MIN), _maxP3(INT_MIN)
{
    LOG_DEBUG("Reading config file: ");
    LOG_DEBUG_LINE(path.c_str());

    llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buffer =
        llvm::MemoryBuffer::getFile(path);
    if (buffer)
    {
        const llvm::StringRef &content = buffer.get()->getBuffer();

        const std::string whitespace(" \t\f\v\n\r");
        if (content.str().find_last_not_of(whitespace) == std::string::npos) {
            // Config file is only whitespace.
            // Prevent LLVM crash:
            // http://lists.cs.uiuc.edu/pipermail/llvmbugs/2013-May/028254.html
            LOG_DEBUG_LINE("Skip parsing empty config file");
            return;
        }

        llvm::yaml::Input yin(content);
        yin >> *this;
    }
    else
    {
        LOG_DEBUG_LINE(buffer.getError().message().c_str());
    }
}

const std::string &oclint::option::ConfigFile::path() const
{
    return _path;
}

const std::vector<llvm::StringRef> &oclint::option::ConfigFile::rules() const
{
    return _rules;
}

const std::vector<llvm::StringRef> &oclint::option::ConfigFile::disableRules() const
{
    return _disableRules;
}

const std::vector<llvm::StringRef> &oclint::option::ConfigFile::rulePaths() const
{
    return _rulePaths;
}

const std::vector<RuleConfigurationPair> &oclint::option::ConfigFile::ruleConfigurations() const
{
    return _ruleConfigurations;
}

static llvm::Optional<std::string> createOptionalString(const llvm::StringRef &value)
{
    const std::string &string = value.str();
    return string.size() ? llvm::Optional<std::string>(string) : llvm::Optional<std::string>();
}

llvm::Optional<std::string> oclint::option::ConfigFile::output() const
{
    return createOptionalString(_output);
}

llvm::Optional<std::string> oclint::option::ConfigFile::reportType() const
{
    return createOptionalString(_reportType);
}

static llvm::Optional<int> createOptionalInt(int value)
{
    return value == INT_MIN ? llvm::Optional<int>() : llvm::Optional<int>(value);
}

llvm::Optional<int> oclint::option::ConfigFile::maxP1() const
{
    return createOptionalInt(_maxP1);
}

llvm::Optional<int> oclint::option::ConfigFile::maxP2() const
{
    return createOptionalInt(_maxP2);
}

llvm::Optional<int> oclint::option::ConfigFile::maxP3() const
{
    return createOptionalInt(_maxP3);
}

static llvm::Optional<bool> createOptionalBool(const TriState value)
{
    switch (value)
    {
    case FALSE:
        return llvm::Optional<bool>(false);
    case TRUE:
        return llvm::Optional<bool>(true);
    case UNDEFINED:
        return llvm::Optional<bool>();
    }
}

llvm::Optional<bool> oclint::option::ConfigFile::clangChecker() const
{
    return createOptionalBool(_clangChecker);
}

llvm::Optional<bool> oclint::option::ConfigFile::allowDuplicatedViolations() const
{
    return createOptionalBool(_allowDuplicatedViolations);
}

void oclint::option::ConfigFile::mapping(llvm::yaml::IO& inputOutput)
{
    inputOutput.mapOptional("rules", _rules);
    inputOutput.mapOptional("disable-rules", _disableRules);
    inputOutput.mapOptional("rule-paths", _rulePaths);
    inputOutput.mapOptional("rule-configurations", _ruleConfigurations);
    inputOutput.mapOptional("output", _output);
    inputOutput.mapOptional("report-type", _reportType);
    inputOutput.mapOptional("max-priority-1", _maxP1);
    inputOutput.mapOptional("max-priority-2", _maxP2);
    inputOutput.mapOptional("max-priority-3", _maxP3);
    inputOutput.mapOptional("enable-clang-static-analyzer", _clangChecker);
    inputOutput.mapOptional("allow-duplicated-violations", _allowDuplicatedViolations);
}
