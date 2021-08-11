#include "Core/AnalyzerManager.h"

/* Include all needed analyzers here */
#include "Analyzer/ExampleAnalyzer.h"

// Required by Singleton
AnalyzerManager *pAnaMgr = nullptr;

// Get Instance Class
AnalyzerManager *AnalyzerManager::CreateInstance() {
    if (pAnaMgr == nullptr)
        pAnaMgr = new AnalyzerManager();

    return pAnaMgr;
}

AnalyzerManager::AnalyzerManager() = default;


void AnalyzerManager::InitializeAnalyzers() {
    // Register Example Analyzer to AnalyzerManager
    this->RegisterAnalyzer(new ExampleAnalyzer);
}


void AnalyzerManager::RegisterAnalyzer(PAnalyzer *analyzer) {
    if (analyzer_col.count(analyzer->getName()) != 0) {
        std::cerr << "[WARNING] ==> Analyzer " + analyzer->getName() + " already exists." << std::endl;
    } else {
        analyzer_col.insert(std::make_pair(analyzer->getName(), analyzer));
        analyzer_list.emplace_back(analyzer->getName());
    }
}

void AnalyzerManager::BeginAnalyzers() {
    Processed_Evt = 0;
    global_start = clock();

    if (Verbose > 0) {
        cout << "======================================================================" << endl;
        cout << "[ ALGO PROCESS LIST ] : (Verbosity 1)" << endl;
        cout << std::right;
        cout << std::setw(10) << "----" << std::setw(50) << "-----------" << endl;
        cout << std::setw(10) << "name" << std::setw(50) << "Description" << endl;
        cout << std::setw(10) << "----" << std::setw(50) << "-----------" << endl;
    }

    for (auto itr = analyzer_list.begin(); itr != analyzer_list.end(); itr++) {
        if (analyzer_col.count(*itr) == 0) {
            itr = analyzer_list.erase(itr);
            if (itr == analyzer_list.end()) break;
        }
        // Initialize Processor
        analyzer_col.at(*itr)->Begin();

        // Initialize Timer
        processing_avg_time.insert(std::pair<std::string, double>(*itr, 0.));

        if (Verbose > 0) {
            cout << std::right << std::setw(5) << " " << std::left;
            cout << std::setw(39) << *itr;
            cout << std::setw(50) << analyzer_col.at(*itr)->getDescription() << endl;
        }
    }
}

void AnalyzerManager::ProcessEvtAnalyzers(PEvent *evt) {
    for (const auto &itr : analyzer_list) {

        // record start time for each processor
        start_processing = clock();

        if (Verbose > 2) {
            cout << "[ PROCESSOR ] (Verbosity 3) : " << itr << endl;
        }

        try {
            // process evt
            analyzer_col.at(itr)->ProcessEvt(evt);
        } catch (const std::out_of_range &oor) {
            std::cerr << "[ERROR] Evt: " << Processed_Evt << " -- Out of Range error: " << oor.what() << " in Algo"
                      << itr << endl;
        } catch (...) {
            std::cerr << "[ERROR] Evt: " << Processed_Evt << " --  Some unknown errors appear. Please check: " << itr
                      << endl;
        }

        // record end time for each processor
        end_processing = clock();

        // calculate average processing time (second)
        processing_avg_time.at(itr) = processing_avg_time.at(itr) * static_cast<double>(Processed_Evt) +
                                      double(end_processing - start_processing) / CLOCKS_PER_SEC;

        processing_avg_time.at(itr) /= static_cast<double>(Processed_Evt + 1);
    }

    Processed_Evt++;
}

void AnalyzerManager::CheckEvtAnalyzers(PEvent *evt) {
    for (const auto &itr : analyzer_list)
        analyzer_col.at(itr)->CheckEvt(evt);
}

void AnalyzerManager::EndAnalyzers() {
    for (const auto &itr : analyzer_list)
        analyzer_col.at(itr)->End();

    global_end = clock();
}

void AnalyzerManager::SetAnalyzerList(const std::string &AnalyzerList) {

    analyzer_list.clear();
    std::istringstream sin(AnalyzerList);
    do {
        std::string ProcessorName;
        sin >> ProcessorName;

        if (!ProcessorName.empty()) {
            if (std::find(analyzer_list.begin(), analyzer_list.end(), ProcessorName) == analyzer_list.end()) {
                {
                    if (analyzer_col.count(ProcessorName) != 0)
                        analyzer_list.emplace_back(ProcessorName);
                    else
                        std::cerr << "[WARNING] ==> No Analyzer named: " << ProcessorName << std::endl;
                }
            } else
                std::cerr << "[WARNING] ==> Duplicate Analyzer Name." << std::endl;
        }
    } while (sin);
}


void AnalyzerManager::PrintRunLog() {
    cout << "======================================================================" << endl;
    cout << "---------------------------> Run Summary <----------------------------" << endl;

    double total_time = 0.;

    cout << std::left;
    cout << std::setw(5) << " " << std::setw(35) << "Processor Name";
    cout << std::setw(40) << "Execution Time / Event [sec]" << endl;
    cout << std::fixed << std::setprecision(8);
    for (const auto &itr : analyzer_list) {

        cout << std::setw(5) << " " << std::setw(40) << itr;
        cout << std::setw(40) << processing_avg_time.at(itr) << endl;

        total_time += processing_avg_time.at(itr);
    }
    cout << "----------------------------------------------------------------------" << endl;
    cout << std::setw(5) << " " << std::setw(26) << "Total Processed Event(s): " << std::setw(14) << Processed_Evt
         << endl;
    cout << std::setw(5) << " " << std::setw(26) << "Total Processing Time: "
         << (double) (global_end - global_start) / CLOCKS_PER_SEC;
    cout << " [sec]" << endl;
    cout << "======================================================================" << endl;
}
