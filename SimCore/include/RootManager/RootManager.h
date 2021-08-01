#ifndef RootManager_h
#define RootManager_h 1

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
#include "Control/Control.h"
#include "DataIO/PEvent.h"

class RootManager {
public:

    RootManager(const RootManager &) = delete;

    RootManager &operator=(RootManager const &) = delete;

    static RootManager *CreateInstance();

    void book();

    void bookCollection(const G4String &);

    void save();

    void initialize();

    /* Fill Methods */
    void FillGeometry(const G4String &filename);

    void FillSimHit(const TString& name, PHit* sim_hit);

    void FillSimStep(const TString& name, const G4Step *aStep);

    void FillSimTrack(const TString& name, MCParticle* mcp, int ParentID);

    void FillTree(int eventID, const double *Rnd);

private:

    /*              */
    /* Root Outputs */
    /*              */

    G4String outfile_name;
    TFile *rootFile;
    TTree *tr;

    int RunNumber{};
    int TotalEventNumber{};
    int CurrentEventNumber{};

    double Rndm[4]{}; //Random double array, size=4

    PEvent* evt{};

private:
    RootManager();

};

extern RootManager *pRootMng;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
