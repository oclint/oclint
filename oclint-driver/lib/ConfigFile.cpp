#include "llvm/Support/YAMLParser.h"
#include "llvm/Support/YAMLTraits.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/SourceMgr.h"

#include "oclint/ConfigFile.h"
#include "oclint/Debug.h"

using namespace oclint;
using namespace oclint::option;

LLVM_YAML_IS_SEQUENCE_VECTOR(RuleConfigurationPair)

template <>
struct llvm::yaml::MappingTraits<RuleConfigurationPair>
{
    static void mapping(IO& io, RuleConfigurationPair& ruleConfiguration)
    {
        io.mapOptional("key", ruleConfiguration.key);
        io.mapOptional("value", ruleConfiguration.value);
    }
};

LLVM_YAML_IS_SEQUENCE_VECTOR(llvm::StringRef)

template <>
struct llvm::yaml::MappingTraits<ConfigFile>
{
    static void mapping(IO& io, ConfigFile& config)
    {
        io.mapOptional("rules", config.rules);
        io.mapOptional("disable-rules", config.disableRules);
        io.mapOptional("rule-paths", config.rulePaths);
        io.mapOptional("rule-configurations", config.ruleConfigurations);
    }
};

static ConfigFile readConfigFromFile(const std::string &path)
{
    debug::emit("Reading config file: ");
    debug::emit(path.c_str());

    ConfigFile configFile;
    configFile.path = path;

    llvm::error_code ec = llvm::MemoryBuffer::getFile(path, configFile.buffer);
    if (ec)
    {
        debug::emit(ec.message().c_str());
        debug::emit("\n");
    }
    else
    {
        llvm::yaml::Input yin(configFile.buffer->getBuffer());
        yin >> configFile;
    }
    return configFile;
}

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
    const std::vector<std::string> paths = configFilePaths();
    std::vector<ConfigFile> configFiles;
    transform(paths.begin(), paths.end(), std::back_inserter(configFiles), readConfigFromFile);
    return configFiles;
}
