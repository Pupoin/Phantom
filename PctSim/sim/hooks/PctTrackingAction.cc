#include "PctTrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "PctRunOpt.hh"
#include "PctTrajectory.hh"


void PctTrackingAction::PreUserTrackingAction(const G4Track* aTrack) 
{
  fpTrackingManager->SetStoreTrajectory(true);
  fpTrackingManager->SetTrajectory(new PctTrajectory(aTrack));

  // set filters here to control information for different types track.
  //if (TrackingOptionNotApplied)
  //{
  //  //StoreTrajectoryOpt = fpTrackingManager->GetStoreTrajectory();
  //  if (PctRunOpt::OptTrackingPrimaryAndSecondary)
  //  {
  //    fpTrackingManager->SetStoreTrajectory(3);
  //  }
  //  else
  //  {
  //    if (PctRunOpt::OptTrackingPrimaryOnly)
  //    {
  //      if (aTrack->GetTrackID() == 1)
  //        fpTrackingManager->SetStoreTrajectory(1);
  //      else
  //        fpTrackingManager->SetStoreTrajectory(0);
  //    }
  //    else
  //    {
  //      if (aTrack->GetTrackID() == 1)
  //        fpTrackingManager->SetStoreTrajectory(0);
  //      else
  //        fpTrackingManager->SetStoreTrajectory(1);
  //    }
  //  }
  //  TrackingOptionNotApplied = false;
  //}
  //考虑支持设定每种粒子
  //if(aTrack->GetTrackID() == 1){
  // G4cout << __FUNCTION__ << "::TRACK ID=" << aTrack->GetTrackID() << G4endl;
   // G4cout <<"Track ID"<<aTrack->GetTrackID()
   // <<", creatorModelName="<<aTrack->GetCreatorModelName()
   // <<", volume name="<<aTrack->GetVolume()->GetName()
   // <<", next volume name="<<aTrack->GetNextVolume()->GetName()
   // <<", history depth="<<aTrack->GetTouchable()->GetHistory()->GetDepth()
   // <<G4endl;
  //}
}

void PctTrackingAction::PostUserTrackingAction(const G4Track *aTrack)
{
  //if(StoreTrajectoryOpt != fpTrackingManager->GetStoreTrajectory()){
  //  TrackingOptionNotApplied = true;
  //}
  //test some functions of geant4
 // if(aTrack->GetTrackID() == 1){
  // G4cout << __FUNCTION__ << "::TRACK ID=" << aTrack->GetTrackID() << G4endl;
 //   G4cout <<"Track ID"<<aTrack->GetTrackID()
 //   <<", creatorModelName="<<aTrack->GetCreatorModelName()
 //   <<", volume name="<<aTrack->GetVolume()->GetName()
 //   <<", next volume name="<<aTrack->GetNextVolume()->GetName()
 //   <<", history A depth="<<aTrack->GetTouchable()->GetHistoryDepth()
 //   <<", history B depth="<<aTrack->GetTouchable()->GetHistory()->GetDepth()
 //   <<", volume 1="<<aTrack->GetTouchable()->GetVolume(0)->GetName()
 //   <<", volume 2="<<aTrack->GetTouchable()->GetVolume(1)->GetName()
 //   <<", volume 3="<<aTrack->GetTouchable()->GetVolume(2)->GetName()
 //   <<", copy A NO ="<<aTrack->GetVolume()->GetCopyNo()
 //   <<", copy B NO ="<<aTrack->GetTouchable()->GetCopyNumber()
 //   <<", copy C name ="<<aTrack->GetTouchableHandle()->GetHistory()->GetTopVolume()->GetName()
 //   <<G4endl;
 // }
}
