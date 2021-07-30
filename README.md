# Phantom Simulation

The simulation package for proton CT.


[[_TOC_]]

## Get Started

- **PSim** is the simulation program based on Geant4 and ROOT, characterized by proton CT.

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

**Batch Mode**: ```PSim [ -y default.yaml]```

- (**IMPORTANT**)The config file after '-y' is the yaml file, which will control the whole configuration of the program.
  The default yaml file is in ```Scripts/```, or it will also be installed to the ```CMAKE_ISNTALL_PREFIX``` path.
- There are several config files (in ```Scripts/```):
    - ```default.yaml```: the main configuration file, including particle gun, random seed, output root file, geometry settings etc.
    - ```material.yaml```: all the materials needed by geometry are registered here, including optical properties.  
- One can ignore ```-y default.yaml``` if there is a file called ```default.yaml``` under the running directory.

**Batch Mode**: ```PSim -v```

