
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "Geometry/DetectorConstruction.h"
#include "Control/Control.h"
#include "RootManager/RootManager.h"

#include "G4GDMLParser.hh"
#include "G4SolidStore.hh"
#include "G4UserLimits.hh"
#include "G4GeometryManager.hh"
#include "G4GeometryTolerance.hh"

#include <iterator>
#include <cstdio>
#include <filesystem>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction() {

    fCheckOverlaps = pControl->check_overlaps;
    fStepLimit = nullptr;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::Construct() {
    DefineParameters();
    return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineParameters() {

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::DefineVolumes() {

    DefineWorld();      // Build World

    // Book RootMng
    pRootMng->book();
    G4cout << "[Root Manager] ==> Root Manager initialized ..." << G4endl;
    G4cout << "[Root Manager] ==> Output File " << pControl->outfile_Name << " created ..." << G4endl;

    // Save Geometry
    if (pControl->save_geometry) SaveGeometry();

    // Set User Limit
    G4double maxStep = 10 * mm;
    fStepLimit = new G4UserLimits(maxStep, DBL_MAX, 200 * s);

    return World_PV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineWorld() {

    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(pControl->Size_World.z());

    G4cout << "Computed tolerance = "
           << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() / mm
           << " mm" << G4endl;

    auto World_Box = new G4Box("World_Box", pControl->Size_World.x() / 2, pControl->Size_World.y() / 2,
                               pControl->Size_World.z() / 2); // Solid of World.
    World_LV = new G4LogicalVolume(World_Box, pControl->World_Mat, "World_LV");
    World_PV = new G4PVPlacement(nullptr, G4ThreeVector(), World_LV, "World", nullptr, false, 0, fCheckOverlaps);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineTarget() {


}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField() {

    /*                              */
    /* Construct Sensitive Detector */
    /*                              */

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SaveGeometry() {

    /* Save GDML to ROOT File */
    auto filename = "geometry.gdml";
//    if (std::filesystem::exists(filename))
//        std::remove(filename);

    G4GDMLParser parser;
    parser.Write(filename, World_PV);

    pRootMng->FillGeometry(filename);

}
