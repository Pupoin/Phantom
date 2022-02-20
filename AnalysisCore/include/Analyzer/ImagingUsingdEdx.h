//
// Created by Jifeng Hu on 16/9/21, hujf@scnu.edu.cn
//

#ifndef PCTSIMULATION_IMAGINGRECOUSINGDEDX_H
#define PCTSIMULATION_IMAGINGRECOUSINGDEDX_H

#include "Analyzer/PCTAnalyzer.h"

#include <iostream>
#include <fstream>
#include <map>
#include "TH2D.h"
#include "TFile.h"

class ImagingUsingdEdx : public PCTAnalyzer {
public:
    ImagingUsingdEdx();

    // Override virtual function of base class
    void Begin() override;

    void ProcessEvt(PCTEvent *event) override;

    void CheckEvt(PCTEvent *event) override;

    void End() override;

private:
    std::string out_file_name;

	TFile* fFile;
    TH2D* himg;

	int fNUsed;

};


#endif //PCTSIMULATION_IMAGINGRECOUSINGDEDX_H
