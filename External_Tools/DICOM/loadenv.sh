#!/opt/local/bin/bash
# . ~/spack2/spack/share/spack/setup-env.sh 
# spack env activate pha

export DYLD_LIBRARY_PATH="$DYLD_LIBRARY_PATH:/Users/zhaoyangyuan/newppp/DICOM/build/install/lib"
export PATH="/Users/zhaoyangyuan/newppp/DICOM/build/install/bin:$PATH"

# . /opt/local/bin/thisroot.sh
# . /usr/local/Cellar/geant4/bin/geant4.sh
source /opt/local/libexec/Geant4/Geant4.10.6/geant4.sh
source /opt/local/libexec/Geant4/Geant4.10.6/geant4data.sh

# export CMAKE_PREFIX_PATH="/opt/local/lib/Geant4/Geant4.10.5/Geant4-10.5.1:$CMAKE_PREFIX_PATH"
export CMAKE_PREFIX_PATH="/opt/local/lib/Geant4/Geant4.10.6/Geant4-10.6.3:$CMAKE_PREFIX_PATH"

export CMAKE_PREFIX_PATH="/usr/local/Cellar/dcmtk/3.6.6/lib/cmake/dcmtk:$CMAKE_PREFIX_PATH"


# export DICOM_USE_HEAD=1
# export DICOM_PATH="/Users/zhaoyangyuan/newppp/DICOM/build/DICOM1.1/DICOM_HEAD_TEST"
# cmake -DDICOM_USE_HEAD=1  ..

# export DICOM_USE_DCMTK="1"
# export DCMTK_DIR="/usr/local/Cellar/dcmtk/3.6.6/"
# export DYLD_LIBRARY_PATH="$DYLD_LIBRARY_PATH:/usr/local/Cellar/dcmtk/3.6.6/lib"
# cmake -DDICOM_USE_DCMTK=/usr/local/Cellar/dcmtk/3.6.6/  ..

