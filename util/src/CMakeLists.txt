set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. DIR_SRCS)

add_library(tarsutil ${DIR_SRCS})

if(${TARS_SSL} EQUAL 1)
    target_link_libraries(tarsutil; crypto; ssl)
else()
    target_link_libraries(tarsutil)
endif()
if(${TARS_HTTP2} EQUAL 1)
    target_link_libraries(tarsutil; nghttp2)
else()
    target_link_libraries(tarsutil)
endif()

install(TARGETS tarsutil
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
