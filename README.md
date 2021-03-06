# Phantom Simulation

The simulation package for proton CT.

[[_TOC_]]

## Get Started
___________
Update 2022/02/20 by Zhaoyang Yuan  
Adding organism (such as lobster) as target.
Firstly, dicom package need to be compiled.
```bash 
cd External_Tools/DICOM
source loadenv.sh
mkdir -p build
cd build
cmake .. 
make -j 
make install # installation path is phantom/External_Tools/DICOM/DICOM_install
```

_____________________________________
- **PCTSim** is the simulation program based on Geant4 and ROOT, characterized by proton CT.
- **PCTDump** is a quick print program to read output from **PCTSim**.
- **PCTAna** is the reconstruction and analysis framework for **PCTEvent** data.

<mark>The default unit in Phantom Project is MeV, nanosecond, millimeter<mark>

## Installation

Phantom can be easily downloaded through GitLab.

```c++
    git clone https://gitlab.com/yulei_zhang/phantom.git
```

Before installing, several dependencies need to be checked.

- C++17
- Geant4 10.06
- ROOT 6 (compiled with c++17)
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

### PCTSim

necessary configuration files: ```default.yaml```, ```material.yaml```

**Batch Mode**: ```PCTSim [ -y default.yaml]```

- (**IMPORTANT**)The config file after '-y' is the yaml file, which will control the whole configuration of the program.
  The default yaml file is in ```Scripts/```, or it will also be installed to the ```CMAKE_ISNTALL_PREFIX``` path.
- There are several config files (in ```Scripts/```):
    - ```default.yaml```: the main configuration file, including particle gun, random seed, output root file, geometry
      settings etc.
    - ```material.yaml```: all the materials needed by geometry are registered here, including optical properties.

- One can ignore ```-y default.yaml``` if there is a file called ```default.yaml``` under the running directory.

**Graphic Mode**: ```PCTSim -v```

### PCTAna

There is no explicit default configuration file for **PCTAna**. One can simply generate a default version through **
Default Mode**.

**Default Mode**: ```PCTAna -x```

- Default mode will automatically generate a plug-and-play configuration file (yaml file). All the parameters are set to
  be the default value in the analyzers (which should be the most recommended value by the developer.)
- A common procedure (under unix) is ``` PCTAna -x > analysis.yaml```, which simply means redirect the output
  of ```PCTAna -x``` to a new file ```analysis.yaml```.

**Active Mode**: ```PCTAna [ analysis.yaml ]```

- Active mode should work with a given configuration file explicitly (which should be generated by Default mode and
  modified by own demands).
- Event number of value "-1" means proceeding all available events.
- It's strongly to recommend set high verbosity to debug first, but remember to turn off verbosity for large amount of
  events. (One can set EventReader in Verbosity to be 2 in order to start process reminder.)
- ```Analyzer_List``` means the list of analyzers users want to activate. The content should be the name of the analyzer
  class while defining its constructor.
- ```Analyzer_Settings``` gives the interface for users changing parameters of certain analyzer without recompilation.
  For the parameters, current supported data type are:
    - ```bool, std::vector<bool>```
    - ```int, std::vector<int>```
    - ```double, std::vector<double>```
    - ```std::string, std::vector<std::string>```
- Analyzer developers should pay attention to the data type of the parameters and set reasonable default value.

### PCTDump

Since the data are encapsulated in ```PCTEvent``` class, it's hard to direct read them from root (libraries and scripts
are needed). One can simply dump all the detailed information in ```PCTEvent``` class by **PCTDump**.

**Simplified Mode**: ```PCTDump [target file] [target tree name]```

- Only show the simplified information of all collections.

**Full Mode**: ```PCTDump [target file] [target tree name] [entry number to be printed]```

# PCTEvent Wiki

<details><summary><b>Data Structure</b></summary>
PCTEvent is the self-developed C++ data structure used for Phantom Simulation. It's basically a C++ class with several class members
, which are some fundamental physical quantities. The primary members of PCTEvent are as following:

- ```MCParticle```: a class recording all monte carlo particle information.
- ```PCTStep```: mainly focusing on step of motion of the primary proton.
- ```PCTXData```: containing raw hits and digitization information.

Each basic type has its own collection map:

- ```mcp_map = std::map<TString, vector<MCParticle * >>```
- ```step_map = std::map<TString, vector<PCTStep * >>```
- ```hit_map = std::map<TString, vector<PCTXData * >>```

All the information records in the corresponding map. For example, the detector hits of Scintillator module fills
as ```std::pair("Scintillator", vector of PCTXData* )```, and the hits from Range Telescope
are ```std::pair("Telescope", vector of PCTXData* )```.

</details>

<details><summary><b>Read Collections</b></summary>
PCTEvent class has encapsulated the central method to read wanted vector as long as the collection name is given. 
The way to read the vector is like:

```c++
// Read PCTEvent from somewhere
PCTEvent *evt;

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
 * hits: std::vector<PCTXData* >* 
 * mcps: std::vector<MCParticle* >*
 * steps: std::vector<PCTStep* >*
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

It's quite straight forward to add or delete collections, which is similar to ```std::map```, but PCTEvent has already
provided the interface to do it:

```c++
// Read PCTEvent from somewhere
PCTEvent *evt;

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

# PCTAna Wiki

There are several singleton classes in **PCTAna** so that one can easily retrieve global information:

- ```GeometryHelper```: pGeo
- ...

<details> <summary><b>GeometryHelper</b></summary>
<mark>The default unit in GeometryHelper is millimeter.</mark>

The extern pointer of this singleton class is ```pGeo```, which is equivalent to ```GeometryHelper::CreateInstace()```.
Before running any function of this class, it will automatically scan if the Geometry is successfully imported. If not,
the program will exit with failure and error message. The main function of this geometry class is to quickly calculate
the geometry information as followed:

```c++
// Calculate distance of two points
double GetDisOfTwo(const TVector3 &a, const TVector3 &b);

// Get Center Point of the targeted geometry
TVector3 GetPosOfVolume(const TString &det_name, int copyNo);

// Get Size of the targeted geometry (Not Half!)
TVector3 GetSizeOfVolume(const TString &det_name, int copyNo);

// Get Distance between the given point and the surface of targeted geometry
// surface is defined as:
//  x_plus: point to the +x surface (left)
//  x_minus: point to the -x surface (right)
//  y_plus: point to the +y surface (top)
//  y_minus: point to the -y surface (bottom)
//  z_plus: point to the +z surface (front)
//  z_minus: point to the -z surface (back)
//
double GetDisToSuf(const TVector3 &point, const TString &det_name, int copyNo, surface surf);
```

It's user's responsibility to supply the detector name and the corresponding copy number of the wanted cell, which
usually can be fetched in hit information. One can do the calculation in the Analyzer:

````c++
/* Example of how to use GeometryHelper Class to calculate geometry information */
// Get The position of Scintillator with copy number of 2
TVector3 position = pGeo->GetPosOfVolume("Scintillator", 2);

// Get The length of Scintillator with copy number of 2
TVector3 size = pGeo->GetSizeOfVolume("Telescope", 2);

// Get The distance of one point to the surface
TVector3 point = { 110., 50., 2.1 };
double distance = pGeo->GetDisToSuf(point, "Scintillator", 1, surface::z_plus); // distance to the +z surface
````

</details>