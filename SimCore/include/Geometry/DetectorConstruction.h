
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UniformMagField.hh"
#include "G4PVPlacement.hh"

#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "Geometry/MatrixPlacement.h"
#include "Geometry/HumanModel.h"
// start, dicom 
#include "DicomRegularDetectorConstruction.hh"
#include "DicomNestedParamDetectorConstruction.hh"
#include "DicomPartialDetectorConstruction.hh"
// #include "Dicom2ActionInitialization.hh"
#include "DicomIntersectVolume.hh"
#include "DicomDetectorConstruction.hh"
#include "DicomFileMgr.hh"
// end of dicom

#include <vector>


class DetectorConstruction : public DicomDetectorConstruction {
public:
    explicit DetectorConstruction();

    ~DetectorConstruction() override;

    void ConstructSDandField() override; // build SD in run action

public:
    G4VPhysicalVolume *Construct() override;

    /// \brief Save Geometry to ROOT file
    void SaveGeometry();

    void DefineTarget();

private:
    // methods
    G4VPhysicalVolume *DefineVolumes();

    void ConstructPhantom() override;
    // void ConstructPhantomContainerNew();

    void DefineParameters();

    void DefineWorld();

    void DefineDet(const G4String &det_name, PlaceType type, bool if_parameterized = false);

    void DefineSD(const G4String &det_name, PlaceType type);

    G4LogicalVolume *World_LV{}; // Logical Volume of World.
    G4PVPlacement *World_PV{}; // Placement (Phyisical Volume) of the World.

    map<G4String, G4LogicalVolume *> LV_Storage;

    //global option
    G4UserLimits *fStepLimit; // pointer to user step limits
    G4bool fCheckOverlaps;   // option to activate checking of volumes overlaps

    // Human Building
    HumanModel *hm;
    // for dicom targets
    // DicomDetectorConstruction *dicom_target;
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
