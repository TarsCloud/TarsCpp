
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

        SET(RUN_PROTOBUF_INSTALL_FILE "${PROJECT_BINARY_DIR}/run-protobuf-install.cmake")
        FILE(WRITE ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/protobuf)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/protobuf/lib)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/protobuf/include/google)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/protobuf-lib/${CMAKE_BUILD_TYPE}/libprotoc.dll ${CMAKE_BINARY_DIR}/src/protobuf/lib)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/protobuf-lib/${CMAKE_BUILD_TYPE}/libprotoc.lib ${CMAKE_BINARY_DIR}/src/protobuf/lib)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/protobuf-lib/${CMAKE_BUILD_TYPE}/libprotobuf.dll ${CMAKE_BINARY_DIR}/src/protobuf/lib)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/protobuf-lib/${CMAKE_BUILD_TYPE}/libprotobuf.lib ${CMAKE_BINARY_DIR}/src/protobuf/lib)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/protobuf-lib/${CMAKE_BUILD_TYPE}/protoc.exe ${CMAKE_BINARY_DIR}/src/protobuf/lib)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy_directory ${CMAKE_BINARY_DIR}/src/protobuf-lib/src/google ${CMAKE_BINARY_DIR}/src/protobuf/include/google)\n")

        ExternalProject_Add(ADD_${LIB_PROTOBUF}
                URL http://cdn.tarsyun.com/src/protobuf-cpp-3.11.3.tar.gz
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/protobuf-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND cmake --build . --config release
                LOG_CONFIGURE 1
                LOG_BUILD 1
                INSTALL_COMMAND cmake -P ${RUN_PROTOBUF_INSTALL_FILE}
                URL_MD5 fb59398329002c98d4d92238324c4187
                )

        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/protobuf/lib/libprotoc.dll DESTINATION lib)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/protobuf/lib/libprotoc.lib DESTINATION lib)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/protobuf/lib/libprotobuf.dll DESTINATION lib)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/protobuf/lib/libprotobuf.lib DESTINATION lib)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/protobuf/lib/protoc.exe DESTINATION bin)

    else ()
        set(LIB_PROTOC "protoc")
        set(LIB_PROTOBUF "protobuf")

        SET(RUN_PROTOBUF_INSTALL_FILE "${PROJECT_BINARY_DIR}/run-protobuf-install.cmake")
        FILE(WRITE ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/protobuf)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/protobuf/lib)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/protobuf/bin)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/protobuf/include/google)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/protobuf-lib/libprotoc.a ${CMAKE_BINARY_DIR}/src/protobuf/lib)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/protobuf-lib/libprotobuf.a ${CMAKE_BINARY_DIR}/src/protobuf/lib)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/protobuf-lib/protoc ${CMAKE_BINARY_DIR}/src/protobuf/bin)\n")
        FILE(APPEND ${RUN_PROTOBUF_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy_directory ${CMAKE_BINARY_DIR}/src/protobuf-lib/src/google ${CMAKE_BINARY_DIR}/src/protobuf/include/google)\n")

        ExternalProject_Add(ADD_${LIB_PROTOBUF}
                URL http://cdn.tarsyun.com/src/protobuf-cpp-3.11.3.tar.gz
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake cmake -DBUILD_SHARED_LIBS=OFF
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/protobuf-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND make
                LOG_CONFIGURE 1
                LOG_BUILD 1
                INSTALL_COMMAND cmake -P ${RUN_PROTOBUF_INSTALL_FILE}
                URL_MD5 fb59398329002c98d4d92238324c4187
                )

        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/protobuf/lib/libprotoc.a DESTINATION lib)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/protobuf/lib/libprotobuf.a DESTINATION lib)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/protobuf/bin/protoc DESTINATION bin)

    endif ()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/protobuf/include/google DESTINATION include)

    add_dependencies(thirdparty ADD_${LIB_PROTOBUF})

endif ()


