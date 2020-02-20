include_directories(${util_SOURCE_DIR}/include)

link_libraries(tarsutil)

FILE(GLOB_RECURSE SRC_LIST "*.cpp")

add_executable(demo-util ${SRC_LIST})



