
if (WIN32)
    option(TARS_MYSQL "option for mysql" OFF)
else ()
    option(TARS_MYSQL "option for mysql" ON)
endif ()

option(TARS_SSL "option for ssl" OFF)
option(TARS_HTTP2 "option for http2" OFF)
option(TARS_PROTOBUF "option for protocol" OFF)

if (TARS_MYSQL)
    add_definitions(-DTARS_MYSQL=1)
    if(WIN32)
    set(TARS_SSL ON)
    endif()

    IF(UNIX AND NOT APPLE)
        FIND_PACKAGE(Curses) 
        IF(NOT CURSES_FOUND)
            SET(ERRORMSG "Curses library not found. Please install appropriate package,
            remove CMakeCache.txt and rerun cmake.")
            IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
                SET(ERRORMSG ${ERRORMSG} 
                "On Debian/Ubuntu, package name is libncurses5-dev(apt-get install libncurses5-dev), on Redhat/Centos and derivates " 
                "it is ncurses-devel (yum install ncurses-devel).")
        ENDIF()
        MESSAGE(FATAL_ERROR ${ERRORMSG})
        ENDIF()
   ENDIF()

endif ()

if (TARS_SSL)
    add_definitions(-DTARS_SSL=1)
endif ()

if (TARS_HTTP2)
    add_definitions(-DTARS_HTTP2=1)
endif ()

if (TARS_PROTOBUF)
    add_definitions(-DTARS_PROTOBUF=1)
endif ()

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

if (TARS_PROTOBUF)
    set(PROTOBUF_DIR_INC "${THIRDPARTY_PATH}/protobuf/include")
    set(PROTOBUF_DIR_LIB "${THIRDPARTY_PATH}/protobuf/lib")
    include_directories(${PROTOBUF_DIR_INC})
    link_directories(${PROTOBUF_DIR_LIB})

    if (WIN32)
        set(LIB_PROTOC "libprotoc")
        set(LIB_PROTOBUF "libprotobuf")

        ExternalProject_Add(ADD_${LIB_PROTOBUF}
                URL http://cdn.tarsyun.com/src/protobuf-cpp-3.11.3.tar.gz
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake cmake -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/protobuf -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/protobuf-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND cmake --build . --config release
                LOG_CONFIGURE 1
                LOG_BUILD 1
                # INSTALL_COMMAND cmake -P ${RUN_PROTOBUF_INSTALL_FILE}
                URL_MD5 fb59398329002c98d4d92238324c4187
                )
    else ()
        set(LIB_PROTOC "protoc")
        set(LIB_PROTOBUF "protobuf")

        ExternalProject_Add(ADD_${LIB_PROTOBUF}
                URL http://cdn.tarsyun.com/src/protobuf-cpp-3.11.3.tar.gz
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake cmake -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/protobuf -DBUILD_SHARED_LIBS=OFF
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/protobuf-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND make
                LOG_CONFIGURE 1
                LOG_BUILD 1
                # INSTALL_COMMAND cmake -P ${RUN_PROTOBUF_INSTALL_FILE}
                URL_MD5 fb59398329002c98d4d92238324c4187
                )

    endif ()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/protobuf/lib DESTINATION .)
    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/protobuf/include/google DESTINATION include)

    add_dependencies(thirdparty ADD_${LIB_PROTOBUF})

endif ()


if (TARS_SSL)
    set(SSL_DIR "${THIRDPARTY_PATH}/openssl")
    set(SSL_DIR_INC "${THIRDPARTY_PATH}/openssl/include/")
    set(SSL_DIR_LIB "${THIRDPARTY_PATH}/openssl/lib")
    include_directories(${SSL_DIR_INC})
    link_directories(${SSL_DIR_LIB})

    if (WIN32)
        set(LIB_SSL "libssl")
        set(LIB_CRYPTO "libcrypto")

        ExternalProject_Add(ADD_${LIB_SSL}
                DEPENDS ${LIB_ZLIB}
                URL http://cdn.tarsyun.com/src/openssl-1.1.1d.tar.gz
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND perl Configure --prefix=${CMAKE_BINARY_DIR}/src/openssl VC-WIN64A no-asm 
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/openssl-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND nmake
                LOG_CONFIGURE 1
                LOG_BUILD 1
                INSTALL_COMMAND nmake install
                URL_MD5 3be209000dbc7e1b95bcdf47980a3baa
                )
    else ()
        set(LIB_SSL "ssl")
        set(LIB_CRYPTO "crypto")

        ExternalProject_Add(ADD_${LIB_SSL}
                DEPENDS ${LIB_ZLIB}
                URL http://cdn.tarsyun.com/src/openssl-1.1.1d.tar.gz
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND ./config --prefix=${CMAKE_BINARY_DIR}/src/openssl no-shared
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/openssl-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND make
                LOG_CONFIGURE 1
                LOG_BUILD 1
                # INSTALL_COMMAND cmake -P ${RUN_SSL_INSTALL_FILE}
                URL_MD5 3be209000dbc7e1b95bcdf47980a3baa
                )

    endif ()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/openssl/lib DESTINATION .)
    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/openssl/include/openssl DESTINATION include)

    add_dependencies(thirdparty ADD_${LIB_SSL})
