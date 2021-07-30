//
// Created by Zhang Yulei on 12/31/20.
//

#include "Control/Control.h"

// Geant4
#include "G4UnitsTable.hh"

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
    G4NistManager *nistManager = G4NistManager::Instance();

}

void Control::ConstructG4MaterialTable() const {

}

bool Control::ReadYAML(const G4String &file_in) {
    G4String infile;
    if (file_in.empty()) infile = "default.yaml";
    else infile = file_in;
    auto Node = YAML::LoadFile(infile);

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
            outfile_Name = n["outfile_Name"].as<std::string>();
            tree_Name = n["tree_Name"].as<std::string>();
            Run_Number = n["Run_Number"].as<int>();
            Total_Event_Number = n["Total_Event_Number"].as<int>();
        }
        //----------------------------------------
        // Out Collection Options
        if (auto n = Node["OutCollection"]; n.IsDefined()) {
            save_all_mcp = n["save_all_mcp"].as<bool>();
            MCParticle_Name = n["MCParticle_Name"].as<std::string>();
            ParticleStep_Name = n["ParticleStep_Name"].as<std::string>();
        }

        //========================================
        /* Geometry */
        //----------------------------------------
        if (auto n = Node["Geometry"]; n.IsDefined()) {
            target_offset = readV2(n["target_offset"]);
        }
    }
    catch (YAML::BadConversion &e) {
        std::cerr << "[Read YAML] ==> " << e.msg << std::endl;
        return false;
    }
    catch (YAML::InvalidNode &e) {
        std::cerr << "[Read YAML] ==> " << e.msg << std::endl;
        return false;
    }

    return true;
}


void Control::RebuildVariables() {

}


G4ThreeVector Control::readV3(const YAML::Node &n, bool unit) {

    // Sanity Check
    if ((n.size() != 3 && !unit) || (n.size() != 6 && unit)) {
        std::cerr << "[Reading Vector from YAML] ==> vector size is incompatible with unit..." << std::endl;
        exit(EXIT_FAILURE);
    }

    double tmp[3] = {0.};
    for (unsigned i = 0; i < 3; ++i) {
        auto extra = unit ? G4UnitDefinition::GetValueOf(n[2 * i + 1].as<std::string>()) : 1;
        tmp[i] = n[unit ? 2 * i : i].as<double>() * extra;
    }

    G4ThreeVector res;
    res.set(tmp[0], tmp[1], tmp[2]);
    return res;
}

// Read number and unit
double Control::readV2(const YAML::Node &n) {
    if (n.size() != 2) {
        std::cerr << "[Reading Value with Unit from YAML] ==> Value size is incompatible with unit..." << std::endl;
        exit(EXIT_FAILURE);
    }
    return n[0].as<double>() * G4UnitDefinition::GetValueOf(n[1].as<std::string>());
}
