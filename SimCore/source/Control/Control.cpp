//
// Created by Zhang Yulei on 12/31/20.
//

#include "Control/Control.h"

// Geant4
#include "G4UnitsTable.hh"

using std::string;
using std::cout, std::cerr, std::endl;

// Required by Singleton
Control *pControl = nullptr;

// Get Instance Class
Control *Control::CreateInstance() {
    if (pControl == nullptr)
        pControl = new Control();

    return pControl;
}

Control::Control() {

    // Initialize G4 Material Manager
    nistManager = G4NistManager::Instance();

}

bool Control::ReadYAML(const G4String &file_in) {
    G4String infile;
    if (file_in.empty()) infile = "default.yaml";
    else infile = file_in;
    auto Node = YAML::LoadFile(infile);

    try {
        material_file = Node["material_file"].as<string>(); // should be defined explicitly
    } catch (YAML::InvalidNode &e) {
        cerr << "[Read YAML] ==> Error reading material file" << endl;
        exit(EXIT_FAILURE);
    }
    BuildMaterialTable(material_file);

    try {
        //========================================
        /* Global Variables */
        //----------------------------------------

        // Default value is false
        save_geometry = (Node["save_geometry"].IsDefined()) && Node["save_geometry"].as<bool>();
        check_overlaps = (Node["check_overlaps"].IsDefined()) && Node["check_overlaps"].as<bool>();
        memory_check = (Node["memory_check"].IsDefined()) && Node["memory_check"].as<bool>();

        //----------------------------------------
        // Root Manager Options
        if (auto n = Node["RootManager"]; n.IsDefined()) {
            outfile_Name = n["outfile_Name"].as<string>();
            tree_Name = n["tree_Name"].as<string>();
            Run_Number = n["Run_Number"].as<int>();
            Total_Event_Number = n["Total_Event_Number"].as<int>();
        }
        //----------------------------------------
        // Out Collection Options
        if (auto n = Node["OutCollection"]; n.IsDefined()) {
            save_all_mcp = n["save_all_mcp"].as<bool>();
            MCParticle_Name = n["MCParticle_Name"].as<string>();
            ParticleStep_Name = n["ParticleStep_Name"].as<string>();
        }

        //========================================
        /* Geometry */
        //----------------------------------------
        {
            auto n = Node["Geometry"];
            build_target = !(n["build_target"].IsDefined()) || n["build_target"].as<bool>();
            build_ftrk = !(n["build_ftrk"].IsDefined()) || n["build_ftrk"].as<bool>();
            build_rtrk = !(n["build_rtrk"].IsDefined()) || n["build_rtrk"].as<bool>();
            build_scintillator = !(n["build_scintillator"].IsDefined()) || n["build_scintillator"].as<bool>();
            build_telescope = !(n["build_telescope"].IsDefined()) || n["build_telescope"].as<bool>();
        }
        if (auto n = Node["Geometry"]; n.IsDefined()) {
            target_offset = readV2(n["target_offset"]);

            world_material = nistManager->FindMaterial(n["world_material"].as<string>());

            // Tracker
            // --------
            // Front Tracker
            ftrk_material = nistManager->FindMaterial(n["ftrk_material"].as<string>());
            ftrk_strip_size = readV3(n["ftrk_strip_size"], true);
            ftrk_strip_arrangement = readV3(n["ftrk_strip_arrangement"], false);
            for (auto pos : n["ftrk_position"]) {
                ftrk_position.emplace_back(readV3(pos, true));
            }
            // --------
            // Rear Tracker
            rtrk_material = nistManager->FindMaterial(n["rtrk_material"].as<string>());
            rtrk_strip_size = readV3(n["rtrk_strip_size"], true);
            rtrk_strip_arrangement = readV3(n["rtrk_strip_arrangement"], false);
            for (auto pos : n["rtrk_position"]) {
                rtrk_position.emplace_back(readV3(pos, true));
            }

            // Calorimeter
            // material reference: https://www.crystals.saint-gobain.com/products/bc-408-bc-412-bc-416
            // --------
            // holder
            holder_material = nistManager->FindMaterial(n["holder_material"].as<string>());
            holder_gap = readV2(n["holder_gap"]);
            // --------
            // wrapper
            wrapper_material = nistManager->FindMaterial(n["wrapper_material"].as<string>());
            wrapper_size = readV3(n["wrapper_size"], true);
            // --------
            // Scintillator
            scintillator_material = nistManager->FindMaterial(n["scintillator_material"].as<string>());
            scintillator_size = readV3(n["scintillator_size"], true);
            scintillator_arrangement = readV3(n["scintillator_arrangement"]);
            scintillator_offset = readV2(n["scintillator_offset"]);
            // --------
            // Range Telescope
            telescope_material = nistManager->FindMaterial(n["telescope_material"].as<string>());
            telescope_size = readV3(n["telescope_size"], true);
            telescope_arrangement = readV3(n["telescope_arrangement"]);
            telescope_offset = readV2(n["telescope_offset"]);
        }
    }
    catch (YAML::BadConversion &e) {
        cerr << "[Read YAML] ==> " << e.msg << endl;
        return false;
    }
    catch (YAML::InvalidNode &e) {
        cerr << "[Read YAML] ==> " << e.msg << endl;
        return false;
    }

    return true;
}


