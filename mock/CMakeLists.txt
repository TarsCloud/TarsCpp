
include_directories(../)
file(GLOB_RECURSE SRC_FILES *.cpp)
add_library(tarsmock ${SRC_FILES})

add_dependencies(tarsmock tarsservant)
install(DIRECTORY . DESTINATION include/mock FILES_MATCHING PATTERN "*.h")

install(TARGETS tarsmock RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

