//
// Created by Jifeng Hu on 16/9/21, hujf@scnu.edu.cn
//

#ifndef PCTSIMULATION_PSTEPDUMP_H
#define PCTSIMULATION_PSTEPDUMP_H

#include "Analyzer/PCTAnalyzer.h"

#include <iostream>
#include <fstream>
#include <map>
#include "TTree.h"
#include "TFile.h"

class PStepDump : public PCTAnalyzer {
public:
    PStepDump();

    // Override virtual function of base class
    void Begin() override;

    void ProcessEvt(PCTEvent *event) override;

    void CheckEvt(PCTEvent *event) override;

    void End() override;

private:
    std::string out_file_name;

	double fDataVec[10][12];//idx=0,10, 5:px,py,pz,E,dE,p,beta
	int fDataCnt[10];//idx=0,10, 4:px,py,pz,dE

	TFile* fFile;
	TTree* dump;

	Int_t fEvtNo;
	Int_t fNstep;

	Double_t fdis[10];
	Double_t fDepth;

	Double_t fThetaIF;//initial direction and final direction
	Double_t fThetaTF;//the direction entering into target and final direction
};


#endif //PCTSIMULATION_PSTEPDUMP_H