void Control::RebuildVariables() {
    // Recalculate variables according to input yaml
    ftrk_region_size.setX(ftrk_strip_size.x() * ftrk_strip_arrangement.x() * 1.25);
    ftrk_region_size.setY(ftrk_strip_size.y() * ftrk_strip_arrangement.y() * 1.25);
    ftrk_region_size.setZ(ftrk_position.back().z() - ftrk_position.front().z()
                          + ftrk_strip_size.z() * ftrk_strip_arrangement.z());
    ftrk_region_position = G4ThreeVector(0, 0, -(target_offset + ftrk_region_size.z() / 2.));

    rtrk_region_size.setX(rtrk_strip_size.x() * rtrk_strip_arrangement.x() * 1.25);
    rtrk_region_size.setY(rtrk_strip_size.y() * rtrk_strip_arrangement.y() * 1.25);
    rtrk_region_size.setZ(rtrk_position.back().z() - rtrk_position.front().z()
                          + rtrk_strip_size.z() * rtrk_strip_arrangement.z());
    rtrk_region_position = G4ThreeVector(0, 0, (target_offset + rtrk_region_size.z() / 2.));

    scintillator_region_size.setX((scintillator_size.x() + 2 * wrapper_size.x()) * scintillator_arrangement.x());
    scintillator_region_size.setY((scintillator_size.y() + 2 * wrapper_size.y()) * scintillator_arrangement.y());
    scintillator_region_size.setZ((scintillator_size.z() + 2 * wrapper_size.z()) * scintillator_arrangement.z());
    scintillator_region_position = G4ThreeVector(0, 0, rtrk_region_position.z() + rtrk_region_size.z() / 2
                                                       + scintillator_offset + scintillator_region_size.z() / 2.);

    telescope_region_size.setX((telescope_size.x() + 2 * wrapper_size.x()) * telescope_arrangement.x());
    telescope_region_size.setY((telescope_size.y() + 2 * wrapper_size.y()) * telescope_arrangement.y());
    telescope_region_size.setZ((telescope_size.z() + 2 * wrapper_size.z()) * telescope_arrangement.z());
    telescope_region_position = G4ThreeVector(0, 0, scintillator_region_position.z() +
                                                     scintillator_region_size.z() / 2. +
                                                    telescope_offset +
                                                     telescope_region_size.z() / 2.);

    world_region_size.setX(1.5 * std::max(
            {ftrk_region_size.x(), rtrk_region_size.x(), scintillator_region_size.x(), telescope_region_size.x()}));
    world_region_size.setY(1.5 * std::max(
            {ftrk_region_size.y(), rtrk_region_size.y(), scintillator_region_size.y(), telescope_region_size.y()}));
    world_region_size.setZ(1.5 * (telescope_region_position.z() - ftrk_region_position.z())
                           + (telescope_region_size.z() + ftrk_region_size.z()));

}


