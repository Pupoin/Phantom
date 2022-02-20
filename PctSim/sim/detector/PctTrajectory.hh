#pragma once

#include "G4Trajectory.hh"
#include "G4Allocator.hh"
#include <stdlib.h>
#include "G4ThreeVector.hh"
#include "G4ios.hh"     
#include "globals.hh" 
#include "G4ParticleDefinition.hh" 
#include "G4TrajectoryPoint.hh"   
#include "G4Track.hh"
#include "G4Step.hh"
#include <vector>

//class G4Polyline;
//class G4AttDef;
//class G4AttValue;

typedef std::vector<G4VTrajectoryPoint*> PctTrajectoryPointContainer;

class PctTrajectory : public G4Trajectory
{
	public:
		PctTrajectory(const G4Track* aTrack);
		PctTrajectory(PctTrajectory &);
		virtual ~PctTrajectory();

		//virtual void ShowTrajectory(std::ostream& os=G4cout) const;
		//virtual void DrawTrajectory() const;

		virtual void AppendStep(const G4Step *aStep);
		virtual void MergeTrajectory(G4VTrajectory *secondTrajectory);

		virtual int GetPointEntries() const { return fPositionRecord->size(); }
		virtual G4VTrajectoryPoint *GetPoint(G4int i) const
		{
			return (*fPositionRecord)[i];
		}

		inline void *operator new(size_t);
		inline void operator delete(void *);
		inline int operator==(const PctTrajectory &right) const;

	private:
		PctTrajectoryPointContainer *fPositionRecord;
		G4String fCreatorProcessName;
		G4int fCreatorProcessType;
		G4ThreeVector fMomentum;
		G4ThreeVector fVertex;
		//G4int fParticleIndex;
};

extern G4ThreadLocal G4Allocator<PctTrajectory>* myPctTrajectoryAllocator;

inline void *PctTrajectory::operator new(size_t)
{
	if (!myPctTrajectoryAllocator)
		myPctTrajectoryAllocator = new G4Allocator<PctTrajectory>;
	return (void *)myPctTrajectoryAllocator->MallocSingle();
}

inline void PctTrajectory::operator delete(void *aTrajectory)
{
	myPctTrajectoryAllocator->FreeSingle((PctTrajectory *)aTrajectory);
}
inline G4int PctTrajectory::operator == (const PctTrajectory& r) const
{
  return (this==&r);
}

/*extern G4TRACKING_DLL G4Allocator<PctTrajectory>*& aPctTrajectoryAllocator();

inline void* PctTrajectory::operator new(size_t)
{
  if (aPctTrajectoryAllocator() == nullptr)
  {
    aPctTrajectoryAllocator() = new G4Allocator<PctTrajectory>;
  }
  return (void*)aPctTrajectoryAllocator()->MallocSingle();
}

inline void PctTrajectory::operator delete(void* aTrajectory)
{
  aPctTrajectoryAllocator()->FreeSingle((PctTrajectory*)aTrajectory);
}

inline G4int PctTrajectory::operator == (const PctTrajectory& r) const
{
  return (this==&r);
}*/
