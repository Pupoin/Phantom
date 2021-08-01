# Phantom Simulation

The simulation package for proton CT.

[[_TOC_]]

## Get Started

- **PSim** is the simulation program based on Geant4 and ROOT, characterized by proton CT.
- **PDump** is a quick print program to read output from **PSim**.

## Installation

Phantom can be easily downloaded through GitLab.

```c++
    git clone https://gitlab.com/yulei_zhang/phantom.git
```

Before installing, several dependencies need to be checked.

- C++17
- Geant4 10.06
- ROOT 6
- yaml-cpp

With everything needed, it's ready to install.

```shell script
cd phantom   # <source-directory>
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<some-directory> ../
make -j100  # Just do it!
make install
```

Now in your install directory, everything should be there. Now it's the time to have fun with them.

## Quick guide on Phantom Simulation

<mark>All the example scripts can be found under the "Scripts" folder. <mark>

### PSim

necessary configuration files: ```default.yaml```, ```material.yaml```

**Batch Mode**: ```PSim [ -y default.yaml]```

- (**IMPORTANT**)The config file after '-y' is the yaml file, which will control the whole configuration of the program.
  The default yaml file is in ```Scripts/```, or it will also be installed to the ```CMAKE_ISNTALL_PREFIX``` path.
- There are several config files (in ```Scripts/```):
    - ```default.yaml```: the main configuration file, including particle gun, random seed, output root file, geometry
      settings etc.
    - ```material.yaml```: all the materials needed by geometry are registered here, including optical properties.
- One can ignore ```-y default.yaml``` if there is a file called ```default.yaml``` under the running directory.

**Graphic Mode**: ```PSim -v```

### PDump

Since the data are encapsulated in ```PEvent``` class, it's hard to direct read them from root (libraries and scripts are needed). 
One can simply dump all the detailed information in ```PEvent``` class by **PDump**. 

**Simplified Mode**: ```PDump [target file] [target tree name]```

- Only show the simplified information of all collections.

**Full Mode**: ```PDump [target file] [target tree name] [entry number to be printed]```

# PEvent Wiki

<details><summary><b>Data Structure</b></summary>
PEvent is the self-developed C++ data structure used for Phantom Simulation. It's basically a C++ class with several class members
, which are some fundamental physical quantities. The primary members of PEvent are as following:

- ```MCParticle```: a class recording all monte carlo particle information.
- ```PStep```: mainly focusing on step of motion of the primary proton.
- ```PHit```: containing raw hits and digitization information.

Each basic type has its own collection map:

- ```mcp_map = std::map<TString, vector<MCParticle * >>```
- ```step_map = std::map<TString, vector<PStep * >>```
- ```hit_map = std::map<TString, vector<PHit * >>```

All the information records in the corresponding map. For example, the detector hits of Scintillator module fills
as ```std::pair("Scintillator", vector of PHit* )```, and the hits from Range Telescope
are ```std::pair("Telescope", vector of PHit* )```.

</details>

<details><summary><b>Read Collections</b></summary>
PEvent class has encapsulated the central method to read wanted vector as long as the collection name is given. 
The way to read the vector is like:

```c++
// Read PEvent from somewhere
PEvent *evt;

/*
 * The names of the following collections are just for reference
 */

// Read Detector Hits        
auto hits = evt->GetData("Scintillator", DetectorHit_DataType::COL);

// Read Monte Carlo Particles
auto mcps = evt->GetData("MCPaticles", MCParticle_DataType::COL);

// Read Initial Proton Steps
auto steps = evt->GetData("Init_Particle_Step", ParticleStep_DataType::COL);

/*
 * hits: std::vector<PHit* >* 
 * mcps: std::vector<MCParticle* >*
 * steps: std::vector<PStep* >*
 */
```

There are three enum types which are used to differentiate which kind of collections users want to retreive:

```c++
enum class MCParticle_DataType { COL };
enum class ParticleStep_DataType { COL };
enum class DetectorHit_DataType { COL };
```

</details>


<details><summary><b>Add and Delete Collections</b></summary>

It's quite straight forward to add or delete collections, which is similar to ```std::map```, but PEvent has already
provided the interface to do it:

```c++
// Read PEvent from somewhere
PEvent *evt;

// Register a detector hit collection called "Telescope"
evt->RegisterCollection("Telescope", Phantom_DataType::DetectorHit);

// Register a mc particle collection called "MCParticle"
evt->RegisterCollection("MCParticle", Phantom_DataType::MCParticle);

// Register a detector hit collection called "InitialStep"
evt->RegisterCollection("InitialStep", Phantom_DataType::ParticleStep);

// Delete the detector hit collection called "FrontTracker"
evt->DeleteCollection("FrontTracker", Phantom_DataType::DetectorHit);
```

The second argument is a self-defined enum class type, which is defined as 
```c++
enum class Phantom_DataType {
    MCParticle, ParticleStep, DetectorHit
};
```
</details>
