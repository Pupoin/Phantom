//
// Created by Jifeng Hu on  17/9/21
//

#include "Analyzer/ImagingUsingIntersection3D.h"
#include "Core/GeometryHelper.h"
#include "TFile.h"
#include "TH2D.h"
#include "TVector3.h"
#include <iomanip>
#include <iostream>
using namespace std;

ImagingUsingIntersection3D::ImagingUsingIntersection3D() 
{
    SetName("ImagingUsingIntersection3D");
    SetTips("imaging the target with scattering points");
    
    RegisterParameter("output_file", &out_file_name, std::string("image_3d_intersection.root"), "output file from ImagingUsingIntersection3D", "std::string");
}
#include "TString.h"
void ImagingUsingIntersection3D::Begin() {
    //compute the width X height X depth;
    //the direction along the proton motion is defined as z+, i.e., depth,
    //the direction perpendicular to the proton direction is x-y plane, horizontal x, vertical y
    fImages.resize( cDepth );
    for (size_t i = 0; i < fImages.size(); ++i)
    {
        TString _histoName = TString::Format("hslice%03zu", i);
        TString _histoTitle = TString::Format("2D slice in x-y plane, depth=%03zu", i);
        fImages[i] = new TH2D(_histoName, _histoTitle, cWidth, xlower, xupper, cHeight, ylower, yupper);
    }
    //using 2D image
    fThetas.resize( cDepth*cWidth*cHeight );
    for(int d =0; d< cDepth; ++d){//z axis
        for(int h = 0; h< cHeight; ++h){//y axis
            for(int w =0; w< cWidth; ++w){//x axis
                int idx = pixel2index(d, h, w);
                TString _histoName = TString::Format("hpixel_d%03d_h%03d_w%03d", d, h, w);
                TString _histoTitle = TString::Format("3D pixel in x-y-z coor., %03d, %03d, %03d", d, h, w);
                fThetas[idx] = new TH1D(_histoName, _histoTitle, 50, 0, 0.5*TMath::Pi());
            }
        }
    }
    fNUsed = 0;
    fNTot = 0;
}

void ImagingUsingIntersection3D::ProcessEvt(PCTEvent *evt)
{
    //const double mn =  938.272013;
    // Find PStep Information
    auto proton_steps = evt->GetData("Init_Particle_Step", ParticleStep_DataType::COL);
    int set_ini = -1;
    int set_fnl = -1;
    TVector3 startingPoint; //
    TVector3 endingPoint;   //
    TVector3 startingDirection;
    TVector3 endingDirection;
    double iniE = -100000;
    double fnlE = -100000;
    //collection required information
    for (auto step : *proton_steps)
    {
        double zz = step->getZ();
        if (zz < -230)
        { //world from beam position to front tracker first layer
        }
        else if (zz < -30)
        { //-230, -30 front tracker
        }
        else if (zz < -25)
        { //-30, -25 world air
        }
        else if (zz < 25. + 1.E-2)
        { //-25, 25 target
            if (set_ini < 0)
            {
                startingPoint.SetXYZ(step->getX(), step->getY(), step->getZ());
                startingDirection.SetXYZ(step->getPx(), step->getPy(), step->getPz());
                iniE = step->getE();
                set_ini = 1;
            }
        }
        else if (zz < 30.)
        { //25, 30 world air
        }
        else if (zz < 230)
        { //30, 230 rear trakcer
            if (set_fnl < 0)
            {
                endingPoint.SetXYZ(step->getX(), step->getY(), step->getZ());
                endingDirection.SetXYZ(step->getPx(), step->getPy(), step->getPz());
                fnlE = step->getE();
                set_fnl = 1;
            }
        }
        else if (zz < 280)
        { //rear tracker to world air
        }
        else if (zz < 285)
        { //scintillator
        }
        else if (zz < 340)
        { //scintillator to air
        }
        else
        { //scintillator > 340
        }
    }
    //computing the intersection3D point and fill in histograms
    if (iniE - fnlE > 0.05 && iniE - fnlE < 20.) //a reasonable energy deposition (unit MeV) is required
    {
        TVector3 n21 = startingDirection.Unit();
        TVector3 n43 = endingDirection.Unit();
        TVector3 dr = startingPoint - endingPoint;
        double d1321 = dr.Dot(n21);
        double d1343 = dr.Dot(n43);
        double d2121 = 1.;
        double d4321 = n43.Dot(n21);
        double d4343 = 1.;
        double ta = (d1343 * d4321 - d1321 * d4343) / (d2121 * d4343 - d4321 * d4321);
        double tb = (d1343 + ta * d4321) / d4343;
        TVector3 ip1 = endingPoint + tb * n43;
        TVector3 ip2 = startingPoint + ta * n21;
        TVector3 ip = 0.5 * (ip1 + ip2);

        int id = int((ip.Z() - zlower) / depth_unit);
        int iy = int((ip.Y() - ylower) / height_unit);
        int ix = int((ip.X() - xlower) / width_unit);
        int index = pixel2index(id, iy, ix);
        if (index >= 0 && index < cDepth*cHeight*cWidth)
        {
            fThetas[index]->Fill(n21.Angle(n43));
            ++fNUsed;
        }
    }
    ++fNTot;
}

void ImagingUsingIntersection3D::CheckEvt(PCTEvent *)
{
}

void ImagingUsingIntersection3D::End()
{
    cout << "We reconstruct a 3D image using " << fNUsed << " events among " << fNTot << endl;
    fFile = new TFile(out_file_name.c_str(), "RECREATE");
    for(int d =0; d< cDepth; ++d){//z axis
        for(int h = 0; h< cHeight; ++h){//y axis
            for(int w =0; w< cWidth; ++w){//x axis
                int idx = pixel2index(d, h, w);
                fThetas[idx]->Write();

                double val = fThetas[idx]->GetMean();
                double err = fThetas[idx]->GetRMS();
                fImages[d]->SetBinContent(w, h, val);
                fImages[d]->SetBinError(w, h, err);

                fImages[d]->Write();
            }
        }
    }
    fFile->Close();
}