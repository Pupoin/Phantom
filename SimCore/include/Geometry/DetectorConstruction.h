
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

#include <vector>


class DetectorConstruction : public G4VUserDetectorConstruction {
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

    void DefineParameters();

    void DefineWorld();

    void DefineDet(const G4String& det_name, PlaceType type);

    void DefineSD(const G4String& det_name, PlaceType type);


    G4LogicalVolume *World_LV{}; // Logical Volume of World.
    G4PVPlacement *World_PV{}; // Placement (Phyisical Volume) of the World.

    map<G4String, G4LogicalVolume*> LV_Storage;

    //global option
    G4UserLimits *fStepLimit; // pointer to user step limits
    G4bool fCheckOverlaps;   // option to activate checking of volumes overlaps
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
