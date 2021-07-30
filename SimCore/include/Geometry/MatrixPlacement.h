//
// Created by Zhang Yulei on 7/30/21.
//

#ifndef PHANTOM_MATRIXPLACEMENT_H
#define PHANTOM_MATRIXPLACEMENT_H

#include "G4VPVParameterisation.hh"

#include "Control/Control.h"

#include "G4String.hh"


enum class PlaceType {
    NotDefined, Tracker, ECAL
};

class MatrixPlacement : public G4VPVParameterisation {
public:
    MatrixPlacement(G4String  name, PlaceType type);

    ~MatrixPlacement() override = default;

    void ComputeTransformation(G4int copyNo, G4VPhysicalVolume *physVol) const override;

    void BuildCopyNoTable();

    static copy_dictionary BuildCaloTable(const G4ThreeVector& arr, const G4ThreeVector& size);

    static copy_dictionary BuildTrkTable(const G4ThreeVector& arr, const G4ThreeVector& size, const vector<G4ThreeVector>& Cen_Pos);

    // void ComputeDimensions(G4Box &trackerLayer, G4int copyNo, const G4VPhysicalVolume *physVol) const override;

private:
    PlaceType det_type = PlaceType::NotDefined;
    G4String det_name;

    G4ThreeVector Arrangement;
};


#endif //PHANTOM_MATRIXPLACEMENT_H
