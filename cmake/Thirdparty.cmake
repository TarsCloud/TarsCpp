
option(TARS_MYSQL "option for mysql" ON)
if(UNIX)
option(TARS_GZIP "option for gzip" ON)
else(UNIX)
option(TARS_GZIP "option for gzip" OFF)
endif(UNIX)
option(TARS_SSL "option for ssl" OFF)
option(TARS_HTTP2 "option for http2" OFF)
option(TARS_PROTOBUF "option for protocol" OFF)

if (TARS_MYSQL)
    add_definitions(-DTARS_MYSQL=1)
endif ()

if (TARS_GZIP)
    add_definitions(-DTARS_GZIP=1)
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
set(LIB_GZIP)
set(LIB_HTTP2)
set(LIB_SSL)
set(LIB_CRYPTO)
set(LIB_PROTOBUF)
set(LIB_GTEST)

#-------------------------------------------------------------

add_custom_target(thirdparty)

include(ExternalProject)

if(WIN32)

    ExternalProject_Add(ADD_CURL
        URL https://cnb.cool/tarsyun/tars/src/-/packages/maven/tars:curl/-/tag/7.75.0/-/file/curl-7.75.0.tar.gz
        DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/download
        PREFIX ${CMAKE_BINARY_DIR}
        INSTALL_DIR ${CMAKE_SOURCE_DIR}
        CONFIGURE_COMMAND ${CMAKE_COMMAND} . -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/curl -DBUILD_TESTING=OFF
        SOURCE_DIR ${CMAKE_BINARY_DIR}/src/curl-lib
        BUILD_IN_SOURCE 1
        BUILD_COMMAND ${CMAKE_COMMAND} --build . --config release -- /maxcpucount:4
        INSTALL_COMMAND ${CMAKE_COMMAND} --build . --config release --target install
        URL_MD5 2071994cfc5079d03439915f2751c8bc
    )

    add_dependencies(thirdparty ADD_CURL)

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/curl/ DESTINATION thirdparty)
endif(WIN32)

if (WIN32)
    set(LIB_GTEST "gtest")

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(LIB_GTEST "${LIB_GTEST}d")
    endif()

    ExternalProject_Add(ADD_${LIB_GTEST}
            URL https://cnb.cool/tarsyun/tars/src/-/packages/maven/tars:gtest/-/tag/1.10.0/-/file/gtest-1.10.0.zip
            DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/download
            PREFIX ${CMAKE_BINARY_DIR}
            INSTALL_DIR ${CMAKE_SOURCE_DIR}
            CONFIGURE_COMMAND ${CMAKE_COMMAND} . -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/gtest -A x64 -Dgtest_force_shared_crt=on -DBUILD_GMOCK=OFF
            SOURCE_DIR ${CMAKE_BINARY_DIR}/src/gtest-lib
            BUILD_IN_SOURCE 1
            BUILD_COMMAND ${CMAKE_COMMAND} --build . --config ${CMAKE_BUILD_TYPE} -- /maxcpucount:4
            INSTALL_COMMAND ${CMAKE_COMMAND} --build . --config  ${CMAKE_BUILD_TYPE}  --target install
            URL_MD5 82358affdd7ab94854c8ee73a180fc53
            )
else()
    set(LIB_GTEST "gtest")

    ExternalProject_Add(ADD_${LIB_GTEST}
            URL https://cnb.cool/tarsyun/tars/src/-/packages/maven/tars:gtest.fixed/-/tag/1.10.0/-/file/gtest.fixed-1.10.0.tar.gz
            DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/download
            PREFIX ${CMAKE_BINARY_DIR}
            INSTALL_DIR ${CMAKE_SOURCE_DIR}
            CONFIGURE_COMMAND ${CMAKE_COMMAND} . -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/gtest -DBUILD_GMOCK=OFF -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
            SOURCE_DIR ${CMAKE_BINARY_DIR}/src/gtest-lib
            BUILD_IN_SOURCE 1
            BUILD_COMMAND make  -j4
            URL_MD5 6f26d634fa9cac718263c2df20df21a4
            )
endif()

INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/gtest/ DESTINATION thirdparty)

add_dependencies(thirdparty ADD_${LIB_GTEST})

