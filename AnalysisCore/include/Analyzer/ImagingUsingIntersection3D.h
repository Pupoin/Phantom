//
// Created by Jifeng Hu on 17/9/21, hujf@scnu.edu.cn
//

#ifndef PCTSIMULATION_IMAGINGUSINGINTERSECTION3D_H
#define PCTSIMULATION_IMAGINGUSINGINTERSECTION3D_H

#include "Analyzer/PCTAnalyzer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include "TH2D.h"
#include "TFile.h"
#include <assert.h>

class ImagingUsingIntersection3D : public PCTAnalyzer {
public:
    ImagingUsingIntersection3D();

    // Override virtual function of base class
    void Begin() override;

    void ProcessEvt(PCTEvent *event) override;

    void CheckEvt(PCTEvent *event) override;

    void End() override;

private:
	TFile* fFile;
    std::string out_file_name;
    std::vector<TH2D*> fImages;//image for each slice along z
    std::vector<TH1D*> fThetas;//scattering angle for each pixel per slice


	int fNTot;
	int fNUsed;

    const double depth_unit = 1.0;// z direction, unit: mm
    const double width_unit = 0.5;// x direction, unit: mm
    const double height_unit = 0.5;// y direction, unit: mm
    const double xlower = -15.;//mm
    const double xupper = 15.;
    const double ylower = -20.;
    const double yupper = 20.;
    const double zlower = -25.;
    const double zupper = 25.;

    const int cDepth= int((zupper-zlower)/depth_unit);//z
    const int cHeight = int((yupper-ylower)/height_unit);//y
    const int cWidth = int((xupper-xlower)/width_unit);//x

    int pixel2index(int d, int y, int x)
    {
        if ((d < 0 || d >= cDepth) || (y < 0 && y >= cHeight) || (x < 0 && x >= cWidth))
            return -1;
        return d * cWidth * cHeight + y * cWidth + x;
    }
    void index2pixel(int index, int &d, int &y, int &x)
    {
        assert(index >= 0 && index < cDepth * cHeight * cWidth);
        d = index / (cWidth * cHeight);
        y = (index - d * cWidth * cHeight) / cWidth;
        x = (index - d * cWidth * cHeight) % cWidth;
    }
};

#endif //PCTSIMULATION_IMAGINGRECOUSINGDEDX_H