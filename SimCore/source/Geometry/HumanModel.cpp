//
// Created by Zhang Yulei on 8/10/21.
//

#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include "Geometry/HumanModel.h"
#include "Control/Control.h"

void HumanModel::ImportModel(const G4String &model_name, G4Material *model_mat) {

    std::cout << "[Human Model] ==> Importing " << model_name << " ..." << std::endl;

    auto current_mesh = CADMesh::TessellatedMesh::FromSTL(pControl->human_model_directory + "/" + model_name + ".stl");
    current_mesh->SetScale(500);

    auto bunny_logical = new G4LogicalVolume(current_mesh->GetSolid(), model_mat, model_name + "_LV",
                                             nullptr, nullptr, nullptr);

    auto vis_attr = new G4VisAttributes(G4Color(1, 0, 0));
    vis_attr->SetVisibility(true);
    bunny_logical->SetVisAttributes(vis_attr);

    human_lv.insert(std::make_pair(model_name, bunny_logical));

    std::cout << "[Human Model] ==> Import " << model_name << " successfully..." << std::endl;
}

void HumanModel::BuildAll(G4LogicalVolume *world) {


    auto target_solid = new G4Box("Target_Region_Box", 0.95 * pControl->target_offset, 0.95 * pControl->target_offset,
                                  0.95 * pControl->target_offset);
    auto target_logical = new G4LogicalVolume(target_solid, pControl->world_material, "Target_LV",
                                              nullptr, nullptr, nullptr);
    new G4PVPlacement(nullptr, G4ThreeVector(), target_logical, "Target_PV", world, false, 0);
    auto vis_attr = new G4VisAttributes(G4Color(0, 0, 0));
    vis_attr->SetVisibility(false);
    target_logical->SetVisAttributes(vis_attr);

    for (const auto &part : pControl->human_model_composition) {
        ImportModel(part.first, part.second);
    }

    for (const auto &lv : human_lv) {
        new G4PVPlacement(nullptr, G4ThreeVector(), lv.second, lv.first + "_PV", target_logical, false, 0);
    }
}
