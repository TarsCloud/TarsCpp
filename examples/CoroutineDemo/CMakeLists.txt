
include_directories(AServer)
add_subdirectory(AServer)

include_directories(BServer)
add_subdirectory(BServer)

add_subdirectory(client)
add_subdirectory(testCoro)
add_subdirectory(testParallelCoro)

