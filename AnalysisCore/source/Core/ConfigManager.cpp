#include "Core/ConfigManager.h"

#include "Core/EventReader.h"
#include "Core/ControlManager.h"
#include "Core/AnalyzerManager.h"
#include "Analyzer/PCTAnalyzer.h"

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

    ReadBasic();
    ReadVerbosity();
    ReadAnalyzers();
}

void ConfigManager::ReadBasic() {
    try {
        pControlMgr->file_name = Node["input_file"].as<std::string>();
        pControlMgr->geom_name = Node["geometry_file"].as<std::string>();
        pControlMgr->input_tree = Node["input_tree"].as<std::string>();
    } catch (YAML::BadConversion &e) {
        std::cerr << "[Read YAML] : Input File ==> " << e.msg << std::endl;
        exit(EXIT_FAILURE);
    } catch (YAML::InvalidNode &e) {
        std::cerr << "[Read YAML] : Input File  ==> " << e.msg << std::endl;
        exit(EXIT_FAILURE);
    }

    try {
        pEvtReader->setEventNumber(Node["event_number"].IsDefined() ? Node["event_number"].as<int>() : -1);
        pEvtReader->setSkipNumber(Node["skip_number"].IsDefined() ? Node["skip_number"].as<int>() : 0);
    } catch (YAML::BadConversion &e) {
        std::cerr << "[Read YAML] : Event/Skip Number  ==> " << e.msg << std::endl;
        exit(EXIT_FAILURE);
    } catch (YAML::InvalidNode &e) {
        std::cerr << "[Read YAML] : Event/Skip Number  ==> " << e.msg << std::endl;
        exit(EXIT_FAILURE);
    }

}

void ConfigManager::ReadVerbosity() {
    if (auto n = Node["Verbosity"]; n.IsDefined()) {
        pEvtReader->setVerbose(n["EventReader"].IsDefined() ? n["EventReader"].as<int>() : 0);
        pControlMgr->setVerbose(n["ControlManager"].IsDefined() ? n["ControlManager"].as<int>() : 0);
        pAnaMgr->setVerbose(n["AnalyzerManager"].IsDefined() ? n["AnalyzerManager"].as<int>() : 0);
    }
}

void ConfigManager::ReadAnalyzers() {
    // Read Activated Analyzer Lists
    if (auto n = Node["Analyzer_List"]; n.IsDefined()) {
        std::vector<std::string> tmp;
        for (auto str : n) {
            tmp.push_back(str.as<std::string>());
        }
        pAnaMgr->setAnalyzerList(tmp);
    } else {
        std::cerr << "[Read YAML] : Analyzer_List  ==> Not Defined" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Read Parameters for each analyzer
    if (auto n = Node["Analyzer_Settings"]; n.IsDefined()) {
        for (auto ana : n) {
            auto cur_ana = pAnaMgr->getAnalyzerCol().at(ana.first.as<std::string>())->getAnaVarCol();
            for (auto par : ana.second) {
                auto cur_par = cur_ana.at(par.first.as<std::string>());
                assignAnaVar(std::get<1>(cur_par), std::get<0>(cur_par), par.second);
            }
        }
    } else {
        std::cerr << "[Read YAML] : Analyzer_Settings  ==> Not Defined" << std::endl;
        exit(EXIT_FAILURE);
    }
}

ConfigManager::ConfigManager() = default;
