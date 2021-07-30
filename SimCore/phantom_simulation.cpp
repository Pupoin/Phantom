//
// Created by Zhang Yulei on 7/30/21.
//

/* Geant4 Libraries */
#include "FTFP_BERT.hh"
#include "G4RunManager.hh"
#include "G4Types.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

#include "Randomize.hh"

#include "Control/Control.h"
#include "RootManager/RootManager.h"
#include "Geometry/DetectorConstruction.h"
#include "UserAction/RunAction.h"
#include "UserAction/EventAction.h"
#include "UserAction/PrimaryGeneratorAction.h"
#include "UserAction/TrackingAction.h"
#include "UserAction/SteppingAction.h"

namespace {
    void PrintUsage() {
        G4cerr << " Usage: " << G4endl;
        G4cerr << " PSim [-y yaml.file]" << G4endl;
        G4cerr << "   note: yaml file is necessary." << G4endl;
        G4cerr << G4endl;
    }
} // namespace

int main(int argc, char **argv) {

    // Get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    // Choose the Random engine
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

    // Initiate Control Class
    Control::CreateInstance();

    //-------------------------------------------------------------------------------
    // Evaluate Arguments

    if (argc > 9) {
        PrintUsage();
        return 1;
    }

    bool vis_mode = false;
    G4String yamlFileName;
    for (G4int i = 1; i < argc; i = i + 2) {
        if (G4String(argv[i]) == "-y")
            yamlFileName = argv[i + 1];
        else if (G4String(argv[i]) == "-v")
            vis_mode = true;
        else {
            PrintUsage();
            return 1;
        }
    }

    // Read Configuration from YAML
    auto yaml_valid = pControl->ReadYAML(yamlFileName);
    if (!yaml_valid) {
        std::cerr << "[Read YAML] ==> Reading Error from YAML file: " << std::endl;
        return -1;
    }
    // Rebuild all dependent variables
    // All the parameters are locked for now
    pControl->RebuildVariables();

    // Initiate RootManager Class
    RootManager::CreateInstance();

    // Construct the default run manager
    auto *runManager = new G4RunManager;

    // Set mandatory initialization classes
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new FTFP_BERT);

    // Set user action classes
    runManager->SetUserAction(new RunAction());
    runManager->SetUserAction(new EventAction());
    runManager->SetUserAction(new PrimaryGeneratorAction());
    runManager->SetUserAction(new TrackingAction());
    runManager->SetUserAction(new SteppingAction());

    // Initialize G4 kernel
    runManager->Initialize();

    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();

    if (vis_mode) { // interactive mode : define UI session
        auto *ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        if (ui->IsGUI())
            UImanager->ApplyCommand("/control/execute gui.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;

    return 0;
}
