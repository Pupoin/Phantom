#include "PctTrajectory.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
//#include "G4Polyline.hh"
//#include "G4Circle.hh"
//#include "G4Colour.hh"
//#include "G4AttDefStore.hh"
//#include "G4AttDef.hh"
//#include "G4AttValue.hh"
//#include "G4UIcommand.hh"
//#include "G4VisAttributes.hh"
//#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "G4DynamicParticle.hh"
#include "G4PrimaryParticle.hh"
//#include "PctTrackInformation.hh"
#include "PctTrajectoryPoint.hh"
#include "G4VProcess.hh"

G4ThreadLocal G4Allocator<PctTrajectory>* myPctTrajectoryAllocator = 0;

PctTrajectory::PctTrajectory(const G4Track *aTrack)
    : G4Trajectory(aTrack), fPositionRecord(NULL)
{
   const G4VProcess *theProc = aTrack->GetCreatorProcess();
   if (theProc)
   {
      fCreatorProcessName = theProc->GetProcessName();
      fCreatorProcessType = theProc->GetProcessType();
   }
   // fKineticEnergy = aTrack->GetKineticEnergy();
   fPositionRecord = new PctTrajectoryPointContainer();
   //fPositionRecord->push_back(new PctTrajectoryPoint(aTrack));
   //fMomentum = aTrack->GetMomentum();
   //fVertex = aTrack->GetPosition();
}

PctTrajectory::~PctTrajectory()
{
   size_t i;
   for (i = 0; i < fPositionRecord->size(); i++)
   {
      delete (*fPositionRecord)[i];
   }
   fPositionRecord->clear();
   delete fPositionRecord;
}
void PctTrajectory::AppendStep(const G4Step *aStep)
{
   fPositionRecord->push_back(new PctTrajectoryPoint(aStep));
}

void PctTrajectory::MergeTrajectory(G4VTrajectory *secondTrajectory)
{
   if (!secondTrajectory)
      return;

   PctTrajectory *seco = (PctTrajectory *)secondTrajectory;
   G4int ent = seco->GetPointEntries();
   //
   // initial point of the second trajectory should not be merged
   for (int i = 1; i < ent; i++)
   {
      fPositionRecord->push_back((*(seco->fPositionRecord))[i]);
   }
   delete (*seco->fPositionRecord)[0];
   seco->fPositionRecord->clear();
}
PctTrajectory::PctTrajectory(PctTrajectory &right)
    : G4Trajectory(right)
{
   fCreatorProcessName = right.fCreatorProcessName;
   fCreatorProcessType = right.fCreatorProcessType;
   fPositionRecord = new PctTrajectoryPointContainer();
   for (size_t i = 0; i < right.fPositionRecord->size(); i++)
   {
      PctTrajectoryPoint *rightPoint = (PctTrajectoryPoint *)((*(right.fPositionRecord))[i]);
      fPositionRecord->push_back(new PctTrajectoryPoint(*rightPoint));
   }
}