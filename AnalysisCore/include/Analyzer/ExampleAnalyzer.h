//
// Created by Avencast on 8/12/2021.
//

#ifndef PCTSIMULATION_EXAMPLEANALYZER_H
#define PCTSIMULATION_EXAMPLEANALYZER_H

#include "Analyzer/PCTAnalyzer.h"

#include "TH1D.h"

class ExampleAnalyzer : public PCTAnalyzer {
public:
    ExampleAnalyzer();

    // Override virtual function of base class
    void Begin() override;

    void ProcessEvt(PCTEvent *event) override;

    void CheckEvt(PCTEvent *event) override;

    void End() override;

private:
    int parameter_1{};
    double parameter_2{};
    vector<string> parameter_3;

    vector<TH1D *> hists;
};

#endif //PCTSIMULATION_EXAMPLEANALYZER_H
