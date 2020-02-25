
if(WIN32)
option(TARS_MYSQL "option for mysql" OFF)
else()
option(TARS_MYSQL "option for mysql" ON)
endif()

option(TARS_SSL "option for ssl" OFF)
option(TARS_HTTP2 "option for http2" OFF)
option(TARS_PROTOBUF "option for protocol" OFF)

if(TARS_MYSQL)
add_definitions(-DTARS_MYSQL=1)
endif()

if(TARS_SSL)
add_definitions(-DTARS_SSL=1)
endif()

if(TARS_HTTP2)
add_definitions(-DTARS_HTTP2=1)
endif()

if(TARS_PROTOBUF)
add_definitions(-DTARS_PROTOBUF=1)
endif()

#-------------------------------------------------------------

set(THIRDPARTY_PATH "${CMAKE_BINARY_DIR}/src")

set(LIB_MYSQL)
set(LIB_HTTP2)
set(LIB_SSL)
set(LIB_CRYPTO)
set(LIB_PROTOBUF)

#-------------------------------------------------------------

add_custom_target(thirdparty)

include(ExternalProject)

if(TARS_PROTOBUF)
    set(PROTOBUF_DIR_INC "${THIRDPARTY_PATH}/protobuf-lib/src")
    set(PROTOBUF_DIR_LIB "${THIRDPARTY_PATH}/protobuf-lib")
    include_directories(${PROTOBUF_DIR_INC})
    link_directories(${PROTOBUF_DIR_LIB})

    if(WIN32)
        set(LIB_PROTOBUF "liblibmysql")
    else()
        set(LIB_PROTOBUF "protoc")
    endif()

    ExternalProject_Add(ADD_${LIB_PROTOBUF}
        URL http://cdn.tarsyun.com/src/protobuf-cpp-3.11.3.tar.gz
        PREFIX    ${CMAKE_BINARY_DIR}
        INSTALL_DIR ${CMAKE_SOURCE_DIR}
        CONFIGURE_COMMAND cmake 
        SOURCE_DIR ${CMAKE_BINARY_DIR}/src/protobuf-lib
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make -j4 libprotoc
        LOG_CONFIGURE 1
        LOG_BUILD 1
        INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
        URL_MD5 fb59398329002c98d4d92238324c4187
        )

    add_dependencies(thirdparty ADD_${LIB_PROTOBUF})

endif()

if(TARS_MYSQL)
    set(MYSQL_DIR_INC "${THIRDPARTY_PATH}/mysql-lib/include")
    set(MYSQL_DIR_LIB "${THIRDPARTY_PATH}/mysql-lib/libmysql")
    include_directories(${MYSQL_DIR_INC})
    link_directories(${MYSQL_DIR_LIB})

    if(WIN32)
        set(LIB_MYSQL "libmysql")
        ExternalProject_Add(ADD_${LIB_MYSQL}
                URL http://cdn.tarsyun.com/src/mysql-5.6.28.zip
                PREFIX    ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake . -DDEFAULT_CHARSET=utf8 -DDEFAULT_COLLATION=utf8_general_ci -DDISABLE_SHARED=1
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/mysql-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND cmake --build . --config release --target mysqlclient
                LOG_CONFIGURE 1
                LOG_BUILD 1
                INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
                URL_MD5 17d927c8ed638a17ffbc00b662b3e4a0
                )
    else()
        set(LIB_MYSQL "mysqlclient")
        ExternalProject_Add(ADD_${LIB_MYSQL}
                URL http://cdn.tarsyun.com/src/mysql-5.6.26.tar.gz
                PREFIX    ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake . -DDEFAULT_CHARSET=utf8 -DDEFAULT_COLLATION=utf8_general_ci -DDISABLE_SHARED=1
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/mysql-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND make mysqlclient
                LOG_CONFIGURE 1
                LOG_BUILD 1
                INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
                URL_MD5 c537c08c1276abc79d76e8e562bbcea5
                #URL_MD5 9d225528742c882d5b1e4a40b0877690
                )
    endif()
        
    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/mysql-lib/include/mysql DESTINATION include)
    if(WIN32)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/mysql-lib/libmysql/${LIB_MYSQL}.dll DESTINATION lib)
    else()
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/mysql-lib/libmysql/lib${LIB_MYSQL}.a DESTINATION lib)
    endif()

    add_dependencies(thirdparty ADD_${LIB_MYSQL})

endif()

