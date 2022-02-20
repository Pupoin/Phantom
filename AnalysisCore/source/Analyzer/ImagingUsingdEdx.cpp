//
// Created by Jifeng Hu on  16/9/21
//

#include "Analyzer/ImagingUsingdEdx.h"
#include "Core/GeometryHelper.h"
#include "TFile.h"
#include "TH2D.h"
#include <iomanip>
#include <iostream>
using namespace std;

ImagingUsingdEdx::ImagingUsingdEdx() 
{
    SetName("ImagingUsingdEdx");
    SetTips("imaging the target with energy deposition");
    
    // RegisterParameter(variable_name, variable address, default value, description, variable_type)
    RegisterParameter("output_file", &out_file_name, std::string("image_2d_dedx.root"), "output file from imagingUsingdEdX", "std::string");
}

void ImagingUsingdEdx::Begin() {
	himg = new TH2D("himg2d_dedx", "2D slice in x-y plane", 60, -15,15, 80, -20, 20);
	fNUsed = 0;
}

void ImagingUsingdEdx::ProcessEvt(PCTEvent *evt) 
{
    // Find PStep Information
    auto proton_steps = evt->GetData("Init_Particle_Step", ParticleStep_DataType::COL);
    int set_ini = -1;
    int set_fnl = -1;
    double x1 = -100000.;
    double y1 = -100000.;
    //double z1 = -100000;
    //double x2 = -100000;
    //double y2 = -100000;
    //double z2 = -100000;
    double iniE = -100000.;
    double fnlE = -100000.;
	for (auto step : *proton_steps) 
	{
		double zz = step->getZ();
		if( zz < -230){//world from beam position to front tracker first layer
		}else if(zz < -30){//-230, -30 front tracker
		}else if(zz < -25){//-30, -25 world air
		}else if(zz < 25.+1.E-2){//-25, 25 target
            if(set_ini < 0){
                x1 = step->getX();
                y1 = step->getY();
                //z1 = step->getZ();
                iniE = step->getE();
                set_ini = 1;
            }
		}else if(zz < 30.){//25, 30 world air
		}else if(zz < 230){//30, 230 rear trakcer
            if(set_fnl < 0){
                //x2 = step->getX();
                //y2 = step->getY();
                //z2 = step->getZ();
                fnlE = step->getE();
                set_fnl = 1;
            }
		}else if(zz < 280){//rear tracker to world air
		}else if(zz < 285){//scintillator
		}else if(zz < 340){//scintillator to air
		}else{//scintillator > 340
		}
	}
    //
    if (iniE - fnlE > 0.05 && iniE - fnlE < 20.) {//MeV
        himg->Fill(x1, y1, (iniE - fnlE));
        ++fNUsed;
    }
}

void ImagingUsingdEdx::CheckEvt(PCTEvent *)
{
}

void ImagingUsingdEdx::End()
{
    cout<<"========== ImagingUsingdEdx::End() ============= "<<endl;
    cout<<"We reconstruct a 2D image using "<< fNUsed <<" events"<<endl;
    fFile = new TFile(out_file_name.c_str(),"RECREATE");
    himg->Write();
    fFile->Close();
}
