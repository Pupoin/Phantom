
#ifndef PctRootManager_h
#define PctRootManager_h 1

/* Root Library */
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TArrayD.h"

/* Geant4 Library */
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <utility>
#include <vector>
#include <map>

/* Phantom */
#include "DataIO/PCTEvent.h"

class PctRootManager {
public:

    PctRootManager(const PctRootManager &) = delete;

    PctRootManager &operator=(PctRootManager const &) = delete;

    static PctRootManager *Instance();

    bool open(const G4String & fileName);

    //void bookCollection(const G4String &);
    void initialize();

    void flush();
    void close();
    void write(PCTEvent* evt);

    /* Fill Methods */
    void FillGeometry(const G4String &filename);

/*
    void FillSimData(const TString& name, PCTXData* sim_data);

    void FillSimHit(const TString& name, PCTHit* sim_hit);

    void FillSimDigi(const TString& name, PCTDigi* sim_digi);

    void FillSimStep(const TString& name, const G4Step *aStep);

    void FillSimTrack(const TString& name, MCParticle* mcp, int ParentID);

    void FillTree(int eventID, const double *Rnd);
    */

private:

    static PctRootManager *pRootMng;
    /*              */
    /* Root Outputs */
    /*              */

    //G4String outfile_name;
    TFile *rootFile;
    TTree *tr;

    int RunNumber{};
    int TotalEventNumber{};
    int CurrentRunNumber{};
    int CurrentEventNumber{};

    double Rndm[4]{}; //Random double array, size=4

    PCTEvent* evt;

private:
    PctRootManager();

};


#endif