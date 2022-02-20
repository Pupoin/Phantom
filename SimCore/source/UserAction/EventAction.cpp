
// $Id$
//
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "UserAction/EventAction.h"
#include "RootManager/RootManager.h"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4THitsCollection.hh"
#include "G4UnitsTable.hh"

#include <iostream>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction() : G4UserEventAction(), fPrintModulo(10000) {
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
= default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event *event) {
    //fPrintModulo = pControl->Total_Event_Number / 100;

    G4int eventID = event->GetEventID();
    if ( eventID % fPrintModulo == 0) {
        G4cout << "\n---> Begin of event: " << eventID << G4endl;
        //CLHEP::HepRandom::showEngineStatus();
    }
    cout << "hahahahhahha" << endl;
    G4RunManager::GetRunManager()->StoreRandomNumberStatusToG4Event(1);

#ifdef MEMCK
    //if(pControl->memory_check) PCTEvent::PrintObjectStatistics("Begin of event");
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event *event) {
    if (event->IsAborted()) {
        pRootMng->initialize();
        return;
    }

    const G4String &RndmS = G4RunManager::GetRunManager()->GetRandomNumberStatusForThisEvent();
    const char *rn = RndmS.data();

    long double r1 = 0;
    int Nr = 0;
    double nr = 10000;
    double rndm[4];
    for (int i = 24; i < 100; i++) {
        if (Nr > 3) break;
        if ((int) rn[i] - (int) '0' == -38) {
            rndm[Nr] = r1 * pow(10, -1 - (log(nr) / log(10)));
            nr = 10000;
            r1 = 0;
            Nr++;
            //G4cout<<"RR: "<<rndm[Nr-1]<<G4endl;
            continue;
        }

        r1 = r1 + nr * (double) ((int) rn[i] - (int) '0');
        nr /= 10.0;
    }

    // print per event (modulo n)
    G4int eventID = event->GetEventID();
    if (eventID < 100 || eventID % fPrintModulo == 0) {
        G4cout << "---> End of event: " << eventID << G4endl;
    }

    // Filling tree, any data manipulations should before this point
    pRootMng->FillTree(eventID, rndm);

    cout << "end of hahahahhahha" << endl;


#ifdef MEMCK
    if(pControl->memory_check) PCTEvent::PrintObjectStatistics("End of event");
#endif

}