if (TARS_PROTOBUF)
    set(PROTOBUF_DIR_INC "${THIRDPARTY_PATH}/protobuf/include")
    set(PROTOBUF_DIR_LIB "${THIRDPARTY_PATH}/protobuf/lib")
    set(PROTOBUF_DIR_LIB64 "${THIRDPARTY_PATH}/protobuf/lib64")
    include_directories(${PROTOBUF_DIR_INC})
    link_directories(${PROTOBUF_DIR_LIB})
    link_directories(${PROTOBUF_DIR_LIB64})

    if (WIN32)
        set(LIB_PROTOC "libprotoc")
        set(LIB_PROTOBUF "libprotobuf")

        ExternalProject_Add(ADD_${LIB_PROTOBUF}
                URL https://cnb.cool/tarsyun/tars/src/-/packages/maven/tars:protobuf-cpp/-/tag/3.11.3/-/file/protobuf-cpp-3.11.3.tar.gz 
                DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/download
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND ${CMAKE_COMMAND} cmake -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/protobuf -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/protobuf-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND ${CMAKE_COMMAND} --build . --config release -- /maxcpucount:4
                INSTALL_COMMAND ${CMAKE_COMMAND} --build . --config release --target install
                URL_MD5 fb59398329002c98d4d92238324c4187
                )
    else ()
        set(LIB_PROTOC "protoc")
        set(LIB_PROTOBUF "protobuf")

        ExternalProject_Add(ADD_${LIB_PROTOBUF}
                URL https://cnb.cool/tarsyun/tars/src/-/packages/maven/tars:protobuf-cpp/-/tag/3.11.3/-/file/protobuf-cpp-3.11.3.tar.gz
                DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/download
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND ${CMAKE_COMMAND} cmake -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/protobuf -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/protobuf-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND make  -j4
                URL_MD5 fb59398329002c98d4d92238324c4187
                )

    endif ()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/protobuf/ DESTINATION thirdparty)

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
                URL https://cnb.cool/tarsyun/tars/src/-/packages/maven/tars:openssl-1.1.1l/-/tag/1.1.1/-/file/openssl-1.1.1l-1.1.1.tar.gz
                DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/download
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND perl Configure --prefix=${CMAKE_BINARY_DIR}/src/openssl --openssldir=ssl VC-WIN64A no-asm
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/openssl-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND nmake
                INSTALL_COMMAND nmake install_sw
                URL_MD5 ac0d4387f3ba0ad741b0580dd45f6ff3
                )
    else ()
        set(LIB_SSL "ssl")
        set(LIB_CRYPTO "crypto")

        ExternalProject_Add(ADD_${LIB_SSL}
                URL https://cnb.cool/tarsyun/tars/src/-/packages/maven/tars:openssl-1.1.1l/-/tag/1.1.1/-/file/openssl-1.1.1l-1.1.1.tar.gz 
                DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/download
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND ./config --prefix=${CMAKE_BINARY_DIR}/src/openssl --openssldir=ssl 
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/openssl-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND make
                INSTALL_COMMAND make install_sw  -j4
                URL_MD5 ac0d4387f3ba0ad741b0580dd45f6ff3
                )

    endif ()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/openssl/ DESTINATION thirdparty)

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
                URL https://cnb.cool/tarsyun/tars/src/-/packages/maven/tars:mysql-connector-c-src.fixed/-/tag/6.1.11/-/file/mysql-connector-c-src.fixed-6.1.11.tar.gz
                DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/download
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND ${CMAKE_COMMAND} . -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/mysql -DBUILD_CONFIG=mysql_release -DDEFAULT_CHARSET=utf8mb4 -DDEFAULT_COLLATION=utf8mb4_general_ci
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/mysql-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND ${CMAKE_COMMAND} --build . --config release -- /maxcpucount:4
                INSTALL_COMMAND ${CMAKE_COMMAND} --build . --config release --target install
                URL_MD5 3578d736b9d493eae076a67e3ed473eb
                )

    else ()
        set(LIB_MYSQL "mysqlclient")

        ExternalProject_Add(ADD_${LIB_MYSQL}
                URL https://cnb.cool/tarsyun/tars/src/-/packages/maven/tars:mysql-connector-c-src.fixed/-/tag/6.1.11/-/file/mysql-connector-c-src.fixed-6.1.11.zip
                DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/download
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND ${CMAKE_COMMAND} .  -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/mysql -DDEFAULT_CHARSET=utf8mb4 -DDEFAULT_COLLATION=utf8mb4_general_ci -DSTACK_DIRECTION=1 -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/mysql-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND make mysqlclient  -j4
                # URL_MD5 3578d736b9d493eae076a67e3ed473eb
                # URL_MD5 b97bd3b3f826c70ed3de4983840fff56
                URL_MD5 bad636fe9bcc9bb62e3f5b784495a9b5
                )

    endif ()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/mysql/lib DESTINATION thirdparty)
    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/mysql/include/ DESTINATION thirdparty/include/mysql)

    add_dependencies(thirdparty ADD_${LIB_MYSQL})
