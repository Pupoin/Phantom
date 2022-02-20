#include "PctRootManager.hh"
#include "TGeoManager.h"
#include "PctRunOpt.hh"
#include <stdexcept>
#include <sstream>

using namespace std;

// Required by Singleton
PctRootManager *PctRootManager::pRootMng = nullptr;

// Get Instance Class
PctRootManager *PctRootManager::Instance() {
    if (PctRootManager::pRootMng == nullptr)
        pRootMng = new PctRootManager();

    return pRootMng;
}

PctRootManager::PctRootManager()
        : rootFile(nullptr), tr(nullptr) {
    evt = new PCTEvent();
#ifdef MEMCK
        Printf("============================================================");
        Printf("-- Remeber to include the following strings in ~/.rootrc");
        Printf("Root.MemStat:            1");
        Printf("Root.MemStat.size:       1");
        Printf("Root.MemStat.cnt:        1");
        Printf("Root.ObjectStat:         1");
        Printf("============================================================");
#endif
}

/// \brief Clean Optical stuff.
void PctRootManager::initialize() {
    evt->Initialization(CleanType::nVector);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/// \brief Create rootFile.
bool PctRootManager::open(const G4String& fileName) { //run level init for all
    rootFile = new TFile(fileName, "RECREATE");
    if (!rootFile) {
        cerr << " PctRootManager::book :" << " problem creating the ROOT TFile " << fileName << endl;
        return false;
    }
    tr = new TTree("evt", "events");
    if(!tr){
        cerr << " PctRootManager::book :" << " problem creating the tree." << endl;
        return false;
    }
    //tr->SetAutoSave();
    //tr->SetAutoFlush(-500000000);
//Int_t Branch(TCollection* list, Int_t bufsize = 32000, Int_t splitlevel = 99, const char* name = "")
//Int_t Branch(TList* list, Int_t bufsize = 32000, Int_t splitlevel = 99)
//Int_t Branch(const char* folder, Int_t bufsize = 32000, Int_t splitlevel = 99)
//TBranch* Branch(const char* name, Long_t address, const char* leaflist, Int_t bufsize = 32000)
//TBranch* Branch(const char* name, char* address, const char* leaflist, Int_t bufsize = 32000)
//TBranch* Branch(const char* name, const char* classname, void* addobj, Int_t bufsize = 32000, Int_t splitlevel = 99)
//TBranch* Branch(const char* name, int address, const char* leaflist, Int_t bufsize = 32000)
//TBranch* Branch(const char* name, void* address, const char* leaflist, Int_t bufsize = 32000)


    tr->Branch("RunNumber", &RunNumber, "RunNumber/I");
    tr->Branch("EventNumber", &CurrentEventNumber, "EventNumber/I");
    tr->Branch("Rndm", &Rndm, "Rndm[4]/D");
    tr->Branch("EventData",&evt, 320000000, 0);
    /* Add default collections in evt */
    //evt->RegisterCollection(pControl->ParticleStep_Name, Phantom_DataType::ParticleStep);
    //evt->RegisterCollection(pControl->MCParticle_Name, Phantom_DataType::MCParticle);

    cout << "==> ROOT file is opened in " << fileName << endl;
    return true;
}


/*
void PctRootManager::bookCollection(const G4String &cIn) {  //run level initialize based on det name

    G4cout << "[Root Manager] ==> Booking tree for " << cIn << " ..." << G4endl;

    // for raw hit (truth)
    evt->RegisterCollection(cIn, Phantom_DataType::DetectorHit);

    // for digitization
    // evt->RegisterCollection(cIn + "_Digitized", Phantom_DataType::DetectorHit);
}*/

/// \brief Save ROOT file of Simulation tree.
void PctRootManager::flush() {
    if (rootFile) {
        rootFile->WriteTObject(tr, "", "Overwrite");
        rootFile->Close();
        G4cout << "[Root Manager] ==> Simulation Tree is saved \n" << G4endl;
    }
}

void PctRootManager::FillGeometry(const G4String &filename) {

    auto geoM = new TGeoManager();
    TGeoManager::Import(filename);

    rootFile->cd();
    geoM->Write("DetGeoManager");

    std::remove(filename);

}
/*
void PctRootManager::FillSimData(const TString &name, PCTXData *sim_data) {
    sim_data->setId(static_cast<int>(evt->GetData(name, DetectorHit_DataType::COL)->size()));
    evt->GetData(name, DetectorData_DataType::COL)->emplace_back(sim_data);
}

void PctRootManager::FillSimHit(const TString &name, PCTHit*sim_hit) {
    sim_hit->setId(static_cast<int>(evt->GetData(name, DetectorHit_DataType::COL)->size()));
    evt->GetData(name, DetectorHit_DataType::COL)->emplace_back(sim_hit);
}

void PctRootManager::FillSimDigi(const TString &name, PCTDigi* sim_digi) {
    sim_digi->setId(static_cast<int>(evt->GetData(name, DetectorHit_DataType::COL)->size()));
    evt->GetData(name, DetectorDigi_DataType::COL)->emplace_back(sim_digi);
}

void PctRootManager::FillSimTrack(const TString &name, MCParticle *mcp, int ParentID) {

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

void PctRootManager::FillSimStep(const TString &name, const G4Step *aStep) {
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
void PctRootManager::FillTree(int eventID, const double *Rnd) {
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

}*/

void PctRootManager::write(PCTEvent *pevt)
{
    CurrentRunNumber = pevt->getRunId();
    CurrentEventNumber = pevt->getEventId();
    //for (int i = 0; i < 4; i++)
    //{
    //    Rndm[i] = *(Rnd + i);
    //}
    PCTEvent* temp = evt;
    evt = pevt;
    //evt->setRndm(Rnd);
    rootFile->cd();
    tr->Fill();
#ifdef MEMCK
        PCTEvent::PrintObjectStatistics("Waiting for Filling the tree");
#endif
    evt->Initialization(CleanType::nVector);
    evt = temp;
}