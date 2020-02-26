include_directories(${util_SOURCE_DIR}/include)

FILE(GLOB_RECURSE SRC_LIST "*.cpp")

if (TARS_SSL)
    if (WIN32)
        link_libraries(tarsutil ${LIB_SSL} ${LIB_CRYPTO} Crypt32)
    else ()
        link_libraries(tarsutil ${LIB_SSL} ${LIB_CRYPTO})
    endif ()
else ()
    link_libraries(tarsutil)
endif ()

add_executable(demo-server ${SRC_LIST})

add_dependencies(demo-server tarsutil)

