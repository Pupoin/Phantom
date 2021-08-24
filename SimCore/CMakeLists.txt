#CADMesh
include_directories(../External_Tools/CADMesh/include)
# PCTEvent
include_directories(${PROJECT_SOURCE_DIR}/DataIO/include)

include_directories(include)

file(GLOB sim_source source/*/*.cpp)
file(GLOB sim_header include/*/*.h)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(PCTSim phantom_simulation.cpp ${sim_source} ${sim_header})
target_link_libraries(PCTSim ${Geant4_LIBRARIES} PCTEvent cadmesh)

install(TARGETS PCTSim DESTINATION bin)

#message(${Geant4_LIBRARIES})