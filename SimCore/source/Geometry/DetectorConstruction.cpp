
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "Geometry/DetectorConstruction.h"
#include "Control/Control.h"
#include "RootManager/RootManager.h"
#include "Geometry/MatrixPlacement.h"

#include "G4GDMLParser.hh"
#include "G4SolidStore.hh"
#include "G4UserLimits.hh"
#include "G4GeometryManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4PVParameterised.hh"
#include "G4VisAttributes.hh"

#include <iterator>
#include <filesystem>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction() {

    fCheckOverlaps = pControl->check_overlaps;
    fStepLimit = nullptr;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() = default;

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

    if (pControl->build_ftrk) DefineFrontTracker();

    if (pControl->build_rtrk) DefineRearTracker();

    if (pControl->build_scintillator) DefineScintillator(); // Build Scintillator

    if (pControl->build_telescope) DefineTelescope(); // Build Range Telescope

    if (pControl->build_target) DefineTarget(); // Define Target

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

    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(pControl->world_region_size.z());

    G4cout << "Computed tolerance = "
           << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() / mm
           << " mm" << G4endl;

    auto World_Box = new G4Box("World_Box", pControl->world_region_size.x() / 2, pControl->world_region_size.y() / 2,
                               pControl->world_region_size.z() / 2); // Solid of World.
    World_LV = new G4LogicalVolume(World_Box, pControl->world_material, "World_LV");
    World_PV = new G4PVPlacement(nullptr, G4ThreeVector(), World_LV, "World", nullptr, false, 0, fCheckOverlaps);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineTarget() {
    // Build Target Region
    G4VSolid *Target_Region_Box = new G4Box("Target_Region_Box", 8 * cm, 8 * cm, 8 * cm);
    auto *Target_Region_LV = new G4LogicalVolume(Target_Region_Box,
                                                 G4NistManager::Instance()->FindOrBuildMaterial("G4_Water"),
                                                 "Target_Region_LV");
    new G4PVPlacement(nullptr, G4ThreeVector(), Target_Region_LV,
                      "Target_Region", World_LV, false, 0, fCheckOverlaps);

    auto vis_attr_r = new G4VisAttributes(G4Color(0, 0.3, 1));
    vis_attr_r->SetVisibility(true);
    Target_Region_LV->SetVisAttributes(vis_attr_r);
}

void DetectorConstruction::DefineFrontTracker() {

    // Build FrontTracker Region
    G4VSolid *FrontTracker_Region_Box = new G4Box("FrontTracker_Region_Box",
                                                  pControl->ftrk_region_size.x() / 2.,
                                                  pControl->ftrk_region_size.y() / 2.,
                                                  pControl->ftrk_region_size.z() / 2.
    );
    auto *FrontTracker_Region_LV = new G4LogicalVolume(FrontTracker_Region_Box, pControl->world_material,
                                                       "FrontTracker_Region_LV");
    new G4PVPlacement(nullptr, pControl->ftrk_region_position, FrontTracker_Region_LV,
                      "FrontTracker_Region", World_LV, false, 0, fCheckOverlaps);

    auto vis_attr_r = new G4VisAttributes(G4Color(1, 1, 0));
    vis_attr_r->SetVisibility(true);
    FrontTracker_Region_LV->SetVisAttributes(vis_attr_r);

    // Build FrontTracker Cell
    int total_No = static_cast<int>(pControl->ftrk_strip_arrangement.x() *
                                    pControl->ftrk_strip_arrangement.y() *
                                    pControl->ftrk_strip_arrangement.z())
                   * static_cast<int> (pControl->ftrk_position.size());
    G4VSolid *FrontTracker_Box = new G4Box("FrontTracker_Box",
                                           pControl->ftrk_strip_size.x() / 2.,
                                           pControl->ftrk_strip_size.y() / 2.,
                                           pControl->ftrk_strip_size.z() / 2.
    );
    auto *FrontTracker_LV = new G4LogicalVolume(FrontTracker_Box, pControl->ftrk_material, "FrontTracker_LV");
    auto *FrontTracker_MP = new MatrixPlacement("FrontTracker", PlaceType::Tracker);
    new G4PVParameterised("FrontTracker", FrontTracker_LV, FrontTracker_Region_LV, kZAxis, total_No, FrontTracker_MP);

    auto vis_attr = new G4VisAttributes(G4Color(1, 1, 0));
    vis_attr->SetVisibility(true);
    FrontTracker_LV->SetVisAttributes(vis_attr);
}

