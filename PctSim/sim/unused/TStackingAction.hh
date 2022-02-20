//created by hujf@scnu.edu.cn @2021.10.22
#ifndef TStackingAction_H
#define TStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class G4Track;
class G4VHitsCollection;

class TStackingAction : public G4UserStackingAction
{
public:
  TStackingAction();
  virtual ~TStackingAction();

public:
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void NewStage();
  virtual void PrepareNewEvent();

private:
  G4VHitsCollection* GetCalCollection();
  
  G4int fStage;
  G4int fCalorimeterHitsColID;
};

#endif