G4ThreeVector Control::readV3(const YAML::Node &n, bool unit) {

    // Sanity Check
    if ((n.size() != 3 && !unit) || (n.size() != 6 && unit)) {
        cerr << "[Reading Vector from YAML] ==> vector size is incompatible with unit..." << endl;
        exit(EXIT_FAILURE);
    }

    double tmp[3] = {0.};
    for (unsigned i = 0; i < 3; ++i) {
        auto extra = unit ? G4UnitDefinition::GetValueOf(n[2 * i + 1].as<string>()) : 1;
        tmp[i] = n[unit ? 2 * i : i].as<double>() * extra;
    }

    G4ThreeVector res;
    res.set(tmp[0], tmp[1], tmp[2]);
    return res;
}

// Read number and unit
double Control::readV2(const YAML::Node &n) {
    if (n.size() != 2) {
        cerr << "[Reading Value with Unit from YAML] ==> Value size is incompatible with unit..." << endl;
        exit(EXIT_FAILURE);
    }
    return n[0].as<double>() * G4UnitDefinition::GetValueOf(n[1].as<string>());
}

void Control::BuildMaterialTable(const G4String &mat_file) {
    G4String infile;
    if (mat_file.empty()) infile = "material.yaml";
    else infile = mat_file;
    auto Node = YAML::LoadFile(infile);

    // build external material
    try {
        auto n = Node["build_material"];
        /* For a material definition,
         * 1. Name (Same as material string)
         * 2. Component, must be 2 * (# of element), defined as ["Element Name", percentage]
         * 3. Density
         * 4. state: solid or gas or liquid
         */
        for (auto mat : n) {
            auto mat_name = mat["name"].as<string>();
            auto mat_den = readMatDensity(mat["density"]);
            G4State mat_state;
            if (auto state = mat["state"].as<string>(); state == "gas") mat_state = kStateGas;
            else if (state == "liquid") mat_state = kStateLiquid;
            else if (state == "solid") mat_state = kStateSolid;
            else {
                cerr << "[Read Material] ==> Error defining material: " << mat_name << endl;
                exit(EXIT_FAILURE);
            }

            int n_com = static_cast<int>(mat["composition"].size());
            if (n_com % 2 != 0) {
                cerr << "[Read Material] ==> Error defining material composition: " << mat_name << endl;
                exit(EXIT_FAILURE);
            }
            auto new_mat = new G4Material(mat_name, mat_den, n_com / 2, mat_state);

            addElements(new_mat, mat["composition"]);
        }
    }
    catch (YAML::InvalidNode &e) {
        cerr << "[Read YAML] ==> " << e.msg << endl;
        exit(EXIT_FAILURE);
    }

    // find internal material
    try {
        auto n = Node["internal_material"];
        for (auto mat : n) {
            nistManager->FindOrBuildMaterial(mat["name"].as<string>());
        }
    }
    catch (YAML::InvalidNode &e) {
        cerr << "[Read YAML] ==> " << e.msg << endl;
        exit(EXIT_FAILURE);
    }


    cout << *(G4Material::GetMaterialTable()) << endl;
}

double Control::readMatDensity(const YAML::Node &n) {
    if (n.size() != 3) {
        cerr << "[Reading Density with Unit from YAML] ==> Value size is incompatible with unit..." << endl;
        exit(EXIT_FAILURE);
    }
    return n[0].as<double>() * G4UnitDefinition::GetValueOf(n[1].as<string>()) /
           G4UnitDefinition::GetValueOf(n[2].as<string>());
}

void Control::addElements(G4Material *mat, const YAML::Node &n) {
    for (size_t i = 0; i < n.size(); i += 2) {
        mat->AddElement(nistManager->FindOrBuildElement(n[i].as<string>()), n[i + 1].as<double>());
    }
}

