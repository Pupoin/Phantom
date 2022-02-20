#pragma once

#include "G4VHit.hh"
#include "PctSdVHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/*class PctSdTofHit :  public G4VHit
{
  public:
    PctSdTofHit();
    PctSdTofHit(const PctSdTofHit&);
    virtual ~PctSdTofHit();

    // operators
    const PctSdTofHit& operator=(const PctSdTofHit&);
    G4bool operator==(const PctSdTofHit&) const;

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
class PctSdTofHit :  public PctSdVHit
{
  public:
    PctSdTofHit();
    PctSdTofHit(const PctSdTofHit&);
    virtual ~PctSdTofHit();

    // operators
    const PctSdTofHit& operator=(const PctSdTofHit&);
    G4bool operator==(const PctSdTofHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);
};


using PctSdTofHitsCollection = G4THitsCollection<PctSdTofHit>;

extern G4ThreadLocal G4Allocator<PctSdTofHit>* PctSdTofHitAllocator;

inline void* PctSdTofHit::operator new(size_t)
{
  if (!PctSdTofHitAllocator) {
    PctSdTofHitAllocator = new G4Allocator<PctSdTofHit>;
  }
  return (void *) PctSdTofHitAllocator->MallocSingle();
}

inline void PctSdTofHit::operator delete(void *hit)
{
  if (!PctSdTofHitAllocator) {
    PctSdTofHitAllocator = new G4Allocator<PctSdTofHit>;
  }
  PctSdTofHitAllocator->FreeSingle((PctSdTofHit*) hit);
}

/*inline void PctSdTofHit::Add(G4int id, G4double de, G4double dt, G4ThreeVector pos)
{
  ++ fNP;
  fDetId = id;
  fE += de;
  fPos += pos;
  fT += dt;

  if (fNP > 0)
  {
    fPos /= fNP;
    fT /= fNP;
  }
  if (dt < fTmin)
    fTmin = dt;
  if (dt > fTmax)
    fTmax = dt;
}*/