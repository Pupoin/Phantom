//
// Created by Jifeng Hu on  19/9/21
//

#include "Analyzer/EnergyDepositionCalibrator.h"
#include "Core/GeometryHelper.h"
#include "TFile.h"
#include "TH2D.h"
#include <iomanip>
#include <iostream>
using namespace std;

EnergyDepositionCalibrator::EnergyDepositionCalibrator() 
{
    SetName("EnergyDepositionCalibrator");
    SetTips("energy deposition calibration");
    
    // RegisterParameter(variable_name, variable address, default value, description, variable_type)
    RegisterParameter("output_file", &out_file_name, std::string("calib_dEdx.root"), "output file from EnergyDepositionCalibrator", "std::string");
}

void EnergyDepositionCalibrator::Begin() {
    //x axis beta*gamma, y axis dE/dx in unit of MeV/cm
	hdEdx = new TH2D("hdEdx", "dEdx versus beta*gamma for water", 30, 0.3,0.6, 60, -0.2, 1.0);
	fNUsed = 0;
}

void EnergyDepositionCalibrator::ProcessEvt(PCTEvent *evt) 
{
    // Find PStep Information
    auto proton_steps = evt->GetData("Init_Particle_Step", ParticleStep_DataType::COL);
    int set_ini = -1;
    int set_fnl = -1;
    double iniP = -10000.;//initial momentum
    double iniX = -10000.;
    double iniY = -10000.;
    double iniE = -10000;
    double fnlE = -10000;
	for (auto step : *proton_steps) 
	{
		double zz = step->getZ();
		if( zz < -230){//world from beam position to front tracker first layer
		}else if(zz < -30){//-230, -30 front tracker
		}else if(zz < -25){//-30, -25 world air
		}else if(zz < 25.+1.E-2){//-25, 25 target
            if (set_ini < 0)
            {
                iniX = step->getX();
                iniY = step->getY();
                iniE = step->getE();
                iniP = sqrt(pow(step->getPx(), 2) + pow(step->getPy(), 2) + pow(step->getPz(), 2));
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
    //
    const double mn =  938.272013;
    if (iniE - fnlE > 2. && iniE - fnlE < 22. && fabs(iniX)> 2. && fabs(iniY)> 3.)
    { //MeV
        double beta_gamma = iniP / mn;
        hdEdx->Fill(beta_gamma, log((iniE-fnlE)/5.));
        ++fNUsed;
    }
}

void EnergyDepositionCalibrator::CheckEvt(PCTEvent *)
{
}
#include "TF1.h"
#include "TProfile.h"

void EnergyDepositionCalibrator::End()
{
    cout<<"========== EnergyDepositionCalibrator::End() ============= "<<endl;
    cout << "We collect " << fNUsed << " events to perform dE/dx calibration." << endl;
    fFile = new TFile(out_file_name.c_str(), "RECREATE");
    TProfile* hpx = hdEdx->ProfileX("hdEdx_bg");
    TF1* fmodel = new TF1("fmodel","pol2", 0.30, 0.60);
    hpx->Fit(fmodel, "RM");
    hdEdx->Write();
    hpx->Write();
    fmodel->Write();
    fFile->Close();
}
