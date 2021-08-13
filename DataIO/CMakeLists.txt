#----------------------------------------------------------------------------
# Generate ROOT library for PCTEvent
#
# Header

include_directories(./)
include_directories(include)

SET(PEvent_header
        include/DataIO/PCTXData.h
        include/DataIO/PCTStep.h
        include/DataIO/MCParticle.h
        include/DataIO/PCTEvent.h
        )

# Source
SET(PEvent_source
        source/PCTEvent.cpp
        )

root_generate_dictionary(G__PCTEvent ${PEvent_header} LINKDEF include/PCTEventLinkDef.h)
add_library(PCTEvent SHARED ${PEvent_header} ${PEvent_source} G__PCTEvent.cxx)


add_executable(PCTDump source/EventDump.cpp)
target_link_libraries(PCTDump PCTEvent)
install(TARGETS PCTDump DESTINATION bin)

install(TARGETS PCTEvent LIBRARY DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libPCTEvent_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libPCTEvent.rootmap DESTINATION lib)