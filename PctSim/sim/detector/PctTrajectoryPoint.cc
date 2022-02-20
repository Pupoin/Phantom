//created by hujf@scnu.edu.cn @2021.10.24

#include "PctTrajectoryPoint.hh"
#include "G4UnitsTable.hh"
#include "G4Material.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

G4ThreadLocal G4Allocator<PctTrajectoryPoint> * fPctTrajPointAllocator= 0;


PctTrajectoryPoint::PctTrajectoryPoint(const G4Track* aTrack): 
	G4TrajectoryPoint(aTrack->GetPosition()),
	fEnergyDeposit(0.),
	fRemainingEnergy(aTrack->GetKineticEnergy()),
	fMaterialIndex(aTrack->GetMaterial()->GetIndex()),
	fProcessType(-99),
	fProcessName("Vertex"),
	fStatus(aTrack->GetTrackStatus()),
	fPVName("World")
{
	if (aTrack->GetCreatorProcess()){
		fProcessType = aTrack->GetCreatorProcess()->GetProcessType();
		fProcessName = aTrack->GetCreatorProcess()->GetProcessName();
	}
}
PctTrajectoryPoint::PctTrajectoryPoint(const G4Step *aStep)
	: G4TrajectoryPoint(aStep->GetPreStepPoint()->GetPosition())
{
	fEnergyDeposit = aStep->GetTotalEnergyDeposit();
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

	G4Material *mt = postStepPoint->GetMaterial();
	if (mt)
		fMaterialIndex = mt->GetIndex();
	fStatus = aStep->GetTrack()->GetTrackStatus();
	if (aStep->GetTrack()->GetCurrentStepNumber() <= 0) // First step
	{
		fRemainingEnergy = aStep->GetTrack()->GetKineticEnergy();
	}
	else
	{
		fRemainingEnergy = postStepPoint->GetKineticEnergy() - fEnergyDeposit;
	}
	fProcessType = postStepPoint->GetProcessDefinedStep()->GetProcessType();
	fProcessName = postStepPoint->GetProcessDefinedStep()->GetProcessName();
	G4VPhysicalVolume *curr_pv = postStepPoint->GetTouchableHandle()->GetVolume();
	if (curr_pv)
		fPVName = curr_pv->GetName();
	else
		fPVName = "END";
}

PctTrajectoryPoint::PctTrajectoryPoint(const PctTrajectoryPoint &right)
	: G4TrajectoryPoint(right.GetPosition()),
	  fEnergyDeposit(right.fEnergyDeposit),
	  fRemainingEnergy(right.fRemainingEnergy),
	  fMaterialIndex(right.fMaterialIndex),
	  fProcessType(right.fProcessType),
	  fStatus(right.fStatus),
	  fProcessName(right.fProcessName),
	  fPVName(right.fPVName)
{
}

PctTrajectoryPoint::~PctTrajectoryPoint()
{
}
