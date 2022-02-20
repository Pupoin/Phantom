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

file(GLOB ana_source source/*/*.cpp)
file(GLOB ana_header include/*/*.h)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(PCTAna phantom_analysis.cpp ${ana_source} ${ana_header})
target_link_libraries(PCTAna PCTEvent ${Geant4_LIBRARIES} yaml-cpp)

install(TARGETS PCTAna DESTINATION bin)