#----------------------------------------------------------------------------
# Generate ROOT library for PEvent
#
# Header

include_directories(./)
include_directories(include)

SET(PEvent_header
        include/DataIO/PCTXData.h
        include/DataIO/PStep.h
        include/DataIO/MCParticle.h
        include/DataIO/PEvent.h
        )

# Source
SET(PEvent_source
        source/PEvent.cpp
        )

root_generate_dictionary(G__PEvent ${PEvent_header} LINKDEF include/PEventLinkDef.h)
add_library(PEvent STATIC ${PEvent_header} ${PEvent_source} G__PEvent.cxx)


add_executable(PDump source/EventDump.cpp)
target_link_libraries(PDump PEvent)
install(TARGETS PDump DESTINATION bin)

install(TARGETS PEvent LIBRARY DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libPEvent_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libPEvent.rootmap DESTINATION lib)