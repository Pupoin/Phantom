#ifndef PCTSIMULATION_ANALYZERMANAGER_H
#define PCTSIMULATION_ANALYZERMANAGER_H

#include "Analyzer/PCTAnalyzer.h"
#include "Core/EventReader.h"

class AnalyzerManager {
public:
    static AnalyzerManager *CreateInstance();

    void RegisterAnalyzer(PCTAnalyzer *);

    void setVerbose(int verbose) { Verbose = verbose; }

    // Summary Log
    void PrintRunLog();

    // Register Processors
    void setAnalyzerList(const vector<std::string> &analyzerList);

    [[nodiscard]] const map<std::string, PCTAnalyzer *> &getAnalyzerCol() const { return analyzer_col; }

    [[nodiscard]] const vector<std::string> &getAnalyzerList() const { return analyzer_list; }

    // Run AnaProcessor
    void InitializeAnalyzers();

    void BeginAnalyzers();

    void ProcessEvtAnalyzers(PCTEvent *);

    void CheckEvtAnalyzers(PCTEvent *);

    void EndAnalyzers();


private:
    std::map<std::string, PCTAnalyzer *> analyzer_col;
    std::vector<std::string> analyzer_list;

    // Time and Event Log
    long long Processed_Evt{0};
    clock_t start_processing{};
    clock_t end_processing{};
    clock_t global_start{};
    clock_t global_end{};

    int Verbose{0};

    std::map<std::string, double> processing_avg_time;

// Required for Singleton
private:
    AnalyzerManager();

public:
    // Delete Copy constructor
    AnalyzerManager(const AnalyzerManager &) = delete;

    AnalyzerManager &operator=(AnalyzerManager const &) = delete;
};

extern AnalyzerManager *pAnaMgr;

#endif //PCTSIMULATION_ANALYZERMANAGER_H
