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

    evt = new PCTEvent();

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
    //tr->SetAutoSave();
    //tr->SetAutoFlush(-500000000);

    tr->Branch("RunNumber", &RunNumber, "RunNumber/I");
    tr->Branch("EventNumber", &CurrentEventNumber, "EventNumber/I");
    tr->Branch("Rndm", &Rndm, "Rndm[4]/D");
    tr->Branch("EventData", &evt, 320000000, 0);

    /* Add default collections in evt */
    evt->RegisterCollection(pControl->ParticleStep_Name, Phantom_DataType::ParticleStep);
    evt->RegisterCollection(pControl->MCParticle_Name, Phantom_DataType::MCParticle);

    cout << "==> ROOT file is opened in " << fileName << endl;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
/// \brief 
/// \param[in] cIn
void RootManager::bookCollection(const G4String &cIn) {  //run level initialize based on det name

    G4cout << "[Root Manager] ==> Booking tree for " << cIn << " ..." << G4endl;

    // for raw hit (truth)
    evt->RegisterCollection(cIn, Phantom_DataType::DetectorData);

    // for digitization
    // evt->RegisterCollection(cIn + "_Digitized", Phantom_DataType::DetectorHit);
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

void RootManager::FillSimHit(const TString &name, PCTXData *sim_hit) {
    sim_hit->setId(static_cast<int>(evt->GetData(name, DetectorData_DataType::COL)->size()));
    evt->GetData(name, DetectorData_DataType::COL)->emplace_back(sim_hit);
}

void RootManager::FillSimTrack(const TString &name, MCParticle *mcp, int ParentID) {

    auto mcps = evt->GetData(name, MCParticle_DataType::COL);
    mcp->setParent(PCTEvent::SearchID(mcps, ParentID));

    auto tmp1 = G4String(mcp->getCreateProcess());
    const char *tmp2;
    if (tmp1.contains("biasWrapper"))
        tmp2 = tmp1(tmp1.index("(") + 1, tmp1.index(")") - tmp1.index("(") - 1).data();
    else
        tmp2 = tmp1.data();
    mcp->setCreateProcess(std::string(tmp2));

    mcps->emplace_back(mcp);

}

void RootManager::FillSimStep(const TString &name, const G4Step *aStep) {
    G4StepPoint *prev = aStep->GetPreStepPoint();
    G4StepPoint *post = aStep->GetPostStepPoint();

    auto Steps = evt->GetData(name, ParticleStep_DataType::COL);
    if (Steps->empty()) {
        auto step_first = new PCTStep();
        step_first->setId(static_cast<int>(Steps->size()));
        step_first->setX(prev->GetPosition()[0]);
        step_first->setY(prev->GetPosition()[1]);
        step_first->setZ(prev->GetPosition()[2]);
        step_first->setPx(prev->GetMomentum()[0]);
        step_first->setPy(prev->GetMomentum()[1]);
        step_first->setPz(prev->GetMomentum()[2]);
        step_first->setE(prev->GetTotalEnergy());
        step_first->setDeltaE(0.);
        step_first->setPVName(prev->GetPhysicalVolume()->GetName().data());
        step_first->setProcessName("Initial Step");
        Steps->emplace_back(step_first);
    }
    auto step = new PCTStep();
    step->setId(static_cast<int>(Steps->size()));
    step->setX(post->GetPosition()[0]);
    step->setY(post->GetPosition()[1]);
    step->setZ(post->GetPosition()[2]);
    step->setPx(post->GetMomentum()[0]);
    step->setPy(post->GetMomentum()[1]);
    step->setPz(post->GetMomentum()[2]);
    step->setE(post->GetTotalEnergy());
    step->setDeltaE(aStep->GetTotalEnergyDeposit());

    if (post->GetPhysicalVolume() == nullptr) {
        step->setPVName("OutofWorld");
        step->setProcessName("Transportation");
    } else {
        step->setPVName(post->GetPhysicalVolume()->GetName().data());
        auto tmp2 = post->GetProcessDefinedStep()->GetProcessName();
        const char *tmp3;
        if (tmp2.contains("biasWrapper"))
            tmp3 = tmp2(tmp2.index("(") + 1, tmp2.index(")") - tmp2.index("(") - 1).data();
        else
            tmp3 = tmp2.data();
        step->setProcessName(std::string(tmp3));
    }

    Steps->emplace_back(step);
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
    if (pControl->memory_check) PCTEvent::PrintObjectStatistics("Waiting for Filling the tree");
#endif

    initialize();

}