void DetectorConstruction::DefineRearTracker() {

    // Build RearTracker Region
    G4VSolid *RearTracker_Region_Box = new G4Box("RearTracker_Region_Box",
                                                 pControl->rtrk_region_size.x() / 2.,
                                                 pControl->rtrk_region_size.y() / 2.,
                                                 pControl->rtrk_region_size.z() / 2.
    );
    auto *RearTracker_Region_LV = new G4LogicalVolume(RearTracker_Region_Box, pControl->world_material,
                                                      "RearTracker_Region_LV");
    new G4PVPlacement(nullptr, pControl->rtrk_region_position, RearTracker_Region_LV,
                      "RearTracker_Region", World_LV, false, 0, fCheckOverlaps);

    auto vis_attr_r = new G4VisAttributes(G4Color(1, 1, 0));
    vis_attr_r->SetVisibility(true);
    RearTracker_Region_LV->SetVisAttributes(vis_attr_r);

    // Build RearTracker Cell
    int total_No = static_cast<int>(pControl->rtrk_strip_arrangement.x() *
                                    pControl->rtrk_strip_arrangement.y() *
                                    pControl->rtrk_strip_arrangement.z())
                   * static_cast<int> (pControl->rtrk_position.size());
    G4VSolid *RearTracker_Box = new G4Box("RearTracker_Box",
                                          pControl->rtrk_strip_size.x() / 2.,
                                          pControl->rtrk_strip_size.y() / 2.,
                                          pControl->rtrk_strip_size.z() / 2.
    );
    auto *RearTracker_LV = new G4LogicalVolume(RearTracker_Box, pControl->rtrk_material, "RearTracker_LV");
    auto *RearTracker_MP = new MatrixPlacement("RearTracker", PlaceType::Tracker);
    new G4PVParameterised("RearTracker", RearTracker_LV, RearTracker_Region_LV, kZAxis, total_No, RearTracker_MP);

    auto vis_attr = new G4VisAttributes(G4Color(1, 1, 0));
    vis_attr->SetVisibility(true);
    RearTracker_LV->SetVisAttributes(vis_attr);
}

void DetectorConstruction::DefineScintillator() {

    auto vis_attr = new G4VisAttributes(G4Color(0, 1, 0));

    // Build Scintillator Region
    G4VSolid *Scintillator_Region_Box = new G4Box("Scintillator_Region_Box",
                                                  pControl->scintillator_region_size.x() / 2.,
                                                  pControl->scintillator_region_size.y() / 2.,
                                                  pControl->scintillator_region_size.z() / 2.
    );
    auto *Scintillator_Region_LV = new G4LogicalVolume(Scintillator_Region_Box, pControl->world_material,
                                                       "Scintillator_Region_LV");
    new G4PVPlacement(nullptr, pControl->scintillator_region_position, Scintillator_Region_LV,
                      "Scintillator_Region", World_LV, false, 0, fCheckOverlaps);

    //vis_attr->SetVisibility(true);
    //Scintillator_Region_LV->SetVisAttributes(vis_attr);


    // Build Scintillator Cell
    int total_No = static_cast<int>(pControl->scintillator_arrangement.x() *
                                    pControl->scintillator_arrangement.y() *
                                    pControl->scintillator_arrangement.z());
    G4VSolid *Scintillator_Box = new G4Box("Scintillator_Box",
                                           pControl->scintillator_size.x() / 2.,
                                           pControl->scintillator_size.y() / 2.,
                                           pControl->scintillator_size.z() / 2.
    );
    auto *Scintillator_LV = new G4LogicalVolume(Scintillator_Box, pControl->scintillator_material, "Scintillator_LV");
    auto *Scintillator_MP = new MatrixPlacement("Scintillator", PlaceType::ECAL);
    new G4PVParameterised("Scintillator", Scintillator_LV, Scintillator_Region_LV, kZAxis, total_No, Scintillator_MP);

    vis_attr->SetVisibility(true);
    Scintillator_LV->SetVisAttributes(vis_attr);
}


void DetectorConstruction::DefineTelescope() {

    auto vis_attr = new G4VisAttributes(G4Color(1, 0, 0.5));

    // Build Range Telescope Region
    G4VSolid *Telescope_Region_Box = new G4Box("Telescope_Region_Box",
                                               pControl->telescope_region_size.x() / 2.,
                                               pControl->telescope_region_size.y() / 2.,
                                               pControl->telescope_region_size.z() / 2.
    );
    auto *Telescope_Region_LV = new G4LogicalVolume(Telescope_Region_Box, pControl->world_material,
                                                    "Telescope_Region_LV");
    new G4PVPlacement(nullptr, pControl->telescope_region_position, Telescope_Region_LV,
                      "Telescope_Region", World_LV, false, 0, fCheckOverlaps);

//    vis_attr->SetVisibility(true);
//    Telescope_Region_LV->SetVisAttributes(vis_attr);

    // Build Range Telescope Cell
    int total_No = static_cast<int>(pControl->telescope_arrangement.x() *
                                    pControl->telescope_arrangement.y() *
                                    pControl->telescope_arrangement.z());
    G4VSolid *Telescope_Box = new G4Box("Telescope_Box",
                                        pControl->telescope_size.x() / 2.,
                                        pControl->telescope_size.y() / 2.,
                                        pControl->telescope_size.z() / 2.
    );
    auto *Telescope_LV = new G4LogicalVolume(Telescope_Box, pControl->telescope_material, "Telescope_LV");
    auto *Telescope_MP = new MatrixPlacement("Telescope", PlaceType::ECAL);
    new G4PVParameterised("Telescope", Telescope_LV, Telescope_Region_LV, kZAxis, total_No, Telescope_MP);

    vis_attr->SetVisibility(true);
    Telescope_LV->SetVisAttributes(vis_attr);
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


