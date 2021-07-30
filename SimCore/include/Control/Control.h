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
#include "G4UImanager.hh"

// yaml-cpp
#include "yaml-cpp/yaml.h"

#include <vector>
#include <tuple>
#include <map>

using std::vector, std::map, std::tuple;
typedef map<G4String, tuple<G4ThreeVector, G4ThreeVector, G4ThreeVector, G4ThreeVector>> calo_info_map;
typedef map<G4String, tuple<G4ThreeVector, G4ThreeVector, G4ThreeVector, G4ThreeVector, size_t>> trk_info_map;

typedef map<int, tuple<double, double, double>> copy_dictionary;
typedef map<G4String, copy_dictionary> det_copy_dictionary;

/// \brief The central control class which should store all the data

class Control {
public:
    Control(const Control &) = delete;

    Control &operator=(Control const &) = delete;

    static Control *CreateInstance();

    // Rebuild all dependent variables
    void RebuildVariables();

    // Read data from yaml
    bool ReadYAML(const G4String& file_in);

    void ReadAndSetVerbosity();

    void ReadAndSetGPS();

    void ReadAndSetRandomSeed();

public:
    /*************************************/
    /*  Define all the variables needed  */
    /*************************************/

    G4String PSim_version = "0.1.0"; // PSim version
    int BeamOnNumber = 0;
    long random_seed = 0;
    double eps = 1 * um;
    //========================================
    /* Global Variables */
    //----------------------------------------
    bool memory_check{}; // Memory check during the process
    bool save_geometry{}; // save the geometry in root
    bool check_overlaps{}; // check the geometry overlap, may be very slow and verbose
    G4String material_file{}; // file for material table
    bool optical_simulation{};
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
    bool build_target{};
    bool build_ftrk{};
    bool build_rtrk{};
    bool build_scintillator{};
    bool build_telescope{};

    G4String ftrk_name;
    G4String rtrk_name;
    G4String scintillator_name;
    G4String telescope_name;

    G4Material *world_material{};

    double target_offset{};

    // Tracker
    // --------
    // Front Tracker
    G4Material* ftrk_material{};
    G4ThreeVector ftrk_strip_size;
    G4ThreeVector ftrk_strip_arrangement;
    vector<G4ThreeVector> ftrk_position;
    // --------
    // Rear Tracker
    G4Material* rtrk_material{};
    G4ThreeVector rtrk_strip_size;
    G4ThreeVector rtrk_strip_arrangement;
    vector<G4ThreeVector> rtrk_position;

    // Calorimeter
    // material reference: https://www.crystals.saint-gobain.com/products/bc-408-bc-412-bc-416
    // --------
    // holder
    G4Material* holder_material{};
    double holder_gap{};
    // --------
    // wrapper
    G4Material* wrapper_material{};
    G4ThreeVector wrapper_size;
    // --------
    // Scintillator
    G4Material* scintillator_material{};
    G4ThreeVector scintillator_size;
    G4ThreeVector scintillator_arrangement;
    double scintillator_offset{};
    // --------
    // Range Telescope
    G4Material* telescope_material{};
    G4ThreeVector telescope_size;
    G4ThreeVector telescope_arrangement;
    double telescope_offset{};
    // --------
    // Regions (calculated by default)
    G4ThreeVector ftrk_region_size;
    G4ThreeVector ftrk_region_position;
    G4ThreeVector rtrk_region_size;
    G4ThreeVector rtrk_region_position;
    G4ThreeVector scintillator_region_size;
    G4ThreeVector scintillator_region_position;
    G4ThreeVector telescope_region_size;
    G4ThreeVector telescope_region_position;
    G4ThreeVector world_region_size;
    G4ThreeVector world_region_position;

    //------
    // Optics

    // Wrapper
    G4MaterialPropertiesTable *wrapper_surface_material;
    G4OpticalSurface *wrapper_surface;
    vector<double> wrapper_energy; // photon energy [eV]
    vector<double> wrapper_reflectivity;
    vector<double> wrapper_efficiency;
    vector<double> wrapper_transmittance;

    // Crystal
    double crystal_ScintillationYield;
    double crystal_FastTimeConstant;
    vector<double> crystal_energy; // photon energy [eV]
    vector<double> crystal_RefractionIdx;
    vector<double> crystal_AbsorptionLength;
    vector<double> crystal_ScintEnergy;
    vector<double> crystal_ScintFast;

    // Misalliance
    calo_info_map calo_info;
    trk_info_map trk_info;
    det_copy_dictionary det_dict; // map recoding the map between copy number and cell position

private:
    Control();

    static G4ThreeVector readV3(const YAML::Node &n, bool unit=false);
    static double readV2(const YAML::Node &n);

    YAML::Node Node;

    // For geant4 internal settings
    G4UImanager* UIManager;

    // For Material
    G4NistManager *nistManager;
    void BuildMaterialTable(const G4String& mat_file);
    static double readMatDensity(const YAML::Node &n);
    void addElements(G4Material* mat, const YAML::Node &n);

};

extern Control *pControl;
#endif //PSIMU_CONTROL_H
