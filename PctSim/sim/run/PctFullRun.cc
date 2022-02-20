#include "PctFullRun.hh"
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

namespace {
  G4Mutex PctRecordEventMutexFullRun = G4MUTEX_INITIALIZER;
}

 PctFullRun::PctFullRun():
   G4Run(/* args */)
{
    databuff.reserve(10000);
}

PctFullRun::~PctFullRun()
{
    //databuff.clear();
}
void PctFullRun::Merge(const G4Run *aRun)
{
    G4Run::Merge(aRun);
}

void PctFullRun::RecordEvent(const G4Event *evt)
{
    //G4cout << "PctFullRun::" << __FUNCTION__
    //       << ", run = " << this->GetRunID() << ", " << this->GetNumberOfEvent() << G4endl;
    G4Run::RecordEvent(evt);

    databuff.clear();

    G4TrajectoryContainer *trajectoryContainer = evt->GetTrajectoryContainer();
    G4int n_trajectories = 0;
    if (trajectoryContainer)
        n_trajectories = trajectoryContainer->entries();

    G4int run_evt = this->GetRunID() << 20;
    run_evt |= evt->GetEventID(); // encode run_evt together
    databuff.push_back(run_evt);

    // particles
    databuff.push_back(n_trajectories);
    for (G4int i = 0; i < n_trajectories; i++)
    {
        PctTrajectory *trj = (PctTrajectory *)((*(evt->GetTrajectoryContainer()))[i]);
        databuff.push_back(trj->GetPDGEncoding());
        databuff.push_back(trj->GetTrackID());
        databuff.push_back(trj->GetParentID());

        databuff.push_back(trj->GetPointEntries());
        for (G4int j = 0; j < trj->GetPointEntries(); ++j)
        {
            PctTrajectoryPoint *trjp = (PctTrajectoryPoint *)trj->GetPoint(j);
            G4ThreeVector value = trjp->GetPosition();
            databuff.push_back(G4int(100 * value.x() / mm));
            databuff.push_back(G4int(100 * value.y() / mm));
            databuff.push_back(G4int(100 * value.z() / mm));
            databuff.push_back(G4int(100 * trjp->GetEnergyDeposit() / keV));
            // databuff.push_back(G4int(100 * trjp->GetEnergyTotal()/keV));
            // databuff.push_back( trjp->GetStatus());
            databuff.push_back(trjp->GetMatertialIndex());
            databuff.push_back(trjp->GetProcessType());
            // trj->ShowTrajectory();
        }
    }

    // hits
    G4int nHitColl = 0;
    G4HCofThisEvent *pHitColl = evt->GetHCofThisEvent();
    if (!pHitColl)
    {
        databuff.push_back(nHitColl); // number of hits collections
        return ;
    }
    databuff.push_back(nHitColl); // number of hits collections
    nHitColl = pHitColl->GetNumberOfCollections();
    // G4cout << "There are " << nHitColl << " hits collections." << G4endl;

    for (G4int i = 0; i < nHitColl; i++)
    {
        G4VHitsCollection *ithHitColl = pHitColl->GetHC(i);
        G4int collID = ithHitColl->GetColID();
        G4String collName = ithHitColl->GetName();
        G4String sdName = ithHitColl->GetSDname();
        size_t nHits = ithHitColl->GetSize();
        // G4cout << "The " << i << " collections#" << collID
        //	   << " name=" << collName
        //	   << " sdname=" << sdName
        //	   << " has " << nHits << " hits." << G4endl;
        databuff.push_back(collID);
        databuff.push_back(nHits);
        for (size_t j = 0; j < nHits; ++j)
        {
            PctSdVHit *aHit = (PctSdVHit *)ithHitColl->GetHit(j);
            databuff.push_back(aHit->GetID());
            databuff.push_back(G4int(100 * aHit->GetX()));
            databuff.push_back(G4int(100 * aHit->GetY()));
            databuff.push_back(G4int(100 * aHit->GetZ()));
            databuff.push_back(G4int(100 * aHit->GetTime()));
            databuff.push_back(G4int(100 * aHit->GetEnergy()));
            // aHit->Print();
        }
        // G4cout << "----------------------------------------" << G4endl;
    }

#ifdef USE_ROOT_IO
    PctROOTWriter *pWriter = PctROOTWriter::Instance();
    G4AutoLock lock(&PctRecordEventMutexFullRun);
    pWriter->write(databuff);
    lock.unlock();
#else
    PctRawWriter *pWriter = PctRawWriter::Instance();
    G4AutoLock lock(&PctRecordEventMutexFullRun);
    pWriter->write(databuff);
    lock.unlock();
#endif
}
