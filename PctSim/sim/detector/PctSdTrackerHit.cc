#include "PctSdTrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<PctSdTrackerHit>* PctSdTrackerHitAllocator = 0;

PctSdTrackerHit::PctSdTrackerHit()
:PctSdVHit("PctSdTrackerHit"){}

PctSdTrackerHit::~PctSdTrackerHit() {}

PctSdTrackerHit::PctSdTrackerHit(const PctSdTrackerHit &right)
    : PctSdVHit(right)
{
}
const PctSdTrackerHit &PctSdTrackerHit::operator=(const PctSdTrackerHit &right)
{
  if (this != &right)
  {
    PctSdVHit::operator=(right);
  }
  return *this;
}

G4bool PctSdTrackerHit::operator==(const PctSdTrackerHit &right) const
{
  return (this == &right) ? true : false;
}
/*
PctSdTrackerHit::PctSdTrackerHit()
 : G4VHit(),
   fDetId(0),
   fPos(0.,0.,0.),
   fE(0.),
   fT(0.),
   fTmin(__DBL_MAX__),
   fTmax(__DBL_MIN__),
   fNP(0)
{}

PctSdTrackerHit::~PctSdTrackerHit() {}

PctSdTrackerHit::PctSdTrackerHit(const PctSdTrackerHit& right)
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

const PctSdTrackerHit& PctSdTrackerHit::operator=(const PctSdTrackerHit& right)
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

G4bool PctSdTrackerHit::operator==(const PctSdTrackerHit &right) const
{
  return (this == &right) ? true : false;
}
void PctSdTrackerHit::Print()
{
  G4cout << std::setiosflags(std::ios::right) << std::setiosflags(std::ios::fixed) << std::setprecision(3);
  G4cout << std::setw(15) << "SdTrackerHit"
         << std::setw(6) << GetID()
         << std::setw(6) << GetNumOfPoints()
         << std::setw(10) << fTmin << "ns"
         << std::setw(10) << GetTime() << "ns"
         << std::setw(10) << fTmax << "ns"
         << std::setw(10) << GetEnergy() << "keV"
         << std::setw(10) << GetPosition() << "mm"
         << G4endl;
}*/
