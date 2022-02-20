//
// Created by Jifeng Hu on 17/9/21, hujf@scnu.edu.cn
//

#ifndef PCTSIMULATION_IMAGINGUSINGINTERSECTION2D_H
#define PCTSIMULATION_IMAGINGUSINGINTERSECTION2D_H

#include "Analyzer/PCTAnalyzer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "TH2D.h"
#include "TFile.h"
#include <assert.h>

class ImagingUsingIntersection2D : public PCTAnalyzer {
public:
    ImagingUsingIntersection2D();

    // Override virtual function of base class
    void Begin() override;

    void ProcessEvt(PCTEvent *event) override;

    void CheckEvt(PCTEvent *event) override;

    void End() override;

private:
	TFile* fFile;
    std::string out_file_name;
    TH2D* fImage;//image for each pixel along z
    std::vector<TH1D*> fThetaDistributions;//scattering angle for each pixel per slice


	int fNTot;
	int fNUsed;

    const double width_unit = 0.5;// x direction, unit: mm
    const double height_unit = 0.5;// y direction, unit: mm
    const double xlower = -15.;//mm
    const double xupper = 15.;
    const double ylower = -20.;
    const double yupper = 20.;

    const int cHeight = int((yupper-ylower)/height_unit);//y
    const int cWidth = int((xupper-xlower)/width_unit);//x

    int pixel2index(int y, int x)
    {
        if ((y < 0 || y >= cHeight) || (x < 0 || x >= cWidth))
            return -1;
        return y * cWidth + x;
    }
    void index2pixel(int index, int &y, int &x)
    {
        assert(index >= 0 && index < cHeight * cWidth);
        y = index / cWidth;
        x = index % cWidth;
    }
};

#endif //PCTSIMULATION_IMAGINGRECOUSINGDEDX_H