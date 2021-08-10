//
// Created by Zhang Yulei on 9/18/20.
//

#include <iostream>

#include "Core/AlgoManager.h"
#include "Core/ControlManager.h"

using namespace std;

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
    }
    else
        configfile = argv[1];

    AnaData::CreateInstance();

    auto control = new ControlManager();
    control->setOnlyPrintUsage(print_usage);

    auto evtrdr = new EventReader();
    control->setEvtReader(evtrdr);

    auto algo = new AlgoManager();
    control->setAlgo(algo);
    control->run();

    delete control;
    return 0;
}