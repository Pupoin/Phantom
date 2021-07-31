#include "RootManager/RootManager.h"

#include "TGeoManager.h"

#include <stdexcept>
#include <sstream>

using std::cout, std::cerr, std::endl;

// Required by Singleton
RootManager *pRootMng = nullptr;

// Get Instance Class
RootManager *RootManager::CreateInstance() {
    if (pRootMng == nullptr)
        pRootMng = new RootManager();

    return pRootMng;
}

RootManager::RootManager()
        : rootFile(nullptr), tr(nullptr) {

    evt = new PEvent();

#ifdef MEMCK
    if (pControl->memory_check) {
        Printf("============================================================");
        Printf("-- Remeber to include the following strings in ~/.rootrc");
        Printf("Root.MemStat:            1");
        Printf("Root.MemStat.size:       1");
        Printf("Root.MemStat.cnt:        1");
        Printf("Root.ObjectStat:         1");
        Printf("============================================================");
    }
#endif

}

/// \brief Clean Optical stuff.
void RootManager::initialize() {
    evt->Initialization(CleanType::nVector);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/// \brief Create rootFile.
void RootManager::book() { //run level init for all
    G4String fileName = pControl->outfile_Name;
    rootFile = new TFile(fileName, "RECREATE");

    if (!rootFile) {
        cerr << " RootManager::book :" << " problem creating the ROOT TFile " << fileName << endl;
        return;
    }

    tr = new TTree(pControl->tree_Name, pControl->tree_Name);
    tr->SetAutoSave();

    tr->Branch("RunNumber", &RunNumber, "RunNumber/I");
    tr->Branch("EventNumber", &CurrentEventNumber, "EventNumber/I");
    tr->Branch("Rndm", &Rndm, "Rndm[4]/D");
    tr->Branch("EventData", &evt, 320000000, 0);

    cout << "==> ROOT file is opened in " << fileName << endl;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
/// \brief 
/// \param[in] cIn
void RootManager::bookCollection(const G4String &cIn) {  //run level initialize based on det name

    G4cout << "[Root Manager] ==> Booking tree for " << cIn << " ..." << G4endl;

    evt->RegisterCollection(cIn, Phantom_DataType::DetectorHit);
}

/// \brief Save ROOT file of Simulation tree.
void RootManager::save() {
    if (rootFile) {
        rootFile->WriteTObject(tr, "", "Overwrite");
        rootFile->Close();
        G4cout << "[Root Manager] ==> Simulation Tree is saved \n" << G4endl;
    }
}


void RootManager::FillGeometry(const G4String &filename) {

    auto geoM = new TGeoManager();
    TGeoManager::Import(filename);

    rootFile->cd();
    geoM->Write("DetGeoManager");

    std::remove(filename);

}

void RootManager::FillSimHit(const TString &name, PHit *sim_hit) {
    evt->GetData(name, DetectorHit_DataType::COL)->emplace_back(sim_hit);
}

void RootManager::FillTree(int eventID, const double *Rnd) {
    CurrentEventNumber = eventID;
    for (int i = 0; i < 4; i++) {
        Rndm[i] = *(Rnd + i);
    }

    evt->setEventId(CurrentEventNumber);
    evt->setRndm(Rnd);

    tr->Fill();

#ifdef MEMCK
    if (pControl->memory_check) PEvent::PrintObjectStatistics("Waiting for Filling the tree");
#endif

    initialize();

}

