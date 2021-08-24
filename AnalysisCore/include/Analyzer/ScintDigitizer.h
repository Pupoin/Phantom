#ifndef PCTSIMULATION_ScintDigitizer_H
#define PCTSIMULATION_ScintDigitizer_H

#include "Analyzer/PCTAnalyzer.h"

class ScintDigitizer : public PCTAnalyzer {
public:
    ScintDigitizer();

    // Override virtual function of base class
    void Begin() override;

    void ProcessEvt(PCTEvent *event) override;

    void CheckEvt(PCTEvent *event) override;

    void End() override;

private:
    double nBC408{1.58};                //refraction index
    double cBC408{299.792458 / nBC408}; // effective velocity in BC408 [mm/ns]
    double cADC_per_MeV{1002.};
    double cATTEN_LENGTH{380};   //mm
    double cSCALE_LENGTH{1000.}; //mm

private:
    int fNumOfDigisProduced{};
};

#endif //PCTSIMULATION_ScintDigitizer_H