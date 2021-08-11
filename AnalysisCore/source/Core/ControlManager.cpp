#include "Core/EventReader.h"
#include "Core/ConfigManager.h"
#include "Core/ControlManager.h"
#include "Core/AnalyzerManager.h"

// Required by Singleton
ControlManager *pControlMgr = nullptr;

// Get Instance Class
ControlManager *ControlManager::CreateInstance() {
    if (pControlMgr == nullptr)
        pControlMgr = new ControlManager();

    return pControlMgr;
}

ControlManager::ControlManager() = default;


void ControlManager::initialize() {
    pAnaMgr->InitializeAnalyzers();
}

void ControlManager::run() {

    // Read Data from ROOT file
    pEvtReader->setInput("test.root", "pct");
    /*
     *  Processing
     */
    pAnaMgr->BeginAnalyzers();

    while (auto evt = pEvtReader->getEntryNext()) {
        if (Verbose > 1) {
            cout << "--------------------------";
            cout << " Process Event: " << pEvtReader->getCurrentEventNumber();
            cout << " --------------------------" << endl;
        }
#ifdef MEMCK
        if (false)
            PEvent::PrintObjectStatistics(Form("Begin of Event: %d", pEvtReader->getCurrentEventNumber()));
#endif
        // process algorithms
        pAnaMgr->ProcessEvtAnalyzers(evt);

        // check algorithms
        pAnaMgr->CheckEvtAnalyzers(evt);

        if (Verbose > 1) {
            cout << "--------------------------";
            cout << " End of Event:  " << pEvtReader->getCurrentEventNumber();
            cout << " --------------------------" << endl;
        }

        evt->Initialization(nALL);

#ifdef MEMCK
        if (false)
            PEvent::PrintObjectStatistics(Form("End of Event: %d", pEvtReader->getCurrentEventNumber()));
#endif

    }

    /*
     *  End
     */
    pAnaMgr->EndAnalyzers();
    pAnaMgr->PrintRunLog();

    std::cout << std::endl << " ==> Done ..." << std::endl;
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

