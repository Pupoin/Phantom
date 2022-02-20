//
// Created by Jifeng Hu on 20/9/21, hujf@scnu.edu.cn
//

#ifndef PCTSIMULATION_IMAGINGUSINGCALIBDEDX_H
#define PCTSIMULATION_IMAGINGUSINGCALIBDEDX_H

#include "Analyzer/PCTAnalyzer.h"

#include <iostream>
#include <fstream>
#include <map>
#include "TH2D.h"
#include "TH1D.h"
#include "TFile.h"

class ImagingUsingCalibdEdx : public PCTAnalyzer {
public:
    ImagingUsingCalibdEdx();

    // Override virtual function of base class
    void Begin() override;

    void ProcessEvt(PCTEvent *event) override;

    void CheckEvt(PCTEvent *event) override;

    void End() override;

private:
    std::string out_file_name;

	TFile* fFile;
    TH2D* himg;
    TH1D* hdEdx;
    TF1* fmodel;//a model used to calibrate dEdx

	int fNUsed;

};


#endif //PCTSIMULATION_IMAGINGRECOUSINGDEDX_H
