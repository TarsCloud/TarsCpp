
add_subdirectory(HttpClient)
add_subdirectory(HttpServer)

if(TARS_HTTP2)
add_subdirectory(Http2Client)
add_subdirectory(Http2Server)
endif()