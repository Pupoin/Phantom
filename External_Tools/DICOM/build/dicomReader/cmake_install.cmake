# Install script for directory: /Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dicomReader" TYPE FILE FILES
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomBeam.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomBeamBlock.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomBeamCompensator.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomBeamControlPoint.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomBeamDevice.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomBeamDevicePos.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomBeamDeviceRef.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomBeamWedge.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomFileCT.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomFileCT_NOdcmrt.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomFileMgr.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomFilePET.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomFilePlan.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomFileStructure.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomROI.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomROIContour.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomVBeamDevice.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomVFile.hh"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include/DicomVFileImage.hh"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/dicomReader/libdicomReader.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdicomReader.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdicomReader.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/usr/local/Cellar/dcmtk/3.6.6/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdicomReader.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdicomReader.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3/dicomReaderTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3/dicomReaderTargets.cmake"
         "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/dicomReader/CMakeFiles/Export/lib/Geant4-10.6.3/dicomReaderTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3/dicomReaderTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3/dicomReaderTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3" TYPE FILE FILES "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/dicomReader/CMakeFiles/Export/lib/Geant4-10.6.3/dicomReaderTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3" TYPE FILE FILES "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/dicomReader/CMakeFiles/Export/lib/Geant4-10.6.3/dicomReaderTargets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Geant4-10.6.3" TYPE FILE FILES
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/InstallTreeFiles/dicomReaderConfig.cmake"
    "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/build/InstallTreeFiles/dicomReaderConfigVersion.cmake"
    )
endif()