if (TARS_SSL)
    set(SSL_DIR_INC "${THIRDPARTY_PATH}/openssl/include/")
    set(SSL_DIR_LIB "${THIRDPARTY_PATH}/openssl/lib")
    include_directories(${SSL_DIR_INC})
    link_directories(${SSL_DIR_LIB})

    if (WIN32)
        set(LIB_SSL "libssl")
        set(LIB_CRYPTO "libcrypto")

        SET(RUN_SSL_INSTALL_FILE "${PROJECT_BINARY_DIR}/run-ssl-install.cmake")
        FILE(WRITE ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/openssl)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/openssl/lib)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/openssl/include/openssl)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/openssl-lib/libssl-1_1-x64.dll ${CMAKE_BINARY_DIR}/src/openssl/lib)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/openssl-lib/libssl.lib ${CMAKE_BINARY_DIR}/src/openssl/lib)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/openssl-lib/libssl_static.lib ${CMAKE_BINARY_DIR}/src/openssl/lib)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/openssl-lib/libcrypto-1_1-x64.dll ${CMAKE_BINARY_DIR}/src/openssl/lib)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/openssl-lib/libcrypto.lib ${CMAKE_BINARY_DIR}/src/openssl/lib)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/openssl-lib/libcrypto_static.lib ${CMAKE_BINARY_DIR}/src/openssl/lib)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy_directory ${CMAKE_BINARY_DIR}/src/openssl-lib/include/openssl ${CMAKE_BINARY_DIR}/src/openssl/include/openssl)\n")

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
                INSTALL_COMMAND cmake -P ${RUN_SSL_INSTALL_FILE}
                URL_MD5 3be209000dbc7e1b95bcdf47980a3baa
                )

        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/openssl/lib/libssl-1_1-x64.dll DESTINATION lib)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/openssl/lib/libssl.lib DESTINATION lib)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/openssl/lib/libssl_static.lib DESTINATION lib)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/openssl/lib/libcrypto-1_1-x64.dll DESTINATION lib)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/openssl/lib/libcrypto.lib DESTINATION lib)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/openssl/lib/libcrypto_static.lib DESTINATION lib)

    else ()
        set(LIB_SSL "ssl")
        set(LIB_CRYPTO "crypto")

        SET(RUN_SSL_INSTALL_FILE "${PROJECT_BINARY_DIR}/run-ssl-install.cmake")
        FILE(WRITE ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/openssl)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/openssl/lib)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/openssl/include/openssl)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/openssl-lib/libssl.a ${CMAKE_BINARY_DIR}/src/openssl/lib)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/openssl-lib/libcrypto.a ${CMAKE_BINARY_DIR}/src/openssl/lib)\n")
        FILE(APPEND ${RUN_SSL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy_directory ${CMAKE_BINARY_DIR}/src/openssl-lib/include/openssl ${CMAKE_BINARY_DIR}/src/openssl/include/openssl)\n")

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
                INSTALL_COMMAND cmake -P ${RUN_SSL_INSTALL_FILE}
                URL_MD5 3be209000dbc7e1b95bcdf47980a3baa
                )

        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/openssl/lib/libssl.a DESTINATION lib)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/openssl/lib/libcrypto.a DESTINATION lib)

    endif ()

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
        SET(RUN_MYSQL_INSTALL_FILE "${PROJECT_BINARY_DIR}/run-mysql-install.cmake")
        FILE(WRITE ${RUN_MYSQL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/mysql)\n")
        FILE(APPEND ${RUN_MYSQL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/mysql/lib)\n")
        FILE(APPEND ${RUN_MYSQL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/mysql/include)\n")
        FILE(APPEND ${RUN_MYSQL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/mysql-lib/libmysql/${CMAKE_BUILD_TYPE}/libmysql.lib ${CMAKE_BINARY_DIR}/src/mysql/lib)\n")
        FILE(APPEND ${RUN_MYSQL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/mysql-lib/libmysql/${CMAKE_BUILD_TYPE}/libmysql.dll ${CMAKE_BINARY_DIR}/src/mysql/lib)\n")
        FILE(APPEND ${RUN_MYSQL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy_directory ${CMAKE_BINARY_DIR}/src/mysql-lib/include ${CMAKE_BINARY_DIR}/src/mysql/include)\n")

        ExternalProject_Add(ADD_${LIB_MYSQL}
                URL http://cdn.tarsyun.com/src/mysql-5.6.46.zip
                DEPENDS ADD_${LIB_SSL}
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake . -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/src/mysql -DBUILD_CONFIG=mysql_release -DWITH_SSL=${CMAKE_BINARY_DIR}/src/openssl
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/mysql-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND cmake --build . --config ${CMAKE_BUILD_TYPE} --target libmysql
                LOG_CONFIGURE 1
                LOG_BUILD 1
                INSTALL_COMMAND cmake -P ${RUN_MYSQL_INSTALL_FILE}
                URL_MD5 851be8973981979041ad422f7e5f693a
                )

        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/mysql/lib/libmysql.lib DESTINATION lib)
        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/mysql/lib/libmysql.dll DESTINATION lib)

    else ()
        set(LIB_MYSQL "mysqlclient")
        SET(RUN_MYSQL_INSTALL_FILE "${PROJECT_BINARY_DIR}/run-mysql-install.cmake")
        FILE(WRITE ${RUN_MYSQL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/mysql)\n")
        FILE(APPEND ${RUN_MYSQL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/mysql/lib)\n")
        FILE(APPEND ${RUN_MYSQL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/mysql/include)\n")
        FILE(APPEND ${RUN_MYSQL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/mysql-lib/libmysql/libmysqlclient.a ${CMAKE_BINARY_DIR}/src/mysql/lib)\n")
        FILE(APPEND ${RUN_MYSQL_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy_directory ${CMAKE_BINARY_DIR}/src/mysql-lib/include ${CMAKE_BINARY_DIR}/src/mysql/include)\n")


        ExternalProject_Add(ADD_${LIB_MYSQL}
                URL http://cdn.tarsyun.com/src/mysql-5.6.26.tar.gz
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake . -DDEFAULT_CHARSET=utf8 -DDEFAULT_COLLATION=utf8_general_ci -DDISABLE_SHARED=1
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/mysql-lib
                BUILD_IN_SOURCE 1
                BUILD_COMMAND make mysqlclient
                LOG_CONFIGURE 1
                LOG_BUILD 1
                INSTALL_COMMAND cmake -P ${RUN_MYSQL_INSTALL_FILE}
                URL_MD5 c537c08c1276abc79d76e8e562bbcea5
                #URL_MD5 9d225528742c882d5b1e4a40b0877690
                )

        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/mysql/lib/libmysqlclient.a DESTINATION lib)

    endif ()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/mysql/include/mysql DESTINATION include)

    add_dependencies(thirdparty ADD_${LIB_MYSQL})

endif ()

if (TARS_HTTP2)

    set(NGHTTP2_DIR_INC "${THIRDPARTY_PATH}/nghttp2/include/")
    set(NGHTTP2_DIR_LIB "${THIRDPARTY_PATH}/nghttp2/lib")
    include_directories(${NGHTTP2_DIR_INC})
    link_directories(${NGHTTP2_DIR_LIB})

    if (WIN32)
        set(LIB_HTTP2 "nghttp2_static")
        SET(RUN_HTTP2_INSTALL_FILE "${PROJECT_BINARY_DIR}/run-http2-install.cmake")
        FILE(WRITE ${RUN_HTTP2_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/nghttp2)\n")
        FILE(APPEND ${RUN_HTTP2_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/nghttp2/lib)\n")
        FILE(APPEND ${RUN_HTTP2_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/nghttp2/include/nghttp2)\n")
        FILE(APPEND ${RUN_HTTP2_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/nghttp2-lib/lib/${CMAKE_BUILD_TYPE}/nghttp2_static.lib ${CMAKE_BINARY_DIR}/src/nghttp2/lib)\n")
        FILE(APPEND ${RUN_HTTP2_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy_directory ${CMAKE_BINARY_DIR}/src/nghttp2-lib/lib/includes/nghttp2 ${CMAKE_BINARY_DIR}/src/nghttp2/include/nghttp2)\n")

        ExternalProject_Add(ADD_${LIB_HTTP2}
                URL http://cdn.tarsyun.com/src/nghttp2-1.40.0.tar.gz
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake . -DENABLE_SHARED_LIB=OFF -DENABLE_STATIC_LIB=ON -DENABLE_LIB_ONLY=ON
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/nghttp2-lib
                BUILD_IN_SOURCE 1
                LOG_BUILD 1
                LOG_CONFIGURE 1
                BUILD_COMMAND cmake --build . --config release
                INSTALL_COMMAND cmake -P ${RUN_HTTP2_INSTALL_FILE}
                URL_MD5 5df375bbd532fcaa7cd4044b54b1188d
                )

        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/nghttp2/lib/nghttp2_static.lib DESTINATION lib)

    else ()
        set(LIB_HTTP2 "nghttp2_static")
        SET(RUN_HTTP2_INSTALL_FILE "${PROJECT_BINARY_DIR}/run-http2-install.cmake")
        FILE(WRITE ${RUN_HTTP2_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/nghttp2)\n")
        FILE(APPEND ${RUN_HTTP2_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/nghttp2/lib)\n")
        FILE(APPEND ${RUN_HTTP2_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/src/nghttp2/include/nghttp2)\n")
        FILE(APPEND ${RUN_HTTP2_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/src/nghttp2-lib/lib/libnghttp2_static.a ${CMAKE_BINARY_DIR}/src/nghttp2/lib)\n")
        FILE(APPEND ${RUN_HTTP2_INSTALL_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy_directory ${CMAKE_BINARY_DIR}/src/nghttp2-lib/lib/includes/nghttp2 ${CMAKE_BINARY_DIR}/src/nghttp2/include/nghttp2)\n")

        ExternalProject_Add(ADD_${LIB_HTTP2}
                URL http://cdn.tarsyun.com/src/nghttp2-1.40.0.tar.gz
                PREFIX ${CMAKE_BINARY_DIR}
                INSTALL_DIR ${CMAKE_SOURCE_DIR}
                CONFIGURE_COMMAND cmake . -DENABLE_SHARED_LIB=OFF -DENABLE_STATIC_LIB=ON -DENABLE_LIB_ONLY=ON
                SOURCE_DIR ${CMAKE_BINARY_DIR}/src/nghttp2-lib
                BUILD_IN_SOURCE 1
                LOG_BUILD 1
                LOG_CONFIGURE 1
                BUILD_COMMAND make
                INSTALL_COMMAND cmake -P ${RUN_HTTP2_INSTALL_FILE}
                URL_MD5 5df375bbd532fcaa7cd4044b54b1188d
                )

        INSTALL(FILES ${CMAKE_BINARY_DIR}/src/nghttp2/lib/libnghttp2_static.a DESTINATION lib)

    endif ()

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/src/nghttp2/include/nghttp2 DESTINATION include)

    add_dependencies(thirdparty ADD_${LIB_HTTP2})
endif ()


message("----------------------------------------------------")
message("TARS_MYSQL:                ${TARS_MYSQL}")
message("TARS_HTTP2:                ${TARS_HTTP2}")
message("TARS_SSL:                  ${TARS_SSL}")
message("TARS_PROTOBUF:             ${TARS_PROTOBUF}")

