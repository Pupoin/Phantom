#CADMesh
include_directories(../External_Tools/CADMesh/include)
# PCTEvent
include_directories(${PROJECT_SOURCE_DIR}/DataIO/include)

# set(YAML_CPP_INCLUDE_DIR "/home/anubis/sw/yaml-cpp/include")
# set(YAML_CPP_LIBRARIES "/home/anubis/sw/yaml-cpp/lib/")
# include_directories(${YAML_CPP_INCLUDE_DIR})
# link_directories(${YAML_CPP_LIBRARIES})
find_package(yaml-cpp REQUIRED)
if (yaml-cpp_FOUND)
    INCLUDE_DIRECTORIES(${YAML_CPP_INCLUDE_DIR})
    LINK_LIBRARIES(${YAML_CPP_LIBRARIES})
endif ()

include_directories(include)

file(GLOB sim_source source/*/*.cpp)
file(GLOB sim_header include/*/*.h)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(PCTSim phantom_simulation.cpp ${sim_source} ${sim_header})
target_link_libraries(PCTSim ${Geant4_LIBRARIES} PCTEvent cadmesh yaml-cpp)

install(TARGETS PCTSim DESTINATION bin)

#message(${Geant4_LIBRARIES})