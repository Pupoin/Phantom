//
// Created by Zhang Yulei on 7/30/21.
//

#include "Geometry/MatrixPlacement.h"

#include "G4VPhysicalVolume.hh"

#include <utility>
#include <algorithm>

using std::get;

void MatrixPlacement::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume *physVol) const {
    auto position = pControl->det_dict.at(det_name).at(copyNo);
    G4ThreeVector origin(get<0>(position), get<1>(position), get<2>(position));
    physVol->SetTranslation(origin);
    physVol->SetRotation(nullptr);

}

MatrixPlacement::MatrixPlacement(G4String name, PlaceType type) :
        det_type(type), det_name(std::move(name)) {

    BuildCopyNoTable();
}

void MatrixPlacement::BuildCopyNoTable() {

    G4ThreeVector arrangement;
    G4ThreeVector cell_size;
    if (det_type == PlaceType::ECAL) {
        if (det_name == "Scintillator") {
            arrangement = pControl->scintillator_arrangement;
            cell_size = pControl->scintillator_size;
        } else if (det_name == "Telescope") {
            arrangement = pControl->telescope_arrangement;
            cell_size = pControl->telescope_size;
        }
        pControl->det_dict.insert(std::make_pair(det_name, BuildCaloTable(arrangement, cell_size)));
    } else if (det_type == PlaceType::Tracker) {
        vector<G4ThreeVector> center_position;
        if (det_name == "FrontTracker") {
            arrangement = pControl->ftrk_strip_arrangement;
            cell_size = pControl->ftrk_strip_size;
            center_position = pControl->ftrk_position;
        } else if (det_name == "RearTracker") {
            arrangement = pControl->rtrk_strip_arrangement;
            cell_size = pControl->rtrk_strip_size;
            center_position = pControl->rtrk_position;
        }
        pControl->det_dict.insert(std::make_pair(det_name, BuildTrkTable(arrangement, cell_size, center_position)));
    }


}

copy_dictionary MatrixPlacement::BuildCaloTable(const G4ThreeVector &arr, const G4ThreeVector &size) {

    copy_dictionary dict;

    auto xNo = arr.x();
    auto yNo = arr.y();
    auto zNo = arr.z();

    auto UnitXHalfLength = size.x() / 2;
    auto UnitYHalfLength = size.y() / 2;
    auto UnitZHalfLength = size.z() / 2;

    auto TotalHalfSize = G4ThreeVector(xNo * UnitXHalfLength,
                                       yNo * UnitYHalfLength,
                                       zNo * UnitZHalfLength);

    double UnitPosX, UnitPosY, UnitPosZ;
    int CopyNo = 0;
    for (int k = 0; k < zNo; k++) {
        for (int j = 0; j < yNo; j++) {
            for (int i = 0; i < xNo; i++) {
                UnitPosX = -1. * TotalHalfSize.x() + (2 * i + 1) * UnitXHalfLength;
                UnitPosY = -1. * TotalHalfSize.y() + (2 * j + 1) * UnitYHalfLength;
                UnitPosZ = -1. * TotalHalfSize.z() + (2 * k + 1) * UnitZHalfLength;

                dict.insert(std::make_pair(CopyNo, std::make_tuple(UnitPosX, UnitPosY, UnitPosZ)));

                CopyNo++;
            }
        }
    }
    return dict;
}

copy_dictionary MatrixPlacement::BuildTrkTable(const G4ThreeVector &arr, const G4ThreeVector &size,
                                               const vector<G4ThreeVector> &Cen_Pos) {
    copy_dictionary dict;

    auto xNo = arr.x();
    auto yNo = arr.y();
    auto zNo = arr.z();

    auto UnitXHalfLength = size.x() / 2;
    auto UnitYHalfLength = size.y() / 2;
    auto UnitZHalfLength = size.z() / 2;

    auto TotalHalfSize = G4ThreeVector(xNo * UnitXHalfLength,
                                       yNo * UnitYHalfLength,
                                       zNo * UnitZHalfLength);

    int CopyNo = 0;
    double UnitPosX, UnitPosY, UnitPosZ;
    for (const auto &c_pos: Cen_Pos) {
        for (int k = 0; k < zNo; k++) {
            for (int j = 0; j < yNo; j++) {
                for (int i = 0; i < xNo; i++) {
                    UnitPosX = -1. * TotalHalfSize.x() + (2 * i + 1) * UnitXHalfLength + c_pos.x();
                    UnitPosY = -1. * TotalHalfSize.y() + (2 * j + 1) * UnitYHalfLength + c_pos.y();
                    UnitPosZ = -1. * TotalHalfSize.z() + (2 * k + 1) * UnitZHalfLength + c_pos.z();

                    dict.insert(std::make_pair(CopyNo, std::make_tuple(UnitPosX, UnitPosY, UnitPosZ)));

                    CopyNo++;
                }
            }
        }
    }

    return dict;
}
