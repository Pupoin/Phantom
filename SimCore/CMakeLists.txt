#CADMesh
include_directories(${PROJECT_SOURCE_DIR}/CADMesh/include)
# PEvent
include_directories(${PROJECT_SOURCE_DIR}/DataIO/include)

include_directories(include)

file(GLOB sim_source source/*/*.cpp)
file(GLOB sim_header include/*/*.h)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(PSim phantom_simulation.cpp ${sim_source} ${sim_header})
target_link_libraries(PSim ${Geant4_LIBRARIES} PEvent cadmesh)

install(TARGETS PSim DESTINATION bin)