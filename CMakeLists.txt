#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(PCTSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

set(OpenGL_GL_PREFERENCE GLVND)

# define path to macros and settings
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if (WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else ()
    find_package(Geant4 REQUIRED)
endif ()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find YAML and XercesC required packages
#

# yaml-cpp
find_package(yaml-cpp REQUIRED)
LINK_LIBRARIES(yaml-cpp)

find_package(XercesC REQUIRED)
if (XercesC_FOUND)
    INCLUDE_DIRECTORIES(${XercesC_INCLUDE_DIRS})
    LINK_LIBRARIES(${XercesC_LIBRARIES})
endif ()

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
FIND_PACKAGE(OpenGL)

FIND_PACKAGE(ROOT COMPONENTS Geom TMVA Eve Gui RGL EG)
IF (ROOT_FOUND)
    INCLUDE_DIRECTORIES(${ROOT_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${ROOT_INCLUDE_DIR})
    LINK_LIBRARIES(${ROOT_LIBRARIES})
    LINK_DIRECTORIES(${ROOT_LIBRARY_DIR})

    if (${ROOT_VERSION} VERSION_GREATER_EQUAL "6.24.0")
        ADD_DEFINITIONS("-DRM_UNIT")
    endif ()
ENDIF ()

#----------------------------------------------------------------------------
# For Memory Check
#
option(MEMCK "MEMCK" OFF)
if (MEMCK OR ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    MESSAGE("-- Memory Check is ON")
    add_definitions("-DMEMCK")
endif ()

#----------------------------------------------------------------------------
# Add sub-directory for this project
#
add_subdirectory(CADMesh)

add_subdirectory(DataIO)

add_subdirectory(SimCore)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#


#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDEvent_rdict.pcm DESTINATION lib)
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDEvent.rootmap DESTINATION lib)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory
#
set(RUN_SCRIPTS
        Scripts/default.yaml
        Scripts/material.yaml
        Scripts/init_vis.mac
        Scripts/vis.mac
        Scripts/gui.mac
        )

foreach (_script ${RUN_SCRIPTS})
    configure_file(
            ${PROJECT_SOURCE_DIR}/${_script}
            ${PROJECT_BINARY_DIR}/
            COPYONLY
    )
endforeach ()
