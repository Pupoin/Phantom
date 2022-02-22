
#include "UserAction/TrackingAction.h"
#include "RootManager/RootManager.h"
#include "Control/Control.h"

#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction() : G4UserTrackingAction() {
    mcp = nullptr;
}

TrackingAction::~TrackingAction() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track *aTrack) {

    if (aTrack->GetParticleDefinition()->GetParticleName() != "opticalphoton") {
        auto fMC = new MCParticle();

        fMC->setName(aTrack->GetParticleDefinition()->GetParticleName());
        fMC->setPdg(aTrack->GetParticleDefinition()->GetPDGEncoding());
        fMC->setId(aTrack->GetTrackID());
        fMC->setMass(aTrack->GetParticleDefinition()->GetPDGMass());
        fMC->setEnergy(aTrack->GetTotalEnergy());
        fMC->setPx(aTrack->GetMomentum()[0]);
        fMC->setPy(aTrack->GetMomentum()[1]);
        fMC->setPz(aTrack->GetMomentum()[2]);
        fMC->setVertexX(aTrack->GetPosition()[0]);
        fMC->setVertexY(aTrack->GetPosition()[1]);
        fMC->setVertexZ(aTrack->GetPosition()[2]);

        if (aTrack->GetCreatorProcess())
            fMC->setCreateProcess(aTrack->GetCreatorProcess()->GetProcessName());

        pRootMng->FillSimTrack(pControl->MCParticle_Name,fMC, aTrack->GetParentID());

        mcp = fMC;

    }
    // G4cout << __LINE__ << "my pre track energy" << aTrack->GetTotalEnergy() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track *aTrack) {
    if (mcp) {
        mcp->setERemain(aTrack->GetKineticEnergy());
        mcp->setEndPointX(aTrack->GetStep()->GetPreStepPoint()->GetPosition()[0]);
        mcp->setEndPointY(aTrack->GetStep()->GetPreStepPoint()->GetPosition()[1]);
        mcp->setEndPointZ(aTrack->GetStep()->GetPreStepPoint()->GetPosition()[2]);
    }

    mcp = nullptr;
    // G4cout << __LINE__ << "my post track energy" << aTrack->GetTotalEnergy() << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
