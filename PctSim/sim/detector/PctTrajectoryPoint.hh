
#pragma once

#include "G4TrajectoryPoint.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"

class G4Material;
class G4Step;
class G4Track;

//
////////////////////////
class PctTrajectoryPoint : public G4TrajectoryPoint
{
	public: 
		// Constructor/Destructor
		PctTrajectoryPoint(const G4Track* aTrack);//the initial point
		PctTrajectoryPoint(const G4Step* aStep);//the other points

		PctTrajectoryPoint(const PctTrajectoryPoint &right);
		virtual ~PctTrajectoryPoint();

		// Operators
		inline void *operator new(size_t);
		inline void operator delete(void *aTrajectoryPoint);
		inline G4bool operator==(const PctTrajectoryPoint& right) const
		{ return (this==&right); };

		//const G4ThreeVector GetPosition() const { return fPosition; }
		G4double GetEnergyDeposit() const { return fEnergyDeposit; }
		G4double GetEnergyTotal() const { return fRemainingEnergy; }
		G4int GetMatertialIndex() const { return fMaterialIndex; }
		G4int GetProcessType() const { return fProcessType; }
		G4int GetStatus() const { return fStatus; }
		G4String GetProcessName() const { return fProcessName; }
		G4String GetPVName() const { return fPVName; }

	private:
		//G4ThreeVector fPosition = G4ThreeVector(0.,0.,0.);
		G4double fEnergyDeposit = -1.;
		G4double fRemainingEnergy = -1.;
		G4int fMaterialIndex = -1;
		G4int fProcessType = -1;
		G4int fStatus = -1;
		G4String fProcessName;
		G4String fPVName;

		// G4StepStatus fPreStepPointStatus = fUndefined;
		// G4StepStatus fPostStepPointStatus = fUndefined;
		// G4double fPreStepPointGlobalTime = 0.0;
		// G4double fPostStepPointGlobalTime = 0.0;
		// G4TouchableHandle fpPreStepPointVolume;
		// G4TouchableHandle fpPostStepPointVolume;
		// G4double fPreStepPointWeight = 0.0;
		// G4double fPostStepPointWeight = 0.0;
};

extern G4ThreadLocal G4Allocator<PctTrajectoryPoint> *fPctTrajPointAllocator;

inline void *PctTrajectoryPoint::operator new(size_t)
{
	if (!fPctTrajPointAllocator)
		fPctTrajPointAllocator = new G4Allocator<PctTrajectoryPoint>;
	return (void *)fPctTrajPointAllocator->MallocSingle();
}

inline void PctTrajectoryPoint::operator delete(void *aTrajectoryPoint)
{
	fPctTrajPointAllocator->FreeSingle((PctTrajectoryPoint *)aTrajectoryPoint);
}