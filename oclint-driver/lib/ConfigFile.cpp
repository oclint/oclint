#include "llvm/Support/YAMLParser.h"
#include "llvm/Support/SourceMgr.h"

#include "oclint/ConfigFile.h"
#include "oclint/Debug.h"

using namespace oclint;
using namespace oclint::option;

/* ---------------------
   RuleConfigurationPair
   ------------------ */

LLVM_YAML_IS_SEQUENCE_VECTOR(RuleConfigurationPair)

template <>
struct llvm::yaml::MappingTraits<RuleConfigurationPair>
{
    static void mapping(IO& inputOutput, RuleConfigurationPair& ruleConfiguration)
    {
        ruleConfiguration.mapping(inputOutput);
    }
};

void oclint::option::RuleConfigurationPair::mapping(llvm::yaml::IO& inputOutput)
{
    inputOutput.mapOptional("key", _key);
    inputOutput.mapOptional("value", _value);
}

/* ----------
   ConfigFile
   ---------- */

LLVM_YAML_IS_SEQUENCE_VECTOR(llvm::StringRef)

template <>
struct llvm::yaml::MappingTraits<ConfigFile>
{
    static void mapping(IO& inputOutput, ConfigFile& config)
    {
        config.mapping(inputOutput);
    }
};

template<>
struct llvm::yaml::ScalarEnumerationTraits<tristate>
{
    static void enumeration(IO &inputOutput, tristate &value)
    {
        inputOutput.enumCase(value, "true", tristate::TRUE);
        inputOutput.enumCase(value, "false", tristate::FALSE);
    }
};

oclint::option::ConfigFile::ConfigFile(const std::string &path)
    : _path(path), _maxP1(INT_MIN), _maxP2(INT_MIN), _maxP3(INT_MIN)
{
    debug::emit("Reading config file: ");
    debug::emit(path.c_str());

    llvm::error_code errorCode = llvm::MemoryBuffer::getFile(path, _buffer);
    if (errorCode)
    {
        debug::emit(errorCode.message().c_str());
        debug::emit("\n");
    }
    else
    {
        llvm::yaml::Input yin(_buffer->getBuffer());
        yin >> *this;
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

static llvm::Optional<bool> createOptionalBool(const tristate value)
{
    switch (value)
    {
    case FALSE:
        return llvm::Optional<bool>(false);
    case TRUE:
        return llvm::Optional<bool>(true);
    case UNDEFINED:
    default:
        return llvm::Optional<bool>();
    }
}

llvm::Optional<bool> oclint::option::ConfigFile::clangChecker() const
{
    return createOptionalBool(_clangChecker);
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
    // Can't use bool here, because we need to detect the unset state.
    // Optional<bool> does not work, either, because it doesn't support ==.
    inputOutput.mapOptional("enable-clang-static-analyzer", _clangChecker, tristate::UNDEFINED);
}

/* ---------
   functions
   --------- */

static std::vector<std::string> configFilePaths()
{
    std::vector<std::string> paths;
    paths.push_back("/etc/oclint");
    const char *home = getenv("HOME");
    if (home)
    {
        paths.push_back(std::string(home) + "/.oclint");
    }
    paths.push_back(".oclint");
    return paths;
}

std::vector<ConfigFile> oclint::option::readConfigFiles()
{
    std::vector<ConfigFile> configFiles;
    for (const std::string &path : configFilePaths())
    {
        configFiles.push_back(ConfigFile(path));
    }
    return configFiles;
}