endif ()


if (TARS_GZIP)
    set(GZIP_DIR_INC "${THIRDPARTY_PATH}/zlib/include")
    set(GZIP_DIR_LIB "${THIRDPARTY_PATH}/zlib/lib")
    include_directories(${GZIP_DIR_INC})
    link_directories(${GZIP_DIR_LIB})

    if (WIN32)
        set(LIB_GZIP "zlib")

        ExternalProject_Add(ADD_${LIB_GZIP}
                URL https://cnb.cool/tarsyun/tars/src/-/packages/maven/tars:zlib/-/tag/1.3.1/-/file/zlib-1.3.1.tar.gz
                DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/download
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND ${CMAKE_COMMAND} . -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/zlib
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/zlib-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND ${CMAKE_COMMAND} --build . --config release -- /maxcpucount:4
                INSTALL_COMMAND ${CMAKE_COMMAND} --build . --config release --target install
                URL_MD5 1c9f62f0778697a09d36121ead88e08e
                )

    else ()
        set(LIB_GZIP "z")

        ExternalProject_Add(ADD_${LIB_GZIP}
                URL https://cnb.cool/tarsyun/tars/src/-/packages/maven/tars:zlib/-/tag/1.3.1/-/file/zlib-1.3.1.tar.gz 
                DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/download
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND ${CMAKE_COMMAND} .  -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/zlib -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} 
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/zlib-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND make -j4
                URL_MD5 9855b6d802d7fe5b7bd5b196a2271655
                )

    endif ()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/zlib/lib DESTINATION thirdparty)
    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/zlib/include/ DESTINATION thirdparty/include/zlib)

    add_dependencies(thirdparty ADD_${LIB_GZIP})
endif ()

if (TARS_HTTP2)

    set(NGHTTP2_DIR_INC "${THIRDPARTY_PATH}/nghttp2/include/")
    set(NGHTTP2_DIR_LIB "${THIRDPARTY_PATH}/nghttp2/lib")
    set(NGHTTP2_DIR_LIB64 "${THIRDPARTY_PATH}/nghttp2/lib64")
    include_directories(${NGHTTP2_DIR_INC})
    link_directories(${NGHTTP2_DIR_LIB})
    link_directories(${NGHTTP2_DIR_LIB64})

    if(ENABLE_SHARED)
        set(LIB_HTTP2 "nghttp2")
    else()
        set(LIB_HTTP2 "nghttp2_static")
    endif()

    if (WIN32)
        ExternalProject_Add(ADD_${LIB_HTTP2}
                URL https://cnb.cool/tarsyun/tars/src/-/packages/maven/tars:nghttp2/-/tag/1.40.0/-/file/nghttp2-1.40.0.tar.gz
                DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/download
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND ${CMAKE_COMMAND} .  -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/nghttp2 -DENABLE_LIB_ONLY=ON
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/nghttp2-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND ${CMAKE_COMMAND} --build . --config release -- /maxcpucount:4
                INSTALL_COMMAND ${CMAKE_COMMAND} --build . --config release --target install
                URL_MD5 5df375bbd532fcaa7cd4044b54b1188d
                )

    else ()
        ExternalProject_Add(ADD_${LIB_HTTP2}
                URL https://cnb.cool/tarsyun/tars/src/-/packages/maven/tars:nghttp2/-/tag/1.40.0/-/file/nghttp2-1.40.0.tar.gz
                DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/download
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND ${CMAKE_COMMAND} . -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/nghttp2 -DENABLE_LIB_ONLY=ON -DENABLE_STATIC_LIB=ON -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/nghttp2-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND make  -j4
                URL_MD5 5df375bbd532fcaa7cd4044b54b1188d
                )

    endif ()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/nghttp2/ DESTINATION thirdparty)

    add_dependencies(thirdparty ADD_${LIB_HTTP2})

endif ()

message("----------------------------------------------------")
message("TARS_MYSQL:                ${TARS_MYSQL}")
message("TARS_GZIP:                 ${TARS_GZIP}")
message("TARS_HTTP2:                ${TARS_HTTP2}")
message("TARS_SSL:                  ${TARS_SSL}")
message("TARS_PROTOBUF:             ${TARS_PROTOBUF}")
#message("TARS_GPERF:                ${TARS_GPERF}")
