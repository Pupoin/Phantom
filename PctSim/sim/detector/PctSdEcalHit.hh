#pragma once

#include "G4VHit.hh"
#include "PctSdVHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/*class PctSdEcalHit : public G4VHit
{
  public:
    PctSdEcalHit();
    PctSdEcalHit(const PctSdEcalHit&);
    virtual ~PctSdEcalHit();

    // operators
    const PctSdEcalHit& operator=(const PctSdEcalHit&);
    G4bool operator==(const PctSdEcalHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    //virtual void Draw() {}
    virtual void Print();

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
    G4int fNP;
};*/
class PctSdEcalHit : public PctSdVHit
{
  public:
    PctSdEcalHit();
    PctSdEcalHit(const PctSdEcalHit&);
    virtual ~PctSdEcalHit();

    // operators
    const PctSdEcalHit& operator=(const PctSdEcalHit&);
    G4bool operator==(const PctSdEcalHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);
};


using PctSdEcalHitsCollection = G4THitsCollection<PctSdEcalHit>;

extern G4ThreadLocal G4Allocator<PctSdEcalHit>* PctSdEcalHitAllocator;

inline void* PctSdEcalHit::operator new(size_t)
{
  if (!PctSdEcalHitAllocator) {
    PctSdEcalHitAllocator = new G4Allocator<PctSdEcalHit>;
  }
  return (void *) PctSdEcalHitAllocator->MallocSingle();
}

inline void PctSdEcalHit::operator delete(void *hit)
{
  if (!PctSdEcalHitAllocator) {
    PctSdEcalHitAllocator = new G4Allocator<PctSdEcalHit>;
  }
  PctSdEcalHitAllocator->FreeSingle((PctSdEcalHit*) hit);
}

/*
inline void PctSdEcalHit::Add(G4int id, G4double de, G4double dt, G4ThreeVector pos)
{
  fDetId = id;
  fE += de;
  fPos += de*pos;
  fT += de* dt;

  if (fE > 0.)
  {
    fPos /= fE;
    fT /= fE;
  }
  if (dt < fTmin)
    fTmin = dt;
  if (dt > fTmax)
    fTmax = dt;
}*/