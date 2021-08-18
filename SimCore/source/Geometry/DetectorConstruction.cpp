
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "Geometry/DetectorConstruction.h"
#include "Control/Control.h"
#include "RootManager/RootManager.h"
#include "Digitization/DetectorSD.h"

#include "G4GDMLParser.hh"
#include "G4SolidStore.hh"
#include "G4UserLimits.hh"
#include "G4GeometryManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4PVParameterised.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SDManager.hh"

#include <iterator>
#include <filesystem>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction() {

    fCheckOverlaps = pControl->check_overlaps;
    fStepLimit = nullptr;
    hm = new HumanModel();
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

    if (pControl->build_ftrk) DefineDet(pControl->ftrk_name, PlaceType::Tracker);

    if (pControl->build_rtrk) DefineDet(pControl->rtrk_name, PlaceType::Tracker);

    if (pControl->build_scintillator) DefineDet(pControl->scintillator_name, PlaceType::ECAL); // Build Scintillator

    if (pControl->build_telescope) DefineDet(pControl->telescope_name, PlaceType::ECAL); // Build Range Telescope

    if (pControl->build_human) {
        hm->BuildAll(World_LV);
    } else {
        if (pControl->build_target) DefineTarget(); // Define Target
    }


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
                                                 G4NistManager::Instance()->FindMaterial("Water"),
                                                 "Target_Region_LV");
    new G4PVPlacement(nullptr, G4ThreeVector(), Target_Region_LV,
                      "Target_Region", World_LV, false, 0, fCheckOverlaps);

    auto vis_attr_r = new G4VisAttributes(G4Color(0, 0.3, 1, 0.25));
    vis_attr_r->SetVisibility(true);
    vis_attr_r->SetForceSolid();
    Target_Region_LV->SetVisAttributes(vis_attr_r);
}

void DetectorConstruction::DefineDet(const G4String &det_name, PlaceType type, bool if_parameterized) {

    // Some parameters
    auto box_size = std::get<0>(pControl->calo_info.at(det_name));
    auto box_arr = std::get<1>(pControl->calo_info.at(det_name));
    auto region_size = std::get<2>(pControl->calo_info.at(det_name));
    auto region_position = std::get<3>(pControl->calo_info.at(det_name));
    auto wrapper_size = pControl->wrapper_size;
    if (type == PlaceType::Tracker) wrapper_size = G4ThreeVector();

    auto extra_layer_no = 1;
    if (det_name == pControl->ftrk_name) extra_layer_no = static_cast<int> (pControl->ftrk_position.size());
    else if (det_name == pControl->rtrk_name) extra_layer_no = static_cast<int> (pControl->rtrk_position.size());

    G4Material *box_mat = nullptr;
    if (det_name == pControl->scintillator_name) box_mat = pControl->scintillator_material;
    else if (det_name == pControl->telescope_name) box_mat = pControl->telescope_material;
    else if (type == PlaceType::Tracker) box_mat = pControl->ftrk_material;

    G4Color color(0, 0, 0);
    if (det_name == pControl->scintillator_name) color = G4Color(0, 1, 0, 0.25);
    if (det_name == pControl->telescope_name) color = G4Color(1, 1, 0, 0.25);
    if (det_name == pControl->ftrk_name) color = G4Color(1, 0.5, 1, 0.5);
    if (det_name == pControl->rtrk_name) color = G4Color(1, 0.5, 1, 0.5);

    auto vis_attr = new G4VisAttributes(color);
    auto vis_attr_out = new G4VisAttributes(color);
    vis_attr_out->SetVisibility(false);

    // Build Calorimeter Region
    G4VSolid *Region_Box = new G4Box(det_name + "_Region_Box",
                                     region_size.x() / 2., region_size.y() / 2., region_size.z() / 2.
    );
    auto *Region_LV = new G4LogicalVolume(Region_Box, pControl->world_material,
                                          det_name + "_Region_LV");
    new G4PVPlacement(nullptr, region_position, Region_LV, det_name + "_Region", World_LV, false, 0, fCheckOverlaps);
    Region_LV->SetVisAttributes(vis_attr_out);

    // Build detailed structure
    // Out box containing wrapper and scintillator
    int total_No = static_cast<int>(box_arr.x() * box_arr.y() * box_arr.z() * extra_layer_no);
    G4VSolid *OUT_Box = new G4Box(det_name + "_Out_Box",
                                  box_size.x() / 2. + wrapper_size.x(),
                                  box_size.y() / 2. + wrapper_size.y(),
                                  box_size.z() / 2. + wrapper_size.z()
    );
    auto *OUT_LV = new G4LogicalVolume(OUT_Box, pControl->wrapper_material, det_name + "_Out_LV");
    OUT_LV->SetVisAttributes(vis_attr_out);
    // Then build scintillator inside box
    G4VSolid *Box = new G4Box(det_name + "_Box",
                              box_size.x() / 2., box_size.y() / 2., box_size.z() / 2.);

    auto *LV = new G4LogicalVolume(Box, box_mat, det_name + "_LV");
    if (type == PlaceType::ECAL) {
        new G4PVPlacement(nullptr, G4ThreeVector(), LV, det_name, OUT_LV, false, 0, fCheckOverlaps);
        new G4LogicalSkinSurface(det_name + "_WrapSkinSurface", LV, pControl->wrapper_surface);
    } else {
        delete OUT_Box;
        delete OUT_LV;
    }

    // Then do matrix placement to replicate
    auto Target_LV = (type == PlaceType::Tracker) ? LV : OUT_LV;
    auto *MP = new MatrixPlacement(det_name, type);
    if (if_parameterized) {
        new G4PVParameterised(det_name + "Out", Target_LV, Region_LV, kZAxis, total_No, MP, fCheckOverlaps);
    } else {
        for (auto cell_dict : pControl->det_dict.at(det_name)) {
            auto position = G4ThreeVector(
                    {std::get<0>(cell_dict.second), std::get<1>(cell_dict.second), std::get<2>(cell_dict.second)});

            new G4PVPlacement(nullptr, position, Target_LV, det_name + "Out", Region_LV, false, cell_dict.first,
                              fCheckOverlaps);
        }

    }
    LV_Storage.insert(make_pair(det_name, LV));

    vis_attr->SetVisibility(true);
    LV->SetVisAttributes(vis_attr);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void DetectorConstruction::DefineSD(const G4String &det_name, PlaceType type) {
    SDType sdt = (type == PlaceType::ECAL) ? SDType::Calorimeter : SDType::Tracker;

    auto *SD = new DetectorSD(det_name, sdt);
    G4SDManager::GetSDMpointer()->AddNewDetector(SD);
    LV_Storage.at(det_name)->SetSensitiveDetector(SD);
}

void DetectorConstruction::ConstructSDandField() {

    /*                              */
    /* Construct Sensitive Detector */
    /*                              */

    if (pControl->build_ftrk) DefineSD(pControl->ftrk_name, PlaceType::Tracker);

    if (pControl->build_rtrk) DefineSD(pControl->rtrk_name, PlaceType::Tracker);

    if (pControl->build_scintillator) DefineSD(pControl->scintillator_name, PlaceType::ECAL);

    if (pControl->build_telescope) DefineSD(pControl->telescope_name, PlaceType::ECAL);

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




