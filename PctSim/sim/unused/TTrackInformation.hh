//created by hujf@scnu.edu.cn @2021.10.22

#ifndef TTrackInformation_h
#define TTrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class TTrackInformation : public G4VUserTrackInformation 
{
public:
  TTrackInformation();
  TTrackInformation(const G4Track* aTrack);
  TTrackInformation(const TTrackInformation* aTrackInfo);
  virtual ~TTrackInformation();
   
  inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);

  TTrackInformation& operator =(const TTrackInformation& right);
  
  void SetSourceTrackInformation(const G4Track* aTrack);
  virtual void Print() const;

public:
  inline G4int GetTrackingStatus() const {return fTrackingStatus;}
  inline void  SetTrackingStatus(G4int i) {fTrackingStatus = i;}
  inline G4int GetSourceTrackID() const {return fSourceTrackID;}
  inline void  SetSuspendedStepID(G4int i) {fSuspendedStepID = i;}
  inline G4int GetSuspendedStepID() const {return fSuspendedStepID;}

private:
  // Information of the primary track at the primary vertex
  G4int                 fOriginalTrackID;  // Track ID of primary particle
  G4ParticleDefinition* fParticleDefinition;
  G4ThreeVector         fOriginalPosition;
  G4ThreeVector         fOriginalMomentum;
  G4double              fOriginalEnergy;
  G4double              fOriginalTime;

  G4int                 fTrackingStatus;
  // trackingStatus = 1 : primary or secondary track which has not yet reached 
  //                      to calorimeter
  //                = 0 : track which or ancester of which has reached to 
  //                      calorimeter
  //                = 2 : track or its ancester had reached to calorimeter 
  //                      and then escaped from it
  // Information of the track which reached to the calorimeter boundary at the 
  // boundary surface. 
  // This information is valid only for trackingStatus = 0 or 2
  G4int                 fSourceTrackID;
  G4ParticleDefinition* fSourceDefinition;
  G4ThreeVector         fSourcePosition;
  G4ThreeVector         fSourceMomentum;
  G4double              fSourceEnergy;
  G4double              fSourceTime;
  G4int                 fSuspendedStepID;
};

extern G4ThreadLocal
 G4Allocator<TTrackInformation> * aTrackInformationAllocator;

inline void* TTrackInformation::operator new(size_t)
{
  if(!aTrackInformationAllocator)
    aTrackInformationAllocator = new G4Allocator<TTrackInformation>;
  return (void*)aTrackInformationAllocator->MallocSingle();
}

inline void TTrackInformation::operator delete(void *aTrackInfo)
{ aTrackInformationAllocator->FreeSingle((TTrackInformation*)aTrackInfo);}

#endif

