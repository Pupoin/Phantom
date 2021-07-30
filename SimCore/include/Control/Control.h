//
// Created by Zhang Yulei on 12/31/20.
//
#ifndef PSIMU_CONTROL_H
#define PSIMU_CONTROL_H

// Geant4 Class
#include <G4ThreeVector.hh>
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4OpticalSurface.hh"

// yaml-cpp
#include "yaml-cpp/yaml.h"

#include <vector>
#include <tuple>

/// \brief The central control class which should store all the data

class Control {
public:
    Control(const Control &) = delete;

    Control &operator=(Control const &) = delete;

    static Control *CreateInstance();

    // build geant4 material table
    void ConstructG4MaterialTable() const;

    // Rebuild all dependent variables
    void RebuildVariables();

    // Read data from yaml
    bool ReadYAML(const G4String& file_in);

public:
    /*************************************/
    /*  Define all the variables needed  */
    /*************************************/

    G4String PSim_version = "0.1.0"; // PSim version
    //========================================
    /* Global Variables */
    //----------------------------------------
    bool memory_check{}; // Memory check during the process
    bool save_geometry{}; // save the geometry in root
    bool check_overlaps{}; // check the geometry overlap, may be very slow and verbose

    //----------------------------------------
    // Root Manager Options
    G4String outfile_Name = "p_out.root";
    G4String tree_Name = "p";

    /* event_id = id + Run_Number * Total_Event_Number */
    int Run_Number{}; // run number
    int Total_Event_Number{}; // event number

    //----------------------------------------
    // Out Collection Options
    bool save_all_mcp = false; // save all the mc particles

    G4String MCParticle_Name = "MCParticle";
    G4String ParticleStep_Name = "Init_Particle_Step";

    //========================================
    /* Geometry Settings */
    //----------------------------------------
    G4ThreeVector Size_World;
    G4Material *World_Mat{};

    double target_offset{};

private:
    Control();

    static G4ThreeVector readV3(const YAML::Node &n, bool unit=false);
    static double readV2(const YAML::Node &n);
};

extern Control *pControl;
#endif //PSIMU_CONTROL_H
