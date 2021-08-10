
# CADMesh
file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hh)

include_directories(include)

# Set up the target.
add_library(cadmesh "SHARED" ${sources} ${headers})

# Link against either the system libraries, or the external project libaries.
target_link_libraries(cadmesh ${Geant4_LIBRARIES})

# Install the CADMesh headers and library.
install(TARGETS cadmesh DESTINATION lib)


