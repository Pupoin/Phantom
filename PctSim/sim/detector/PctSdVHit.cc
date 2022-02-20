
#include "PctSdVHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"
#include <limits>
#include <iomanip>

G4ThreadLocal G4Allocator<PctSdVHit>* PctSdVHitAllocator = 0;

PctSdVHit::PctSdVHit(const G4String& name)
 : G4VHit(),
   fDetId(0),
   fPos(0.,0.,0.),
   fE(0.),
   fT(0.),
   fTmin(std::numeric_limits< double> ::max()),
   fTmax(std::numeric_limits< double> ::min()),
   fNP(0),
   fClassName(name)
{}

PctSdVHit::~PctSdVHit() {}

PctSdVHit::PctSdVHit(const PctSdVHit& right)
  : G4VHit()
{
  fDetId = right.fDetId;
  fPos = right.fPos;
  fE = right.fE;
  fT = right.fT;
  fTmin = right.fTmin;
  fTmax = right.fTmax;
  fNP = right.fNP;
  fClassName = right.fClassName;
}

const PctSdVHit& PctSdVHit::operator=(const PctSdVHit& right)
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
    fClassName = right.fClassName;
  }
  return *this;
}

G4bool PctSdVHit::operator==(const PctSdVHit &right) const
{
    return (this == &right) ? true : false;
}
void PctSdVHit::Print()
{
    G4cout << std::setiosflags(std::ios::right) << std::setiosflags(std::ios::fixed) << std::setprecision(3);
    G4cout << std::setw(15) << fClassName
           << std::setw(6) << GetID()
           << std::setw(6) << GetNumOfPoints()
           << std::setw(10) << fTmin << "ns"
           << std::setw(10) << GetTime() << "ns"
           << std::setw(10) << fTmax << "ns"
           << std::setw(10) << GetEnergy() << "keV"
           << std::setw(10) << GetPosition() << "mm"
           << G4endl;
}
