# Install script for directory: /Users/zhaoyangyuan/phantom/External_Tools/DICOM

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "../DICOM_install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/DICOM" TYPE FILE FILES
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomActionInitialization.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomDetectorConstruction.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomEventAction.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomHandler.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomIntersectVolume.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomNestedParamDetectorConstruction.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomNestedPhantomParameterisation.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomPartialDetectorConstruction.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomPhantomParameterisationColour.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomPhantomZSliceHeader.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomPhantomZSliceMerged.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomPrimaryGeneratorAction.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomRegularDetectorConstruction.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomRun.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/include/DicomRunAction.hh"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/DICOM")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DICOM" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DICOM")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build"
      -delete_rpath "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/dicomReader"
      -delete_rpath "/usr/local/Cellar/dcmtk/3.6.6/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DICOM")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DICOM")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/libDICOM.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libDICOM.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libDICOM.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/dicomReader"
      -delete_rpath "/usr/local/Cellar/dcmtk/3.6.6/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libDICOM.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libDICOM.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3/DICOMTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3/DICOMTargets.cmake"
         "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/CMakeFiles/Export/lib/Geant4-10.6.3/DICOMTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3/DICOMTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3/DICOMTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3" TYPE FILE FILES "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/CMakeFiles/Export/lib/Geant4-10.6.3/DICOMTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3" TYPE FILE FILES "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/CMakeFiles/Export/lib/Geant4-10.6.3/DICOMTargets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3" TYPE FILE FILES
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/InstallTreeFiles/DICOMConfig.cmake"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/InstallTreeFiles/DICOMConfigVersion.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/dicomReader/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
