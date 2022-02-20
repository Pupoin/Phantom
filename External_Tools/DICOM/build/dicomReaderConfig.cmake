set(FOO_VERSION 10.6.3)

macro(set_and_check _var _file)
    set(${_var} "${_file}")
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
    endif()
endmacro()

macro(check_required_components _NAME)
    foreach(comp ${${_NAME}_FIND_COMPONENTS})
        if(NOT ${_NAME}_${comp}_FOUND)
            if(${_NAME}_FIND_REQUIRED_${comp})
                set(${_NAME}_FOUND FALSE)
            endif()
        endif()
    endforeach()
endmacro()


set_and_check(dicomReader_INCLUDE_DIR "/Users/zhaoyangyuan/phantom/External_Tools/DICOM/dicomReader/include")

check_required_components(dicomReader)

include(${CMAKE_CURRENT_LIST_DIR}/dicomReaderBuild.cmake)

find_package(DCMTK REQUIRED)

set(dicomReader_INCLUDE_DIRS ${dicomReader_INCLUDE_DIR} ${DCMTK_INCLUDE_DIRS})
set(dicomReader_LIBRARIES dicomReader ${DCMTK_LIBRARIES})

set(dicomReader_FOUND ON)
