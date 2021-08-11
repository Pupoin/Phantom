#include "Core/ControlManager.h"

// Required by Singleton
ControlManager *pControlMgr = nullptr;

// Get Instance Class
ControlManager *ControlManager::CreateInstance() {
    if (pControlMgr == nullptr)
        pControlMgr = new ControlManager();

    return pControlMgr;
}

ControlManager::ControlManager() = default;

void ControlManager::run() {
    //pEvtReader->getEntryNext();
}


void print_title(const std::string &str) {
    printf("########################################### \n");
    printf("# %s \n", str.data());
    printf("########################################### \n");
}

void ControlManager::generate_config() {
    // Automatically Generate a YAML config file with all defaults
    printf("--- \n# Default configuration file for Phantom Analysis \n \n");

    // Basic Settings
    print_title("Basic Settings");
    printf("input_file: \"p_out.root\" \n");
    printf("geometry_file: \"p_out.root\" \n");
    printf("output_file: \"p_ana.root\" \n \n");

    printf("event_number: -1 \t# number of events to be analyzed \n");
    printf("skip_number: 0 \t\t# number of events to be skipped \n \n");

    // Verbosity Settings
    print_title("Verbosity Settings");

    // Analyzers List
    print_title("Analyzer Lists");

    // Analyzers Settings
    print_title("Analyzer Settings");
    
}
