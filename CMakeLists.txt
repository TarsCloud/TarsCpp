cmake_minimum_required(VERSION 3.2)

project(tars-cpp)

#-------------------------------------------------------------------

if("${TARS_CPP_COMMON}" STREQUAL "")

    set(TARS_CPP_COMMON "1")

    include("${PROJECT_SOURCE_DIR}/cmake/GenVersion.cmake")

    include("${PROJECT_SOURCE_DIR}/cmake/Common.cmake")

    include("${PROJECT_SOURCE_DIR}/cmake/BuildTarsCpp.cmake")

    include("${PROJECT_SOURCE_DIR}/cmake/Thirdparty.cmake")

    include("${PROJECT_SOURCE_DIR}/cmake/ParseAsm.cmake")

endif()

add_definitions(-DTARS_VERSION="${TARS_VERSION}")
message("TARS_VERSION:              ${TARS_VERSION}")

# if(WIN32)
# option(ENABLE_MT "option for /MT" ON)
# include(${PROJECT_SOURCE_DIR}/cmake/compiler_flags_overrides.cmake)
# message("ENABLE_MT:                 ${ENABLE_MT}")
# endif(WIN32)

set(CLEAR_INCLUDE "clear-install.cmake")
FILE(WRITE ${CLEAR_INCLUDE} "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_INSTALL_PREFIX}/include)\n")
install(SCRIPT ${CLEAR_INCLUDE})

#-------------------------------------------------------------------

include_directories(${PROJECT_SOURCE_DIR}/util/include)

include_directories(${PROJECT_SOURCE_DIR}/servant)
include_directories(${PROJECT_SOURCE_DIR}/servant/protocol)

IF(WIN32)
include_directories(${PROJECT_SOURCE_DIR}/util/src/epoll_windows)
ENDIF()

add_subdirectory(util)
add_subdirectory(tools)
add_subdirectory(servant)
add_subdirectory(mock)

IF (NOT ${ONLY_LIB})
add_subdirectory(examples)
add_subdirectory(unit-test)
ENDIF()


