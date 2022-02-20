#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "dicomReader" for configuration "Debug"
set_property(TARGET dicomReader APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(dicomReader PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libdicomReader.dylib"
  IMPORTED_SONAME_DEBUG "@rpath/libdicomReader.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS dicomReader )
list(APPEND _IMPORT_CHECK_FILES_FOR_dicomReader "${_IMPORT_PREFIX}/lib/libdicomReader.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
