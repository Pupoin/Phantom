//
// Created by Zhang Yulei on 8/25/21.
//

#include "Analyzer/PStepDump.h"
#include "Core/GeometryHelper.h"
#include "TFile.h"
#include "TTree.h"
#include <iomanip>
#include <iostream>
using namespace std;

PStepDump::PStepDump() 
{
    SetName("PStepDump");
    SetTips("Format data to output ROOT file");
    
    RegisterParameter("output_file", &out_file_name, std::string("truth.root"), "output file from formatter", "std::string");
}

void PStepDump::Begin() {
	fFile = new TFile(out_file_name.c_str(), "RECREATE");//RECREATE
	dump = new TTree("tru", "truth data");
	dump->Branch("iniPx", &fDataVec[0][0], "iniPx/D");
	dump->Branch("iniPy", &fDataVec[0][1], "iniPy/D");
	dump->Branch("iniPz", &fDataVec[0][2], "iniPz/D");
	dump->Branch("iniE0", &fDataVec[0][3], "iniE0/D");
	dump->Branch("inidE", &fDataVec[0][4], "inidE/D");
	dump->Branch("inix", &fDataVec[0][5], "inix/D");
	dump->Branch("iniy", &fDataVec[0][6], "iniy/D");
	dump->Branch("iniz", &fDataVec[0][7], "iniz/D");
	dump->Branch("init", &fDataVec[0][8], "init/D");
	dump->Branch("inip", &fDataVec[0][9], "inip/D");
	dump->Branch("inibeta", &fDataVec[0][10], "inibeta/D");
	dump->Branch("inibg", &fDataVec[0][11], "inibg/D");

	dump->Branch("wfPx", &fDataVec[1][0], "wfPx/D");
	dump->Branch("wfPy", &fDataVec[1][1], "wfPy/D");
	dump->Branch("wfPz", &fDataVec[1][2], "wfPz/D");
	dump->Branch("wfE0", &fDataVec[1][3], "wfE0/D");
	dump->Branch("wfdE", &fDataVec[1][4], "wfdE/D");
	dump->Branch("wfx", &fDataVec[1][5], "wfx/D");
	dump->Branch("wfy", &fDataVec[1][6], "wfy/D");
	dump->Branch("wfz", &fDataVec[1][7], "wfz/D");
	dump->Branch("wft", &fDataVec[1][8], "wft/D");
	dump->Branch("wfp", &fDataVec[1][9], "wfp/D");
	dump->Branch("wfbeta", &fDataVec[1][10], "wfbeta/D");
	dump->Branch("wfbg", &fDataVec[1][11], "wfbg/D");

	dump->Branch("fwPx", &fDataVec[2][0], "fwPx/D");
	dump->Branch("fwPy", &fDataVec[2][1], "fwPy/D");
	dump->Branch("fwPz", &fDataVec[2][2], "fwPz/D");
	dump->Branch("fwE0", &fDataVec[2][3], "fwE0/D");
	dump->Branch("fwdE", &fDataVec[2][4], "fwdE/D");
	dump->Branch("fwx", &fDataVec[2][5], "fwx/D");
	dump->Branch("fwy", &fDataVec[2][6], "fwy/D");
	dump->Branch("fwz", &fDataVec[2][7], "fwz/D");
	dump->Branch("fwt", &fDataVec[2][8], "fwt/D");
	dump->Branch("fwp", &fDataVec[2][9], "fwp/D");
	dump->Branch("fwbeta", &fDataVec[2][10], "fwbeta/D");
	dump->Branch("fwbg", &fDataVec[2][11], "fwbg/D");

	dump->Branch("wtgPx", &fDataVec[3][0], "wtgPx/D");
	dump->Branch("wtgPy", &fDataVec[3][1], "wtgPy/D");
	dump->Branch("wtgPz", &fDataVec[3][2], "wtgPz/D");
	dump->Branch("wtgE0", &fDataVec[3][3], "wtgE0/D");
	dump->Branch("wtgdE", &fDataVec[3][4], "wtgdE/D");
	dump->Branch("wtgx", &fDataVec[3][5], "wtgx/D");
	dump->Branch("wtgy", &fDataVec[3][6], "wtgy/D");
	dump->Branch("wtgz", &fDataVec[3][7], "wtgz/D");
	dump->Branch("wtgt", &fDataVec[3][8], "wtgt/D");
	dump->Branch("wtgp", &fDataVec[3][9], "wtgp/D");
	dump->Branch("wtgbeta", &fDataVec[3][10], "wtgbeta/D");
	dump->Branch("wtgbg", &fDataVec[3][11], "wtgbg/D");

	dump->Branch("tgwPx", &fDataVec[4][0], "tgwPx/D");
	dump->Branch("tgwPy", &fDataVec[4][1], "tgwPy/D");
	dump->Branch("tgwPz", &fDataVec[4][2], "tgwPz/D");
	dump->Branch("tgwE0", &fDataVec[4][3], "tgwE0/D");
	dump->Branch("tgwdE", &fDataVec[4][4], "tgwdE/D");
	dump->Branch("tgwx", &fDataVec[4][5], "tgwx/D");
	dump->Branch("tgwy", &fDataVec[4][6], "tgwy/D");
	dump->Branch("tgwz", &fDataVec[4][7], "tgwz/D");
	dump->Branch("tgwt", &fDataVec[4][8], "tgwt/D");
	dump->Branch("tgwp", &fDataVec[4][9], "tgwp/D");
	dump->Branch("tgwbeta", &fDataVec[4][10], "tgwbeta/D");
	dump->Branch("tgwbg", &fDataVec[4][11], "tgwbg/D");

	dump->Branch("wrPx", &fDataVec[5][0], "wrPx/D");
	dump->Branch("wrPy", &fDataVec[5][1], "wrPy/D");
	dump->Branch("wrPz", &fDataVec[5][2], "wrPz/D");
	dump->Branch("wrE0", &fDataVec[5][3], "wrE0/D");
	dump->Branch("wrdE", &fDataVec[5][4], "wrdE/D");
	dump->Branch("wrx", &fDataVec[5][5], "wrx/D");
	dump->Branch("wry", &fDataVec[5][6], "wry/D");
	dump->Branch("wrz", &fDataVec[5][7], "wrz/D");
	dump->Branch("wrt", &fDataVec[5][8], "wrt/D");
	dump->Branch("wrp", &fDataVec[5][9], "wrp/D");
	dump->Branch("wrbeta", &fDataVec[5][10], "wrbeta/D");
	dump->Branch("wrbg", &fDataVec[5][11], "wrbg/D");

	dump->Branch("rwPx", &fDataVec[6][0], "rwPx/D");
	dump->Branch("rwPy", &fDataVec[6][1], "rwPy/D");
	dump->Branch("rwPz", &fDataVec[6][2], "rwPz/D");
	dump->Branch("rwE0", &fDataVec[6][3], "rwE0/D");
	dump->Branch("rwdE", &fDataVec[6][4], "rwdE/D");
	dump->Branch("rwx", &fDataVec[6][5], "rwx/D");
	dump->Branch("rwy", &fDataVec[6][6], "rwy/D");
	dump->Branch("rwz", &fDataVec[6][7], "rwz/D");
	dump->Branch("rwt", &fDataVec[6][8], "rwt/D");
	dump->Branch("rwp", &fDataVec[6][9], "rwp/D");
	dump->Branch("rwbeta", &fDataVec[6][10], "rwbeta/D");
	dump->Branch("rwbg", &fDataVec[6][11], "rwbg/D");

	dump->Branch("wsPx", &fDataVec[7][0], "wsPx/D");
	dump->Branch("wsPy", &fDataVec[7][1], "wsPy/D");
	dump->Branch("wsPz", &fDataVec[7][2], "wsPz/D");
	dump->Branch("wsE0", &fDataVec[7][3], "wsE0/D");
	dump->Branch("wsdE", &fDataVec[7][4], "wsdE/D");
	dump->Branch("wsx", &fDataVec[7][5], "wsx/D");
	dump->Branch("wsy", &fDataVec[7][6], "wsy/D");
	dump->Branch("wsz", &fDataVec[7][7], "wsz/D");
	dump->Branch("wst", &fDataVec[7][8], "wst/D");
	dump->Branch("wsp", &fDataVec[7][9], "wsp/D");
	dump->Branch("wsbeta", &fDataVec[7][10], "wsbeta/D");
	dump->Branch("wsbg", &fDataVec[7][11], "wsbg/D");

	dump->Branch("swPx", &fDataVec[8][0], "swPx/D");
	dump->Branch("swPy", &fDataVec[8][1], "swPy/D");
	dump->Branch("swPz", &fDataVec[8][2], "swPz/D");
	dump->Branch("swE0", &fDataVec[8][3], "swE0/D");
	dump->Branch("swdE", &fDataVec[8][4], "swdE/D");
	dump->Branch("swx", &fDataVec[8][5], "swx/D");
	dump->Branch("swy", &fDataVec[8][6], "swy/D");
	dump->Branch("swz", &fDataVec[8][7], "swz/D");
	dump->Branch("swt", &fDataVec[8][8], "swt/D");
	dump->Branch("swp", &fDataVec[8][9], "swp/D");
	dump->Branch("swbeta", &fDataVec[8][10], "swbeta/D");
	dump->Branch("swbg", &fDataVec[8][11], "swbg/D");

	dump->Branch("wtsPx", &fDataVec[9][0], "wtsPx/D");
	dump->Branch("wtsPy", &fDataVec[9][1], "wtsPy/D");
	dump->Branch("wtsPz", &fDataVec[9][2], "wtsPz/D");
	dump->Branch("wtsE0", &fDataVec[9][3], "wtsE0/D");
	dump->Branch("wtsdE", &fDataVec[9][4], "wtsdE/D");
	dump->Branch("wtsx", &fDataVec[9][5], "wtsx/D");
	dump->Branch("wtsy", &fDataVec[9][6], "wtsy/D");
	dump->Branch("wtsz", &fDataVec[9][7], "wtsz/D");
	dump->Branch("wtst", &fDataVec[9][8], "wtst/D");
	dump->Branch("wtsp", &fDataVec[9][9], "wtsp/D");
	dump->Branch("wtsbeta", &fDataVec[9][10], "wtsbeta/D");
	dump->Branch("wtsbg", &fDataVec[9][11], "wtsbg/D");

	dump->Branch("step", &fNstep, "step/I");
	dump->Branch("evt", &fEvtNo, "evt/I");

	//counters
	dump->Branch("wwcnt", &fDataCnt[0], "wwcnt/I");
	dump->Branch("wfcnt", &fDataCnt[1], "wfcnt/I");
	dump->Branch("fwcnt", &fDataCnt[2], "fwcnt/I");
	dump->Branch("wtgcnt",&fDataCnt[3], "wtgcnt/I");
	dump->Branch("tgwcnt",&fDataCnt[4], "tgwcnt/I");
	dump->Branch("wrcnt", &fDataCnt[5], "wrcnt/I");
	dump->Branch("rwcnt", &fDataCnt[6], "rwcnt/I");
	dump->Branch("wscnt", &fDataCnt[7], "wscnt/I");
	dump->Branch("swcnt", &fDataCnt[8], "swcnt/I");
	dump->Branch("wtscnt",&fDataCnt[6], "wtscnt/I");

	//distances
	dump->Branch("depth",&fDepth,"depth/D");
	dump->Branch("w2end",   &fdis[0], "w2end/D");
	dump->Branch("wf2end",  &fdis[1], "wf2end/D");
	dump->Branch("fw2end",  &fdis[2], "fw2end/D");
	dump->Branch("wtg2end", &fdis[3], "wtg2end/D");
	dump->Branch("tgw2end", &fdis[4], "tgw2end/D");
	dump->Branch("wr2end",  &fdis[5], "wr2end/D");
	dump->Branch("rw2end",  &fdis[6], "rw2end/D");
	dump->Branch("ws2end",  &fdis[7], "ws2end/D");
	dump->Branch("sw2end",  &fdis[8], "sw2end/D");
	dump->Branch("wts2end", &fdis[9], "wts2end/D");

	//scattering angle
	dump->Branch("thetaIF", &fThetaIF, "thetaIF/D");
	dump->Branch("thetaTF", &fThetaTF, "thetaTF/D");


	fEvtNo = 0;
}

