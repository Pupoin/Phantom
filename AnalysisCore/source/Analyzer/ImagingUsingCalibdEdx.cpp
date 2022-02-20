//
// Created by Jifeng Hu on  20/9/21
//

#include "Analyzer/ImagingUsingCalibdEdx.h"
#include "Core/GeometryHelper.h"
#include "TFile.h"
#include "TH2D.h"
#include "TF1.h"
#include <iomanip>
#include <iostream>
using namespace std;

ImagingUsingCalibdEdx::ImagingUsingCalibdEdx() 
{
	SetName("ImagingUsingCalibdEdx");
	SetTips("imaging the target with energy deposition");

	// RegisterParameter(variable_name, variable address, default value, description, variable_type)
	RegisterParameter("output_file", &out_file_name, std::string("image_2d_calib_dedx.root"), "output file from imagingUsingCalibdEdX", "std::string");
}

void ImagingUsingCalibdEdx::Begin() {
	//parameters obtained with water : 5cm thick
	//1  p0           3.21130e+00   2.38303e-02   7.65635e-07  -3.80498e-07
	//2  p1          -9.28925e+00   1.05624e-01   2.21473e-06  -2.88102e-06
	//3  p2           6.30460e+00   1.15396e-01   1.72702e-06  -3.11987e-07
	fmodel = new TF1("fCalibModel", "pol2", 0.32, 0.6);
	fmodel->SetParameter(0, 3.21130e+00);fmodel->SetParError(0, 2.38303e-02);
	fmodel->SetParameter(1,-9.28925e+00);fmodel->SetParError(1, 1.05624e-01);
	fmodel->SetParameter(2, 6.30460e+00);fmodel->SetParError(2, 1.15396e-01);

	himg = new TH2D("himg2d_calib_dedx", "2D slice in x-y plane", 60, -15,15, 80, -20, 20);
	hdEdx = new TH1D("hdEdx_calib", "energy deposition spetrum", 200, 1, 21);
	fNUsed = 0;
}

void ImagingUsingCalibdEdx::ProcessEvt(PCTEvent *evt) 
{
	// Find PStep Information
	auto proton_steps = evt->GetData("Init_Particle_Step", ParticleStep_DataType::COL);
	int set_ini = -1;
	int set_fnl = -1;
	double x1 = -100000.;
	double y1 = -100000.;
	double iniE = -100000.;
	double fnlE = -100000.;
	double iniP = -100000.;
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
				iniP = sqrt(pow(step->getPx(), 2)+ pow(step->getPy(),2)+pow(step->getPz(),2));
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
	const double Mp =  938.272013;
	if (iniE - fnlE > 1.0 && iniE - fnlE < 22.) {//MeV
		double weighted_de = (iniE - fnlE) *exp( - fmodel->Eval( iniP/Mp ) );
		himg->Fill(x1, y1, weighted_de);
		hdEdx->Fill( weighted_de ) ;
		++fNUsed;
	}
}

void ImagingUsingCalibdEdx::CheckEvt(PCTEvent *)
{
}

void ImagingUsingCalibdEdx::End()
{
	cout << "========== ImagingUsingCalibdEdx::End() ============= " << endl;
	cout << "We reconstruct a 2D image using " << fNUsed << " events" << endl;
	fFile = new TFile(out_file_name.c_str(), "RECREATE");
	himg->Write();
	hdEdx->Write();
	fFile->Close();
}
