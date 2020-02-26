set(MODULE "pb2tarscpp")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

if(WIN32)
add_definitions(-DPROTOBUF_USE_DLLS)
endif()

aux_source_directory(. DIR_SRCS)
        
add_executable(pb2tarscpp  ${DIR_SRCS})


target_link_libraries(pb2tarscpp ${LIB_PROTOBUF} ${LIB_PROTOC})

install(TARGETS "pb2tarscpp"
        RUNTIME DESTINATION tools)

