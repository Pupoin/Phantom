#----------------------------------------------------------------------------
# Generate ROOT library for PEvent
#
# Header

include_directories(./)
include_directories(include)

SET(PEvent_header
        include/DataIO/PHit.h
        include/DataIO/PStep.h
        include/DataIO/MCParticle.h
        include/DataIO/PEvent.h
        )

# Source
SET(PEvent_source
        source/PEvent.cpp
        )

root_generate_dictionary(G__PEvent ${PEvent_header} LINKDEF include/PEventLinkDef.h)
add_library(PEvent SHARED ${PEvent_header} ${PEvent_source} G__PEvent.cxx)

install(TARGETS PEvent LIBRARY DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libPEvent_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libPEvent.rootmap DESTINATION lib)