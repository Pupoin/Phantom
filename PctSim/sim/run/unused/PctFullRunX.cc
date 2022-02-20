
#include "PctFullRunX.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4AutoLock.hh"
#include "g4root.hh"
#include "G4VTrajectoryPoint.hh"
#include <mutex>
#include <thread>
#include "PctRawWriter.hh"
#include "PctROOTWriter.hh"
#include "G4SystemOfUnits.hh"
#include "PctRunOpt.hh"
#include "PctTrajectory.hh"
#include "PctTrajectoryPoint.hh"
#include "PctSdVHit.hh"
#include "PctRootManager.hh"

namespace {
  G4Mutex PctRecordEventMutexFullRunX = G4MUTEX_INITIALIZER;
}

 PctFullRunX::PctFullRunX():
   G4Run(/* args */)
{
    pctevt.RegisterCollection("ini_step", Phantom_DataType::ParticleStep);
    pctevt.RegisterCollection("det_ft_hits", Phantom_DataType::DetectorHit);//front tracker
    pctevt.RegisterCollection("det_rt_hits", Phantom_DataType::DetectorHit);//rear tracker
    pctevt.RegisterCollection("det_st_hits", Phantom_DataType::DetectorHit);//scintillator
    pctevt.RegisterCollection("det_ts_hits", Phantom_DataType::DetectorHit);//telescope
    //G4cout<<"PctFullRunX::PctFullRunX() done"<<G4endl;
}

PctFullRunX::~PctFullRunX()
{
}
void PctFullRunX::Merge(const G4Run *aRun)
{
    G4Run::Merge(aRun);
}

void PctFullRunX::RecordEvent(const G4Event *evt)
{
    G4cout << "PctFullRunX::" << __FUNCTION__
           << ", run = " << this->GetRunID() << ", " << this->GetNumberOfEvent() << G4endl;
    G4Run::RecordEvent(evt);
    pctevt.setEventId(evt->GetEventID());
    pctevt.setRunId(this->GetRunID());

    G4TrajectoryContainer *trajectoryContainer = evt->GetTrajectoryContainer();
    if (trajectoryContainer)
    {
        G4int n_trajectories = trajectoryContainer->entries();
        G4int nn = n_trajectories>=1 ? 1 :0;
        for (G4int i = 0; i < nn; i++)
        {
            PctTrajectory *trj = (PctTrajectory *)((*(evt->GetTrajectoryContainer()))[i]);
            for (G4int j = 0; j < trj->GetPointEntries(); ++j)
            {
                PctTrajectoryPoint *trjp = (PctTrajectoryPoint *)trj->GetPoint(j);
                //  trj->ShowTrajectory();
                G4ThreeVector value = trjp->GetPosition();

                PCTStep *pri = new PCTStep();
                pri->setId(trj->GetPDGEncoding());
                pri->setX(value.x() / mm);
                pri->setY(value.y() / mm);
                pri->setZ(value.z() / mm);
                pri->setDeltaE(trjp->GetEnergyDeposit() / keV);
                pri->setE(trjp->GetEnergyTotal() / keV);
                pri->setProcessName(trjp->GetProcessName());
                pri->setPVName(trjp->GetPVName());
                pctevt.GetData("ini_step", ParticleStep_DataType::COL)->emplace_back(pri);
            }
        }
    }

    // hits
    G4HCofThisEvent *pHitColl = evt->GetHCofThisEvent();
    if (!pHitColl)
    {
        G4int nHitColl = pHitColl->GetNumberOfCollections();
        G4cout << "There are " << nHitColl << " hits collections." << G4endl;
        for (G4int i = 0; i < nHitColl; i++)
        {
            G4VHitsCollection *ithHitColl = pHitColl->GetHC(i);
            G4int collID = ithHitColl->GetColID();
            G4String collName = ithHitColl->GetName();
            G4String sdName = ithHitColl->GetSDname();
            size_t nHits = ithHitColl->GetSize();
             G4cout << "The " << i << " collections#" << collID
            	   << " name=" << collName
            	   << " sdname=" << sdName
            	   << " has " << nHits << " hits." << G4endl;
            std::vector<PCTHit *> *pdata(NULL);
            if (collName == "FrontTracker")
                pdata = pctevt.GetData("det_ft_hits", DetectorHit_DataType::COL);
            else if (collName == "RearTracker")
                pdata = pctevt.GetData("det_rt_hits", DetectorHit_DataType::COL);
            else if (collName == "Scintillator")
                pdata = pctevt.GetData("det_st_hits", DetectorHit_DataType::COL);
            else if (collName == "Telescope")
                pdata = pctevt.GetData("det_ts_hits", DetectorHit_DataType::COL);
            else
                pdata = NULL;
            if (!pdata)
                continue;
            for (size_t j = 0; j < nHits; ++j)
            {
                PctSdVHit *aHit = (PctSdVHit *)ithHitColl->GetHit(j);
                //  aHit->Print();
                PCTHit *pcthit = new PCTHit();
                pcthit->setX(aHit->GetX());
                pcthit->setY(aHit->GetY());
                pcthit->setZ(aHit->GetZ());
                pcthit->setT(aHit->GetTime());
                pcthit->setE(aHit->GetEnergy());
                pcthit->setId(aHit->GetID());
                pdata->emplace_back(pcthit);
            }
             G4cout << "----------------------------------------" << G4endl;
        }
    }

    G4AutoLock lock(&PctRecordEventMutexFullRunX);
    PctRootManager::Instance()->write(&pctevt);
    lock.unlock();
}
