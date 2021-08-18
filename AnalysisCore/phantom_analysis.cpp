//
// Created by Zhang Yulei on 9/18/20.
//

#include <iostream>

#include "Core/EventReader.h"
#include "Core/ConfigManager.h"
#include "Core/ControlManager.h"
#include "Core/AnalyzerManager.h"
#include "Core/GeometryHelper.h"

using std::cerr, std::endl;
using std::string;

namespace {
    void PrintUsage() {
        cerr << " Usage: " << endl;
        cerr << " PAna [ config.yaml ] [ -x ] [ -h ]" << endl;
        cerr << "   note: -x option will print all available Analyzers" << endl;
        cerr << "   note: -h option will print this information" << endl;
        cerr << endl;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        PrintUsage();
        return 1;
    }

    bool print_usage = false;
    std::string configfile;
    if (std::string(argv[1]) == "-x")
        print_usage = true;
    else if (std::string(argv[1]) == "-h") {
        PrintUsage();
        return 1;
    } else
        configfile = argv[1];

    GeometryHelper::CreateInstance();
    EventReader::CreateInstance();
    ConfigManager::CreateInstance();
    ControlManager::CreateInstance();
    AnalyzerManager::CreateInstance();
    
    pControlMgr->initialize();
    if (print_usage) {
        ControlManager::generate_config();
        return 0;
    }

    pConfigMgr->ReadConfig(configfile);

    pControlMgr->run();


    return 0;
}