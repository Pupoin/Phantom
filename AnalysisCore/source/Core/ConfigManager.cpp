#include "Core/ConfigManager.h"

// Required by Singleton
ConfigManager *pConfigMgr = nullptr;

// Get Instance Class
ConfigManager *ConfigManager::CreateInstance() {
    if (pConfigMgr == nullptr)
        pConfigMgr = new ConfigManager();

    return pConfigMgr;
}

void ConfigManager::ReadConfig(const string &yaml_file) {
    string infile;
    if (yaml_file.empty()) infile = "analysis.yaml";
    else infile = yaml_file;
    Node = YAML::LoadFile(infile);

}

ConfigManager::ConfigManager() = default;
