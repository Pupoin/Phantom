#!/bin/bash
# . ~/spack2/spack/share/spack/setup-env.sh 
# spack env activate geant44

export DYLD_LIBRARY_PATH="$DYLD_LIBRARY_PATH:/Users/zhaoyangyuan/phantom/build/install/lib"
export PATH="$PATH:/Users/zhaoyangyuan/phantom/build/install/bin"
# json
export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/opt/local/include/json/"
export DYLD_LIBRARY_PATH="$DYLD_LIBRARY_PATH:/opt/local/lib/json/"

# for root
# source /opt/local/libexec/root6/bin/thisroot.sh
# export CMAKE_PREFIX_PATH="/usr/local/Cellar/clhep/2.4.5.1/lib/CLHEP-2.4.5.1:$CMAKE_PREFIX_PATH"
export CMAKE_PREFIX_PATH="/opt/local/libexec/root6/share/root/cmake/:$CMAKE_PREFIX_PATH"
export CMAKE_PREFIX_PATH="/usr/local/Cellar/yaml-cpp/0.6.3_1/lib/cmake/yaml-cpp:$CMAKE_PREFIX_PATH"
# for geant4
source /opt/local/libexec/Geant4/Geant4.10.6/geant4.sh
source /opt/local/libexec/Geant4/Geant4.10.6/geant4data.sh
export CMAKE_PREFIX_PATH="/opt/local/lib/Geant4/Geant4.10.6/Geant4-10.6.3:$CMAKE_PREFIX_PATH"
# for dcmtk
export CMAKE_PREFIX_PATH="/usr/local/Cellar/dcmtk/3.6.6/lib/cmake/dcmtk:$CMAKE_PREFIX_PATH"
# for dicom
export CMAKE_PREFIX_PATH="/Users/zhaoyangyuan/phantom/External_Tools/DICOM/install/lib/Geant4-10.6.3:$CMAKE_PREFIX_PATH"
# export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/"
export DYLD_LIBRARY_PATH="$DYLD_LIBRARY_PATH:/Users/zhaoyangyuan/phantom/External_Tools/DICOM/install/lib/"

