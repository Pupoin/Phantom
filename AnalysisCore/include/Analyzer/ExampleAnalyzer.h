//
// Created by Avencast on 8/12/2021.
//

#ifndef PCTSIMULATION_EXAMPLEANALYZER_H
#define PCTSIMULATION_EXAMPLEANALYZER_H

#include "Analyzer/PAnalyzer.h"

class ExampleAnalyzer : public PAnalyzer {
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
};

#endif //PCTSIMULATION_EXAMPLEANALYZER_H
