
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "Geometry/DetectorConstruction.h"
#include "Control/Control.h"
#include "RootManager/RootManager.h"
#include "Digitization/DetectorSD.h"

#include "G4NistManager.hh"
#include "G4GDMLParser.hh"
#include "G4SolidStore.hh"
#include "G4UserLimits.hh"
#include "G4GeometryManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4PVParameterised.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SDManager.hh"
// start, dicom 
#include "DicomRegularDetectorConstruction.hh"
#include "DicomNestedParamDetectorConstruction.hh"
#include "DicomPartialDetectorConstruction.hh"
#include "DicomDetectorConstruction.hh"
#include "DicomPhantomParameterisationColour.hh"
// #include "Dicom2ActionInitialization.hh"
#include "DicomIntersectVolume.hh"
#include "DicomFileMgr.hh"
// end of dicom
#include <iterator>
#include <filesystem>
#include "G4MultiFunctionalDetector.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PSDoseDeposit3D.hh"


//....oooOO0OOooo./.......oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::DetectorConstruction() : DicomDetectorConstruction()
{
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

// override derived from dicom
void DetectorConstruction::ConstructPhantom()
{
#ifdef G4VERBOSE
  G4cout << "DicomRegularDetectorConstruction::ConstructPhantom " << G4endl;
#endif

  //----- Create parameterisation 
  DicomPhantomParameterisationColour* param = new DicomPhantomParameterisationColour();

  //----- Set voxel dimensions
  param->SetVoxelDimensions( fVoxelHalfDimX, fVoxelHalfDimY, fVoxelHalfDimZ );

  //----- Set number of voxels 
  param->SetNoVoxel( fNVoxelX, fNVoxelY, fNVoxelZ );

  //----- Set list of materials
  param->SetMaterials( fMaterials ); 

  //----- Set list of material indices: for each voxel it is a number that
  // correspond to the index of its material in the vector of materials
  // defined above
  param->SetMaterialIndices( fMateIDs );

  //----- Define voxel logical volume
  G4Box* voxel_solid = new G4Box( "Voxel", fVoxelHalfDimX, fVoxelHalfDimY, fVoxelHalfDimZ);
  G4LogicalVolume* voxel_logic = new G4LogicalVolume(voxel_solid,fMaterials[0],
                                                    "VoxelLogical", 0,0,0);
  // material is not relevant, it will be changed by the
  // ComputeMaterial method of the parameterisation
    voxel_logic->SetVisAttributes(
                     new G4VisAttributes(G4VisAttributes::GetInvisible()));
    
  //--- Assign the fContainer volume of the parameterisation
  param->BuildContainerSolid(fContainer_phys);

  //--- Assure yourself that the voxels are completely filling the 
  // fContainer volume
  param->CheckVoxelsFillContainer( fContainer_solid->GetXHalfLength(), 
                                   fContainer_solid->GetYHalfLength(), 
                                   fContainer_solid->GetZHalfLength() );

  //----- The G4PVParameterised object that uses the created parameterisation
  // should be placed in the fContainer logical volume
  G4PVParameterised * phantom_phys = 
    new G4PVParameterised("phantom",voxel_logic,fContainer_logic,
                          kXAxis, fNVoxelX*fNVoxelY*fNVoxelZ, param);
  // if axis is set as kUndefined instead of kXAxis, GEANT4 will 
  //  do an smart voxel optimisation 
  // (not needed if G4RegularNavigation is used)

  //----- Set this physical volume as having a regular structure of type 1, 
  // so that G4RegularNavigation is used
  phantom_phys->SetRegularStructureId(1); // if not set, G4VoxelNavigation
  //will be used instead 

  SetScorer(voxel_logic);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
        if (pControl->build_target) 
        // DefineTarget(); // Define Target
        {
            // define organism, such as lobster etc.
            fConstructed = true;
            InitialisationOfMaterials();
            ReadPhantomDataNew();
            ConstructPhantomContainerNew();
            ConstructPhantom();
            std::cout << __LINE__ << " "
                      << "000000000 ----------------" << std::endl;
        }
    }

    //////////// ready for check.
    // Book RootMng
    // pRootMng->book();
    // G4cout << "[Root Manager] ==> Root Manager initialized ..." << G4endl;
    // G4cout << "[Root Manager] ==> Output File " << pControl->outfile_Name << " created ..." << G4endl;

    // // Save Geometry
    // if (pControl->save_geometry) SaveGeometry();
    //         std::cout <<__LINE__<< " " << "000000000 ----------------" << std::endl;

    // // Set User Limit
    // // G4double maxStep = 1 * mm;
    // // fStepLimit = new G4UserLimits(maxStep, DBL_MAX, 20 * s);
    //         std::cout <<__LINE__<< " " << "000000000 ----------------" << std::endl;

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
    fWorld_logic = World_LV;
    World_PV = new G4PVPlacement(nullptr, G4ThreeVector(), fWorld_logic, "World", nullptr, false, 0, fCheckOverlaps);

    ////////////     another world  for test ///////////////////////////////
    //----- Build world
    // G4double worldXDimension = 1.*m;
    // G4double worldYDimension = 1.*m;
    // G4double worldZDimension = 1.*m;
    // fWorld_solid = new G4Box( "WorldSolid", worldXDimension, worldYDimension, worldZDimension );
    // fWorld_logic = new G4LogicalVolume( fWorld_solid, fAir, "WorldLogical", 0, 0, 0 );
    // World_PV = new G4PVPlacement( 0, G4ThreeVector(0,0,0), "World", fWorld_logic,
    //                                 0, false, 0 );
    //  fWorld_phys                           

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineTarget() {
    // Build Target Region
    auto *Target_Region_Box = new G4Box("Target_Region_Box", pControl->target_size.x() / 2.,
                                        pControl->target_size.y() / 2., pControl->target_size.z() / 2.);
//    auto *Target_Region_Box = new G4Sphere("Target_Region_Box", 0., pControl->target_size.z(), 0, 2 * TMath::Pi(), 0.,
//                                           TMath::Pi());
    auto *Target_Region_LV = new G4LogicalVolume(Target_Region_Box,
                                                 pControl->target_material,
                                                 "Target_Region_LV");
    new G4PVPlacement(nullptr, G4ThreeVector(), Target_Region_LV,
                      "Target_Region", World_LV, false, 0, fCheckOverlaps);

    auto vis_attr_r = new G4VisAttributes(G4Color(0, 0.3, 1, 0.25));
    vis_attr_r->SetVisibility(true);
    vis_attr_r->SetForceSolid();
    Target_Region_LV->SetVisAttributes(vis_attr_r);
    // build center box
    // Build Target Region
    auto *Target_Center_Box = new G4Box("Target_Center_Box", 1.5 * mm, 2 * mm, 2.5 * mm);
    auto *Target_Center_LV = new G4LogicalVolume(Target_Center_Box, G4NistManager::Instance()->FindMaterial("G4_Ca"),
                                                 "Target_Center_LV");
    new G4PVPlacement(nullptr, G4ThreeVector(0., 0., +2 * CLHEP::mm), Target_Center_LV,
                      "Target_Center", Target_Region_LV, false, 0, fCheckOverlaps);
    auto vis_attr_c = new G4VisAttributes(G4Color(0.5, 0.3, 1, 0.55));
    vis_attr_c->SetVisibility(true);
    vis_attr_c->SetForceSolid();
    Target_Center_LV->SetVisAttributes(vis_attr_c);

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
        for (auto cell_dict: pControl->det_dict.at(det_name)) {
            auto position = G4ThreeVector(
                    {std::get<0>(cell_dict.second), std::get<1>(cell_dict.second), std::get<2>(cell_dict.second)});
            // if(det_name == "FrontTracker")
            // G4cout << "daadfadfadfapositionaaa " << position.x() << " "
            //        << position.y() << " "
            //        << position.z() << " " << G4endl;

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
    
    // set the dicom/raw body to be the Sensitive Detectors
    // // Sensitive Detector Name
    // G4String concreteSDname = "phantomSD";
    // std::vector<G4String> scorer_names;
    // scorer_names.push_back(concreteSDname);
    // //------------------------
    // // MultiFunctionalDetector
    // //------------------------
    // //
    // // Define MultiFunctionalDetector with name.
    // // declare MFDet as a MultiFunctionalDetector scorer
    // G4MultiFunctionalDetector* MFDet = 
    //     new G4MultiFunctionalDetector(concreteSDname);
    // G4SDManager::GetSDMpointer()->AddNewDetector( MFDet );
    // //G4VPrimitiveScorer* dosedep = new G4PSDoseDeposit("DoseDeposit");
    // G4VPrimitiveScorer* dosedep = 
    //     new G4PSDoseDeposit3D("DoseDeposit", fNVoxelX, fNVoxelY, fNVoxelZ);
    // MFDet->RegisterPrimitive(dosedep);
    
    // for(std::set<G4LogicalVolume*>::iterator ite = fScorers.begin(); 
    //     ite != fScorers.end(); ++ite) {
    //     SetSensitiveDetector(*ite, MFDet);
    // }
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




