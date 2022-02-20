
#include "Analyzer/ImagingUsingIntersection2D.h"
#include "Core/GeometryHelper.h"
#include "TFile.h"
#include "TH2D.h"
#include "TVector3.h"
#include <iomanip>
#include <iostream>
#include "TString.h"
using namespace std;

ImagingUsingIntersection2D::ImagingUsingIntersection2D() 
{
    SetName("ImagingUsingIntersection2D");
    SetTips("imaging the target with scattering points");
    
    RegisterParameter("output_file", &out_file_name, std::string("image_2d_intersection.root"), "output file from ImagingUsingIntersection2D", "std::string");
}
void ImagingUsingIntersection2D::Begin() {
    //compute the width X height X depth;
    //the direction along the proton motion is defined as z+, i.e., depth,
    //the direction perpendicular to the proton direction is x-y plane, horizontal x, vertical y
    fImage = new TH2D("himg2d_angle", "image using the scattering angle", cWidth, xlower, xupper, cHeight, ylower, yupper);
    //using 2D image
    fThetaDistributions.resize( cWidth*cHeight );
    int ix;
    int iy;
    for (size_t i = 0; i < fThetaDistributions.size(); ++i)
    {
        index2pixel(i, ix, iy);
        TString _histoName = TString::Format("hslice_h%03d_w%03d", ix, iy);
        TString _histoTitle = TString::Format("2D slice in x-y plane, h=%03d, w=%03d", ix, iy);
        fThetaDistributions[i] = new TH1D(_histoName, _histoTitle, 40, 0, 0.5*TMath::Pi());
    }
    fNUsed = 0;
    fNTot = 0;
}

void ImagingUsingIntersection2D::ProcessEvt(PCTEvent *evt)
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
    //computing the intersection2D point and fill in histograms
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
        TVector3 ip = 0.5*(ip1 + ip2);
        int ix = (ip.X() - xlower)/width_unit;
        int iy = (ip.Y() - ylower)/height_unit;
        if(ix>= 0 && ix < cWidth && iy>= 0 && iy< cHeight){
            int index = pixel2index(iy,ix);
            fThetaDistributions[index]->Fill(n21.Angle(n43));
            ++fNUsed;
        }
    }
    ++fNTot;
}

void ImagingUsingIntersection2D::CheckEvt(PCTEvent *)
{
}

void ImagingUsingIntersection2D::End()
{
	cout << "========== ImagingUsingIntersection2D::End() ============= " << endl;
	cout << "We reconstruct a 2D image using " << fNUsed << " events among "<<fNTot << endl;
	fFile = new TFile(out_file_name.c_str(), "RECREATE");
	int ix;
	int iy;
	for (size_t i = 0; i < fThetaDistributions.size(); ++i)
	{
		fThetaDistributions[i]->Write();

		index2pixel(i, iy, ix);
		double val = fThetaDistributions[i]->GetMean();
		double err = fThetaDistributions[i]->GetRMS();
		fImage->SetBinContent(ix, iy, val);
		fImage->SetBinError(ix, iy, err);
	}
	fImage->Write();
	fFile->Close();
}
