#pragma once

#include "G4VHit.hh"
#include "PctSdVHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"
#include "G4SystemOfUnits.hh"

/*class PctSdTrackerHit : public G4VHit
{
  public:
    PctSdTrackerHit();
    PctSdTrackerHit(const PctSdTrackerHit&);
    virtual ~PctSdTrackerHit();

    // operators
    const PctSdTrackerHit& operator=(const PctSdTrackerHit&);
    G4bool operator==(const PctSdTrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    //virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void Add(G4int id, G4double de, G4double dt, G4ThreeVector pos);

    G4int GetID() const { return fDetId; }

    G4ThreeVector GetPosition() const { return fPos; }
    G4double GetX() const { return fPos.x(); }
    G4double GetY() const { return fPos.y(); }
    G4double GetZ() const { return fPos.z(); }
    
    G4double GetTime() const { return fT; }
    G4double GetEnergy() const { return fE; }
    G4int GetNumOfPoints() const { return fNP; }

  private:
    G4int fDetId;//detector id
    G4ThreeVector fPos;
    G4double fE; ///< Energy deposit in the sensitive volume
    G4double fT; ///< hitting time in the sensitive volume
    G4double fTmin; ///< hitting time
    G4double fTmax; ///< hitting time
    G4int fNP;//number of contributed step points
};*/

class PctSdTrackerHit : public PctSdVHit
{
  public:
    PctSdTrackerHit();
    PctSdTrackerHit(const PctSdTrackerHit&);
    virtual ~PctSdTrackerHit();

    // operators
    const PctSdTrackerHit& operator=(const PctSdTrackerHit&);
    G4bool operator==(const PctSdTrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);
};

using PctSdTrackerHitsCollection = G4THitsCollection<PctSdTrackerHit>;

extern G4ThreadLocal G4Allocator<PctSdTrackerHit> *PctSdTrackerHitAllocator;

inline void *PctSdTrackerHit::operator new(size_t)
{
  if (!PctSdTrackerHitAllocator)
  {
    PctSdTrackerHitAllocator = new G4Allocator<PctSdTrackerHit>;
  }
  return (void *)PctSdTrackerHitAllocator->MallocSingle();
}

inline void PctSdTrackerHit::operator delete(void *hit)
{
  if (!PctSdTrackerHitAllocator)
  {
    PctSdTrackerHitAllocator = new G4Allocator<PctSdTrackerHit>;
  }
  PctSdTrackerHitAllocator->FreeSingle((PctSdTrackerHit *)hit);
}

/*
inline void PctSdTrackerHit::Add(G4int id, G4double de, G4double dt, G4ThreeVector pos)
{
  ++ fNP;
  fDetId = id;
  fE += de;
  fPos += pos;
  fT += dt;

  if (fNP > 0.)
  {
    fPos /= fNP;
    fT /= fNP;
  }
  if (dt < fTmin)
    fTmin = dt;
  if (dt > fTmax)
    fTmax = dt;
}
*/