void PStepDump::ProcessEvt(PCTEvent *evt) 
{
	for(int i=0;i<10;++i){
		for(int j=0;j<9;++j)
		  fDataVec[i][j] = 0.;
		fDataCnt[i] = 0;
	}
	fNstep = 0;
    //(x1,y1,z1,t1,dE1) (x2, y2,z3,t2,dE2) .... (xn,yn,zn,tn,dEn) (px py pz, Px,Py,Pz, depth, Z)
    double px = 0, py = 0, pz = 0, Px = 0, Py = 0, Pz = 0, depth = 0;

    auto Contain = [](const std::string &str, const std::string &tar) {
        return str.find(tar) != std::string::npos;
    };
    // Find PStep Information
    auto proton_steps = evt->GetData("Init_Particle_Step", ParticleStep_DataType::COL);
    PCTStep *prev_step = nullptr;
    TVector3 depth_in, depth_out;
	//tracing variations along the proton steps
	
	//cout<<setiosflags(ios::fixed)<<setprecision(6);
	//cout<<setw(10)<<"step"
	//	<<setw(25)<<"Volume"
	//	<<setw(20)<<"Process"
	//	<<setw(20)<<"Energy"
	//	<<setw(20)<<"dE"
	//	<<endl;

	const double mn =  938.272013;
	int istep=0;
	int loc = 0;
	std::string tar_name = "Target";
	for (auto step : *proton_steps) 
	{
		//cout<<"istep="<<istep<<", "<<step->getPVName()<<", "<<step->getE()<<", "<<step->getDeltaE()<<endl;

		double zz = step->getZ();
		if( zz < -230){//world from beam position to front tracker first layer
			loc = 0;
		}else if(zz < -30){//-230, -30 front tracker
			loc =1;
		}else if(zz < -25){//-30, -25 world air
			loc =2;
		}else if(zz < 25.+1.E-2){//-25, 25 target
			loc =3;
		}else if(zz < 30.){//25, 30 world air
			loc =4;
		}else if(zz < 230){//30, 230 rear trakcer
			loc =5;
		}else if(zz < 280){//rear tracker to world air
			loc =6;
		}else if(zz < 285){//scintillator
			loc =7;
		}else if(zz < 340){//scintillator to air
			loc =8;
		}else{//scintillator > 340
			loc =9;
		}

		//cout<<setw(10)<<istep
		//	<<setw(25)<<step->getPVName()
		//	<<setw(20)<<step->getProcessName()
		//	<<setw(20)<<sqrt( pow(step->getPx(), 2) + pow(step->getPy(), 2) + pow(step->getPz(),2) + pow(mn, 2) )
		//	<<setw(20)<<step->getDeltaE()
		//    <<setw(4)<<loc
		//    <<setw(6)<<fEvtNo
		//	<<endl;
		fDataVec[loc][4] += step->getDeltaE();
		fDataCnt[loc] ++;

		if(fDataVec[loc][8] < 1.){
			fDataVec[loc][0] = step->getPx();
			fDataVec[loc][1] = step->getPy();
			fDataVec[loc][2] = step->getPz();
			fDataVec[loc][3] = sqrt( pow(step->getPx(), 2) + pow(step->getPy(), 2) + pow(step->getPz(),2) + pow(mn, 2) );
			fDataVec[loc][5] = step->getX();
			fDataVec[loc][6] = step->getY();
			fDataVec[loc][7] = step->getZ();
			fDataVec[loc][8] = 2.;
			fDataVec[loc][9] = sqrt( pow(step->getPx(), 2) + pow(step->getPy(), 2) + pow(step->getPz(),2) );
			fDataVec[loc][10] = fDataVec[loc][9]/fDataVec[loc][3];//p/E
			fDataVec[loc][11] = fDataVec[loc][9]/mn;
		}



		//if (prev_step) {
		//	// Record the momentum of proton while entering the target
		//	if (Contain(step->getPVName(), tar_name) && Contain(prev_step->getPVName(), "World")) {
		//		depth_in.SetXYZ(step->getX(), step->getY(), step->getZ());
		//		px = step->getPx();
		//		py = step->getPy();
		//		pz = step->getPz();
		//	}
		//	if (Contain(step->getPVName(), "World") && Contain(prev_step->getPVName(), tar_name)) {
		//		depth_out.SetXYZ(prev_step->getX(), prev_step->getY(), prev_step->getZ());
		//		Px = prev_step->getPx();
		//		Py = prev_step->getPy();
		//		Pz = prev_step->getPz();
		//	}
		//}
		prev_step = step;
		++ istep;
	}
	TVector3 endpoint(prev_step->getX(), prev_step->getY(), prev_step->getZ());
	for(int i=0;i<10;++i){
		TVector3 startpoint(fDataVec[i][5], fDataVec[i][6], fDataVec[i][7]);
		fdis[i] = (startpoint-endpoint).Mag();
	}
	fThetaIF = fThetaTF = -10.;
	TVector3 initialDirection(fDataVec[0][0], fDataVec[0][1], fDataVec[0][2]);
	TVector3 wTargetDirection(fDataVec[3][0], fDataVec[3][1], fDataVec[3][2]);
	TVector3 finalDirection(fDataVec[5][0], fDataVec[5][1], fDataVec[5][2]);
	if (wTargetDirection.Mag() > 0.1)
		fThetaIF = finalDirection.Angle(initialDirection);
	if (finalDirection.Mag() > 0.1)
		fThetaTF = finalDirection.Angle(wTargetDirection);

	fDepth = (depth_out - depth_in).z();
	fNstep = istep;
	dump->Fill();

	//cout<<endl;
	++fEvtNo;
}

void PStepDump::CheckEvt(PCTEvent *)
{
}

void PStepDump::End()
{
	std::cout << Form("File: %s has been save to disk.", out_file_name.c_str()) << std::endl;
	fFile->cd();
	dump->Write();
}
