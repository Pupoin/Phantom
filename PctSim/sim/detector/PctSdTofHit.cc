#include "PctSdTofHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<PctSdTofHit>* PctSdTofHitAllocator = 0;

PctSdTofHit::PctSdTofHit()
:PctSdVHit("PctSdTofHit"){}

PctSdTofHit::~PctSdTofHit() {}

PctSdTofHit::PctSdTofHit(const PctSdTofHit &right)
    : PctSdVHit(right)
{
}
const PctSdTofHit &PctSdTofHit::operator=(const PctSdTofHit &right)
{
  if (this != &right)
  {
    PctSdVHit::operator=(right);
  }
  return *this;
}

G4bool PctSdTofHit::operator==(const PctSdTofHit &right) const
{
  return (this == &right) ? true : false;
}

/*
PctSdTofHit::PctSdTofHit()
    : G4VHit(),
      fDetId(0),
      fPos(0., 0., 0.),
      fE(0.),
      fT(0.),
      fTmin(__DBL_MAX__),
      fTmax(__DBL_MIN__),
      fNP(0)
{
}

PctSdTofHit::~PctSdTofHit() {}

PctSdTofHit::PctSdTofHit(const PctSdTofHit &right)
    : G4VHit()
{
  fDetId = right.fDetId;
  fPos = right.fPos;
  fE = right.fE;
  fT = right.fT;
  fTmin = right.fTmin;
  fTmax = right.fTmax;
  fNP = right.fNP;
}

const PctSdTofHit &PctSdTofHit::operator=(const PctSdTofHit &right)
{
  if (this != &right)
  {
    fDetId = right.fDetId;
    fPos = right.fPos;
    fE = right.fE;
    fT = right.fT;
    fTmin = right.fTmin;
    fTmax = right.fTmax;
    fNP = right.fNP;
  }
  return *this;
}

G4bool PctSdTofHit::operator==(const PctSdTofHit &right) const
{
  return (this == &right) ? true : false;
}

void PctSdTofHit::Print()
{
  G4cout << std::setiosflags(std::ios::right) << std::setiosflags(std::ios::fixed) << std::setprecision(3);
  G4cout << std::setw(15) << "SdTofHit"
         << std::setw(6) << GetID()
         << std::setw(6) << GetNumOfPoints()
         << std::setw(10) << fTmin << "ns"
         << std::setw(10) << GetTime() << "ns"
         << std::setw(10) << fTmax << "ns"
         << std::setw(10) << GetEnergy() << "keV"
         << std::setw(10) << GetPosition() << "mm"
         << G4endl;
}*/
