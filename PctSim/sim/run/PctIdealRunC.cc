#include "PctIdealRunC.hh"
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

namespace {
  G4Mutex PctRecordEventMutex = G4MUTEX_INITIALIZER;
}

 PctIdealRunC::PctIdealRunC():
   G4Run(/* args */)
{
    databuff.reserve(10000);
}

PctIdealRunC::~PctIdealRunC()
{
    databuff.clear();
}
void PctIdealRunC::Merge(const G4Run *aRun)
{
    G4Run::Merge(aRun);
}

void PctIdealRunC::RecordEvent(const G4Event *evt)
{
    //G4cout << __FUNCTION__ << "::BEGIN " << evt->GetEventID() << G4endl;
    //G4cout<<"run = "<<this->GetRunID()<<", "<<this->GetNumberOfEvent()<<G4endl;
    G4Run::RecordEvent(evt);

    databuff.clear();

    G4TrajectoryContainer *trajectoryContainer = evt->GetTrajectoryContainer();
    G4int n_trajectories = 0;
    if (trajectoryContainer)
        n_trajectories = trajectoryContainer->entries();

    run_evt_ntrj.item.run = this->GetRunID();
    run_evt_ntrj.item.evt = evt->GetEventID();
    run_evt_ntrj.item.ntrj = n_trajectories;
    databuff.push_back(run_evt_ntrj.value);

    for (G4int i = 0; i < n_trajectories; i++)
    {
        G4VTrajectory *trj = (G4VTrajectory *)((*(evt->GetTrajectoryContainer()))[i]);
        pdg_tid_pid_npoints.item.pdg_id = trj->GetPDGEncoding();
        pdg_tid_pid_npoints.item.trk_id = trj->GetTrackID();
        pdg_tid_pid_npoints.item.parent = trj->GetParentID();
        pdg_tid_pid_npoints.item.npoints = trj->GetPointEntries();
        databuff.push_back(pdg_tid_pid_npoints.value);

        for (G4int j = 0; j < trj->GetPointEntries(); ++j)
        {
            G4VTrajectoryPoint *trjp = trj->GetPoint(j);
            // trj->ShowTrajectory();
            G4ThreeVector value = trjp->GetPosition();
            coor_3d.item.x = G4int(100 * value.x() / mm);
            coor_3d.item.y = G4int(100 * value.y() / mm);
            coor_3d.item.z = G4int(100 * value.z() / mm);
            databuff.push_back(coor_3d.value);
        }
    }

#ifdef USE_ROOT_IO
    //PctROOTWriter *pWriter = PctROOTWriter::Instance();
    //G4AutoLock lock(&PctRecordEventMutex);
    //pWriter->write(databuff);
    //lock.unlock();
#else
    PctRawWriter *pWriter = PctRawWriter::Instance();
    G4AutoLock lock(&PctRecordEventMutex);
    pWriter->write(databuff);
    lock.unlock();
#endif
}
