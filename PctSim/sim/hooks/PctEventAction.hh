//created by hujf@scnu.edu.cn @2021.10.22
//
#ifndef PctEventAction_h
#define PctEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

class G4PrimaryParticle;

class PctEventAction : public G4UserEventAction
{
	public:
		PctEventAction();
		~PctEventAction();

	public:
		virtual void BeginOfEventAction(const G4Event*);
		virtual void EndOfEventAction(const G4Event*);
};

#endif