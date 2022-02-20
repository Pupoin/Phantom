#include "PctIdealRunA.hh"
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

namespace {
  G4Mutex PctRecordEventMutexIdealRunA = G4MUTEX_INITIALIZER;
}

 PctIdealRunA::PctIdealRunA():
   G4Run(/* args */)
{
    databuff.reserve(10000);
}

PctIdealRunA::~PctIdealRunA()
{
    //databuff.clear();
}
void PctIdealRunA::Merge(const G4Run *aRun)
{
    G4Run::Merge(aRun);
}

void PctIdealRunA::RecordEvent(const G4Event *evt)
{
    //G4cout << __FUNCTION__ << "::BEGIN " << evt->GetEventID() << G4endl;
    //G4cout<<"run = "<<this->GetRunID()<<", "<<this->GetNumberOfEvent()<<G4endl;
    G4Run::RecordEvent(evt);

    databuff.clear();

    G4TrajectoryContainer *trajectoryContainer = evt->GetTrajectoryContainer();
    G4int n_trajectories = 0;
    if (trajectoryContainer)
        n_trajectories = trajectoryContainer->entries();

    G4int run_evt = this->GetRunID() << 20;
    run_evt |= evt->GetEventID();//encode run_evt together

    databuff.push_back(run_evt);
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
            // trj->ShowTrajectory();
            G4ThreeVector value = trjp->GetPosition();
            databuff.push_back(G4int(100 * value.x() / mm));
            databuff.push_back(G4int(100 * value.y() / mm));
            databuff.push_back(G4int(100 * value.z() / mm));
            databuff.push_back(G4int(100 * trjp->GetEnergyDeposit()/ keV));
            //databuff.push_back(G4int(100 * trjp->GetEnergyTotal()/keV));
            //databuff.push_back( trjp->GetStatus());
            databuff.push_back(trjp->GetMatertialIndex());
            databuff.push_back(trjp->GetProcessType());
        }
    }

#ifdef USE_ROOT_IO
    PctROOTWriter *pWriter = PctROOTWriter::Instance();
    G4AutoLock lock(&PctRecordEventMutexIdealRunA);
    pWriter->write(databuff);
    lock.unlock();
#else
    PctRawWriter *pWriter = PctRawWriter::Instance();
    G4AutoLock lock(&PctRecordEventMutexIdealRunA);
    pWriter->write(databuff);
    lock.unlock();
#endif
}