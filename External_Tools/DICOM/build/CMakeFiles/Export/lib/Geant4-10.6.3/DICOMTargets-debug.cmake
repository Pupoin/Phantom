#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "DICOM-library" for configuration "Debug"
set_property(TARGET DICOM-library APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(DICOM-library PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libDICOM.dylib"
  IMPORTED_SONAME_DEBUG "@rpath/libDICOM.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS DICOM-library )
list(APPEND _IMPORT_CHECK_FILES_FOR_DICOM-library "${_IMPORT_PREFIX}/lib/libDICOM.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
