//created by hujf@scnu.edu.cn @ 2021.10.22
#ifndef PctRunAction_h
#define PctRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4PhysicsVector.hh"

class G4Run;

class PctRunAction : public G4UserRunAction
{
public:
  PctRunAction();
  virtual ~PctRunAction();

  virtual G4Run* GenerateRun();
public:
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

private:
  G4bool isFileOK;
};

#endif