endif ()

if (TARS_MYSQL)
    set(MYSQL_DIR_INC "${THIRDPARTY_PATH}/mysql/include")
    set(MYSQL_DIR_LIB "${THIRDPARTY_PATH}/mysql/lib")
    include_directories(${MYSQL_DIR_INC})
    link_directories(${MYSQL_DIR_LIB})

    if (WIN32)
        set(LIB_MYSQL "libmysql")

        ExternalProject_Add(ADD_${LIB_MYSQL}
                URL http://cdn.tarsyun.com/src/mysql-connector-c-6.1.11-src.zip
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake . -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/mysql -DBUILD_CONFIG=mysql_release 
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/mysql-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND cmake --build . --config release 
                LOG_CONFIGURE 1
                LOG_BUILD 1
                INSTALL_COMMAND cmake --build . --config release --target install
                URL_MD5 62de01beffc48348708c983a585b4dc1
                )

    else ()
        set(LIB_MYSQL "mysqlclient")

        ExternalProject_Add(ADD_${LIB_MYSQL}
                URL http://cdn.tarsyun.com/src/mysql-connector-c-6.1.11-src.tar.gz
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake .  -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/mysql -DDEFAULT_CHARSET=utf8 -DDEFAULT_COLLATION=utf8_general_ci -DDISABLE_SHARED=1
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/mysql-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND make mysqlclient
                LOG_CONFIGURE 1
                LOG_BUILD 1
                # INSTALL_COMMAND cmake --build . --config release --target install
                URL_MD5 98ca2071f9d4c6b73146cc0455f6b914
                )

    endif ()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/mysql/lib DESTINATION .)

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/mysql/include/mysql DESTINATION include)

    add_dependencies(thirdparty ADD_${LIB_MYSQL})
endif ()


if (TARS_HTTP2)

    set(NGHTTP2_DIR_INC "${THIRDPARTY_PATH}/nghttp2/include/")
    set(NGHTTP2_DIR_LIB "${THIRDPARTY_PATH}/nghttp2/lib")
    include_directories(${NGHTTP2_DIR_INC})
    link_directories(${NGHTTP2_DIR_LIB})
    set(LIB_HTTP2 "nghttp2_static")

    if (WIN32)
        ExternalProject_Add(ADD_${LIB_HTTP2}
                URL http://cdn.tarsyun.com/src/nghttp2-1.40.0.tar.gz
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake .  -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/nghttp2 -DENABLE_LIB_ONLY=ON
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/nghttp2-lib
                BUILD_IN_SOURCE 1
                LOG_BUILD 1
                LOG_CONFIGURE 1
                BUILD_COMMAND cmake --build . --config release
                INSTALL_COMMAND cmake --build . --config release --target install
                URL_MD5 5df375bbd532fcaa7cd4044b54b1188d
                )

    else ()
        ExternalProject_Add(ADD_${LIB_HTTP2}
                URL http://cdn.tarsyun.com/src/nghttp2-1.40.0.tar.gz
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake . -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/nghttp2 -DENABLE_LIB_ONLY=ON
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/nghttp2-lib
                BUILD_IN_SOURCE 1
                LOG_BUILD 1
                LOG_CONFIGURE 1
                BUILD_COMMAND make
                # INSTALL_COMMAND incmake --build . --config release --target install
                URL_MD5 5df375bbd532fcaa7cd4044b54b1188d
                )

    endif ()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/nghttp2/lib DESTINATION .)
    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/nghttp2/include/nghttp2 DESTINATION include)

    add_dependencies(thirdparty ADD_${LIB_HTTP2})

endif ()

message("----------------------------------------------------")
message("TARS_MYSQL:                ${TARS_MYSQL}")
message("TARS_HTTP2:                ${TARS_HTTP2}")
message("TARS_SSL:                  ${TARS_SSL}")
message("TARS_PROTOBUF:             ${TARS_PROTOBUF}")

