include_directories(${util_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/src/gtest/include)
link_directories(${CMAKE_BINARY_DIR}/src/gtest/lib)
link_directories(${CMAKE_BINARY_DIR}/src/gtest/lib64)

FILE(GLOB_RECURSE SRC_LIST "*.cpp")

if (TARS_SSL)
    if (WIN32)
        link_libraries(tarsutil gtest ${LIB_SSL} ${LIB_CRYPTO} Crypt32)
    else ()
        link_libraries(tarsutil gtest ${LIB_SSL} ${LIB_CRYPTO})
    endif ()
else ()
    link_libraries(tarsutil gtest)
endif ()

add_executable(demo-util ${SRC_LIST})

add_dependencies(demo-util tarsutil)


