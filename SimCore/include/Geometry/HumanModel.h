//
// Created by Zhang Yulei on 8/10/21.
//

#ifndef PCTSIMULATION_HUMANMODEL_H
#define PCTSIMULATION_HUMANMODEL_H

#include "G4String.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"

#include "CADMesh.hh"

#include <map>

using std::map;

class HumanModel {
public:

    void BuildAll(G4LogicalVolume* world);

    void ImportModel(const G4String& model_name, G4Material* model_mat);

private:

    std::map<G4String, G4LogicalVolume* > human_lv;
};


#endif //PCTSIMULATION_HUMANMODEL_H
