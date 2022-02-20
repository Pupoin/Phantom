//
// Created by Jifeng Hu on 19/9/21, hujf@scnu.edu.cn
//

#ifndef PCTSIMULATION_ENERGYDEPOSITIONCALIBRATOR_H
#define PCTSIMULATION_ENERGYDEPOSITIONCALIBRATOR_H

#include "Analyzer/PCTAnalyzer.h"

#include <iostream>
#include <fstream>
#include <map>
#include "TH2D.h"
#include "TFile.h"

/*
this calibrator is used to revise the dE/dx so as to 
obtain a well-defined dE/dx not independent on the 
the momentum of projectile protons.
*/

class EnergyDepositionCalibrator : public PCTAnalyzer {
public:
    EnergyDepositionCalibrator();

    // Override virtual function of base class
    void Begin() override;

    void ProcessEvt(PCTEvent *event) override;

    void CheckEvt(PCTEvent *event) override;

    void End() override;

private:
    std::string out_file_name;

	TFile* fFile;
    TH2D* hdEdx;

	int fNUsed;

};


#endif //PCTSIMULATION_IMAGINGRECOUSINGDEDX_H