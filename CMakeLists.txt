cmake_minimum_required(VERSION 3.2)

project(tars-cpp)

#-------------------------------------------------------------------

if("${TARS_CPP_COMMON}" STREQUAL "")

    set(TARS_CPP_COMMON "1")

    include("${PROJECT_SOURCE_DIR}/cmake/Common.cmake")

    include("${PROJECT_SOURCE_DIR}/cmake/BuildTarsCpp.cmake")

    include("${PROJECT_SOURCE_DIR}/cmake/Thirdparty.cmake")

    include("${PROJECT_SOURCE_DIR}/cmake/ParseAsm.cmake")

endif()

#-------------------------------------------------------------------

include_directories(${PROJECT_SOURCE_DIR}/util/include)
include_directories(${PROJECT_SOURCE_DIR}/util/src/epoll_windows)

include_directories(${PROJECT_SOURCE_DIR}/servant)
include_directories(${PROJECT_SOURCE_DIR}/servant/protocol)

IF(WIN32)
include_directories(${CMAKE_SOURCE_DIR}/util/src/epoll_windows)
ENDIF()

add_subdirectory(util)
add_subdirectory(tools)
add_subdirectory(servant)
add_subdirectory(examples)

#add_subdirectory(test_deprecated)


