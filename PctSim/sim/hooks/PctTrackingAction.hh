//created by hujf@scnu.edu.cn @ 2021.10.22
#ifndef PctTrackingAction_h
#define PctTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4Types.hh"

//
//
class PctTrackingAction : public G4UserTrackingAction {

  public:
    PctTrackingAction() : G4UserTrackingAction(), TrackingOptionNotApplied(true) {};
    virtual ~PctTrackingAction(){};
   
    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

    private:
    G4bool TrackingOptionNotApplied;
    G4int StoreTrajectoryOpt;

};

#endif