if(TARS_HTTP2)

    set(NGHTTP2_DIR_INC "${THIRDPARTY_PATH}/nghttp2-lib/lib/includes/")
    set(NGHTTP2_DIR_LIB "${THIRDPARTY_PATH}/nghttp2-lib/lib")
    include_directories(${NGHTTP2_DIR_INC})
    link_directories(${NGHTTP2_DIR_LIB})

    if(WIN32)
        set(LIB_HTTP2 "libnghttp2_static")

        ExternalProject_Add(ADD_${LIB_HTTP2}
            URL http://cdn.tarsyun.com/src/nghttp2-1.40.0.tar.gz
            PREFIX    ${CMAKE_BINARY_DIR}
            INSTALL_DIR ${CMAKE_SOURCE_DIR}
            CONFIGURE_COMMAND cmake . -DENABLE_SHARED_LIB=OFF -DENABLE_STATIC_LIB=ON -DENABLE_LIB_ONLY=ON
            SOURCE_DIR ${CMAKE_BINARY_DIR}/src/nghttp2-lib
            BUILD_IN_SOURCE 1
            LOG_BUILD 1
            LOG_CONFIGURE 1
            BUILD_COMMAND cmake --build . --config release
            INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
            URL_MD5 5df375bbd532fcaa7cd4044b54b1188d
            )
    else()
        set(LIB_HTTP2 "nghttp2_static")

        ExternalProject_Add(ADD_${LIB_HTTP2}
            URL http://cdn.tarsyun.com/src/nghttp2-1.40.0.tar.gz
            PREFIX    ${CMAKE_BINARY_DIR}
            INSTALL_DIR ${CMAKE_SOURCE_DIR}
            CONFIGURE_COMMAND cmake . -DENABLE_SHARED_LIB=OFF -DENABLE_STATIC_LIB=ON -DENABLE_LIB_ONLY=ON
            SOURCE_DIR ${CMAKE_BINARY_DIR}/src/nghttp2-lib
            BUILD_IN_SOURCE 1
            LOG_BUILD 1
            LOG_CONFIGURE 1
            BUILD_COMMAND make
            INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
            URL_MD5 5df375bbd532fcaa7cd4044b54b1188d
            )
    endif()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/nghttp2-lib/lib/includes/nghttp2 DESTINATION include)
    INSTALL(FILES ${CMAKE_BINARY_DIR}/src/nghttp2-lib/lib/lib${LIB_HTTP2}.a DESTINATION lib)    

    add_dependencies(thirdparty ADD_${LIB_HTTP2})
endif()

if(TARS_SSL)
    set(SSL_DIR_INC "${THIRDPARTY_PATH}/openssl-lib/include/")
    set(SSL_DIR_LIB "${THIRDPARTY_PATH}/openssl-lib")
    include_directories(${SSL_DIR_INC})
    link_directories(${SSL_DIR_LIB})

    if(WIN32)
        set(LIB_SSL "libssl")
        set(LIB_CRYPTO "libcrypto")

        ExternalProject_Add(ADD_${LIB_SSL}
            DEPENDS ${LIB_ZLIB}
            URL http://cdn.tarsyun.com/src/openssl-1.1.1d.tar.gz
            PREFIX    ${CMAKE_BINARY_DIR}
            INSTALL_DIR ${CMAKE_SOURCE_DIR}
            CONFIGURE_COMMAND ./config
            SOURCE_DIR ${CMAKE_BINARY_DIR}/src/openssl-lib
            BUILD_IN_SOURCE 1
            BUILD_COMMAND cmake --build . --config release
            LOG_CONFIGURE 1
            LOG_BUILD 1
            INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
            URL_MD5 3be209000dbc7e1b95bcdf47980a3baa
            )
 
    else()
        set(LIB_SSL "ssl")
        set(LIB_CRYPTO "crypto")

        ExternalProject_Add(ADD_${LIB_SSL}
            DEPENDS ${LIB_ZLIB}
            URL http://cdn.tarsyun.com/src/openssl-1.1.1d.tar.gz
            PREFIX    ${CMAKE_BINARY_DIR}
            INSTALL_DIR ${CMAKE_SOURCE_DIR}
            CONFIGURE_COMMAND ./config
            SOURCE_DIR ${CMAKE_BINARY_DIR}/src/openssl-lib
            BUILD_IN_SOURCE 1
            BUILD_COMMAND make
            LOG_CONFIGURE 1
            LOG_BUILD 1
            INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
            URL_MD5 3be209000dbc7e1b95bcdf47980a3baa
            )
    
    endif()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/openssl-lib/include/openssl DESTINATION include)
    INSTALL(FILES 
        ${CMAKE_BINARY_DIR}/src/openssl-lib/lib${LIB_SSL}.a 
        ${CMAKE_BINARY_DIR}/src/openssl-lib/lib${LIB_CRYPTO}.a 
        DESTINATION lib)

    add_dependencies(thirdparty ADD_${LIB_SSL})
endif()

message("----------------------------------------------------")
message("TARS_MYSQL:                ${TARS_MYSQL}")
message("TARS_HTTP2:                ${TARS_HTTP2}")
message("TARS_SSL:                  ${TARS_SSL}")
message("TARS_PROTOBUF:             ${TARS_PROTOBUF}")

