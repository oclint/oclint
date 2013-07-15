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
    static void mapping(IO& io, RuleConfigurationPair& ruleConfiguration)
    {
        ruleConfiguration.mapping(io);
    }
};

void oclint::option::RuleConfigurationPair::mapping(llvm::yaml::IO& io)
{
    io.mapOptional("key", _key);
    io.mapOptional("value", _value);
}

/* ----------
   ConfigFile
   ---------- */

LLVM_YAML_IS_SEQUENCE_VECTOR(llvm::StringRef)

template <>
struct llvm::yaml::MappingTraits<ConfigFile>
{
    static void mapping(IO& io, ConfigFile& config)
    {
        config.mapping(io);
    }
};

oclint::option::ConfigFile::ConfigFile(const std::string &path) : _path(path)
{
    debug::emit("Reading config file: ");
    debug::emit(path.c_str());

    llvm::error_code ec = llvm::MemoryBuffer::getFile(path, _buffer);
    if (ec)
    {
        debug::emit(ec.message().c_str());
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

void oclint::option::ConfigFile::mapping(llvm::yaml::IO& io)
{
    io.mapOptional("rules", _rules);
    io.mapOptional("disable-rules", _disableRules);
    io.mapOptional("rule-paths", _rulePaths);
    io.mapOptional("rule-configurations", _ruleConfigurations);
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
