#include "PctIdealRunB.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4AutoLock.hh"
#include "g4root.hh"
#include <mutex>
#include <thread>
#include "PctRawWriter.hh"
#include "PctROOTWriter.hh"
#include "G4SystemOfUnits.hh"
#include "PctRunOpt.hh"
#include "PctTrajectory.hh"
#include "PctTrajectoryPoint.hh"

namespace {
  G4Mutex PctRecordEventMutexPctIdealRunB = G4MUTEX_INITIALIZER;
}

 PctIdealRunB::PctIdealRunB():
   G4Run(/* args */)
{
    databuff.reserve(10000);
}

PctIdealRunB::~PctIdealRunB()
{
    databuff.clear();
}
void PctIdealRunB::Merge(const G4Run *aRun)
{
    G4Run::Merge(aRun);
}

//#define USE_PCTEVENT 1

void PctIdealRunB::RecordEvent(const G4Event *evt)
{
    //G4cout << __FUNCTION__ << "::BEGIN " << evt->GetEventID() << G4endl;
    //G4cout<<"run = "<<this->GetRunID()<<", "<<this->GetNumberOfEvent()<<G4endl;
    G4Run::RecordEvent(evt);
    databuff.clear();

#ifdef USE_PCTEVENT
    pctevt.clear();
#endif

    G4TrajectoryContainer *trajectoryContainer = evt->GetTrajectoryContainer();
    G4int n_trajectories = 0;
    if (trajectoryContainer)
        n_trajectories = trajectoryContainer->entries();

#ifdef USE_PCTEVENT
    pctevt.run_id = GetRunID();
    pctevt.evt_id = evt->GetEventID();
    pctevt.tracks.resize(n_trajectories);
    //G4cout <<"traj #"<<n_trajectories<<G4endl;
#else
    G4int run_evt = this->GetRunID() << 20;
    run_evt |= evt->GetEventID();//encode run_evt together
    databuff.push_back(run_evt);
    databuff.push_back(n_trajectories);
#endif

    for (G4int i = 0; i < n_trajectories; i++)
    {
        PctTrajectory *trj = (PctTrajectory *)((*(evt->GetTrajectoryContainer()))[i]);
#ifdef USE_PCTEVENT
        pctevt.tracks[i].pdg_code = trj->GetPDGEncoding();
        pctevt.tracks[i].track_id = trj->GetTrackID();
        pctevt.tracks[i].parent_id = trj->GetParentID();
        pctevt.tracks[i].points.resize(trj->GetPointEntries());
#else
        databuff.push_back(trj->GetPDGEncoding());
        databuff.push_back(trj->GetTrackID());
        databuff.push_back(trj->GetParentID());
        databuff.push_back(trj->GetPointEntries());
#endif

        //G4cout << "traj #" << i << " has " << trj->GetPointEntries() << " points" << G4endl;
        for (G4int j = 0; j < trj->GetPointEntries(); ++j)
        {
            PctTrajectoryPoint *trjp = (PctTrajectoryPoint *)trj->GetPoint(j);
            G4ThreeVector value = trjp->GetPosition();
#ifdef USE_PCTEVENT
            //  trj->ShowTrajectory();
            pctevt.tracks[i].points[j].x = value.x();
            pctevt.tracks[i].points[j].y = value.y();
            pctevt.tracks[i].points[j].z = value.z();
            pctevt.tracks[i].points[j].eDep = trjp->GetEnergyDeposit();
            pctevt.tracks[i].points[j].eTot = trjp->GetEnergyTotal();
            pctevt.tracks[i].points[j].materialIndex = trjp->GetMatertialIndex();
            pctevt.tracks[i].points[j].processType = trjp->GetProcessType();
            pctevt.tracks[i].points[j].status = trjp->GetStatus();
#else
            databuff.push_back(G4int(100 * value.x() / mm));
            databuff.push_back(G4int(100 * value.y() / mm));
            databuff.push_back(G4int(100 * value.z() / mm));
            databuff.push_back(G4int(100 * trjp->GetEnergyDeposit()/ keV));
            //databuff.push_back(G4int(100 * trjp->GetEnergyTotal()/keV));
            //databuff.push_back( trjp->GetStatus());
            databuff.push_back(trjp->GetMatertialIndex());
            databuff.push_back(trjp->GetProcessType());
#endif
        }
    }
    //pctevt.print();
#ifdef USE_ROOT_IO
    G4AutoLock lock(&PctRecordEventMutexPctIdealRunB);
    PctROOTWriter *pWriter = PctROOTWriter::Instance();
#ifdef USE_PCTEVENT
    pctevt.writeTo(databuff);
#endif
    pWriter->write(databuff);
    lock.unlock();
#else
    G4AutoLock lock(&PctRecordEventMutexPctIdealRunB);
    PctRawWriter *pWriter = PctRawWriter::Instance();
#ifdef USE_PCTEVENT
    pctevt.writeTo(databuff);
#endif
    pWriter->write(databuff);
    lock.unlock();
#endif
}
