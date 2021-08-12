#ifndef PCTSIMULATION_CONFIGMANAGER_H
#define PCTSIMULATION_CONFIGMANAGER_H

#include "yaml-cpp/yaml.h"

#include <string>

using std::string;

class ConfigManager {
public:
    static ConfigManager *CreateInstance();

    void ReadConfig(const string &yaml_file);

    void ReadBasic();

    void ReadVerbosity();

    void ReadAnalyzers();

private:
    ConfigManager();

    YAML::Node Node;
public:
    // Delete Copy constructor
    ConfigManager(const ConfigManager &) = delete;

    ConfigManager &operator=(ConfigManager const &) = delete;
};

extern ConfigManager *pConfigMgr;

#endif //PCTSIMULATION_CONFIGMANAGER_H
