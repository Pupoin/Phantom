

#----------------------------------------------------------------------------
# find json package
# not work
# find_package(jsoncpp REQUIRED)
# if (jsoncpp_FOUND)
#     INCLUDE_DIRECTORIES(${jsoncpp_INCLUDE_DIRS})
#     LINK_LIBRARIES(${jsoncpp_LIBRARIES})
# endif ()


set(JSONCPP_INCLUDE_DIR "/opt/local/include/json/")
set(JSONCPP_LIBRARIES "/opt/local/lib/json/")
include_directories(${JSONCPP_INCLUDE_DIR})
link_directories(${JSONCPP_LIBRARIES})

# PCTEvent
include_directories(${PROJECT_SOURCE_DIR}/DataIO/include)
include_directories(include)


# include_directories(${jsoncpp_INLCUDE_DIRS}/include)
# include_directories(include
#     ${jsoncpp_INLCUDE_DIRS}
# )

include_directories(${PROJECT_SOURCE_DIR}/PctSim/sim/detector)
include_directories(${PROJECT_SOURCE_DIR}/PctSim/sim/io)
include_directories(${PROJECT_SOURCE_DIR}/PctSim/sim/run)
include_directories(${PROJECT_SOURCE_DIR}/PctSim/sim/tools)
include_directories(${PROJECT_SOURCE_DIR}/PctSim/sim/hooks)

file(GLOB fsim_source_det sim/detector/*.cc)
file(GLOB fsim_source_io sim/io/*.cc)
file(GLOB fsim_source_run sim/run/*.cc)
file(GLOB fsim_source_tools sim/tools/*.cc)
file(GLOB fsim_source_hooks sim/hooks/*.cc)
file(GLOB fsim_header CmdParser.h)

#----------------------------------------------------------------------------
#
add_executable(PctDumpMaterial PctDumpMaterial.cpp ${fsim_header})
target_link_libraries(PctDumpMaterial ${Geant4_LIBRARIES} jsoncpp)

add_executable(PctAddMaterial PctAddMaterial.cpp ${fsim_header})
target_link_libraries(PctAddMaterial ${Geant4_LIBRARIES} jsoncpp)

add_executable(PctDumpElement PctDumpElement.cpp ${fsim_header})
target_link_libraries(PctDumpElement ${Geant4_LIBRARIES} jsoncpp)

add_executable(PctDumpParticleTable PctDumpParticleTable.cpp ${fsim_header})
target_link_libraries(PctDumpParticleTable ${Geant4_LIBRARIES} jsoncpp)

add_executable(FSim PctTSim.cpp Json2Geant4Macro.cpp ${fsim_header} ${fsim_source_det} ${fsim_source_io} ${fsim_source_run} ${fsim_source_tools} ${fsim_source_hooks})
target_link_libraries(FSim ${Geant4_LIBRARIES} jsoncpp)

#add_executable(PctDumpRawFile PctDumpRawFile.cpp sim/io/PctRawReader.cc sim/io/PctDataType.cc ${fsim_header})
#target_link_libraries(PctDumpRawFile ${Geant4_LIBRARIES} ${jsoncpp_LIB} )

install(TARGETS PctDumpElement DESTINATION bin)
install(TARGETS PctDumpMaterial DESTINATION bin)
install(TARGETS PctAddMaterial DESTINATION bin)
install(TARGETS PctDumpParticleTable DESTINATION bin)
#install(TARGETS PctDumpRawFile DESTINATION bin)
install(TARGETS FSim DESTINATION bin)
