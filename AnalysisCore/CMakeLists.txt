# PCTEvent
include_directories(${PROJECT_SOURCE_DIR}/DataIO/include)

include_directories(include)

file(GLOB ana_source source/*/*.cpp)
file(GLOB ana_header include/*/*.h)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(PCTAna phantom_analysis.cpp ${ana_source} ${ana_header})
target_link_libraries(PCTAna PCTEvent Geant4::G4clhep)

install(TARGETS PCTAna DESTINATION bin)