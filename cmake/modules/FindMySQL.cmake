# - Try to find MySQL.
# Once done this will define:
# MYSQL_FOUND			- If false, do not try to use MySQL.
# MYSQL_INCLUDE_DIRS	- Where to find mysql.h, etc.
# MYSQL_LIBRARIES		- The libraries to link against.
# MYSQL_VERSION_STRING	- Version in a string of MySQL.
#
# Created by RenatoUtsch based on eAthena implementation.
#
#

find_path(MYSQL_INCLUDE_DIR
        NAMES mysql.h
        PATHS /usr/local/opt/mysql-client/include/mysql /usr/include/mysql /usr/local/include/mysql /usr/local/mysql/include/
            $ENV{PROGRAMFILES}/MySQL/*/include $ENV{SYSTEMDRIVE}/MySQL/*/include C:/mysql/include)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".lib")

find_library(MYSQL_LIBRARY
        NAMES mysqlclient mysqlclient_r
        PATHS /usr/local/opt/mysql-client/lib /lib/mysql /lib64/mysql /usr/lib/mysql /usr/lib64/mysql /usr/local/lib/mysql /usr/local/lib64/mysql /usr/local/mysql/lib/
            $ENV{PROGRAMFILES}/MySQL/*/lib $ENV{SYSTEMDRIVE}/MySQL/*/lib C:/mysql/lib/vs14)

find_path(SSL_INCLUDE_DIR
        NAMES openssl/ssl.h
        PATHS
        /usr/local/Cellar/openssl@1.1/1.1.1d /usr/include /usr/local/include /opt/local/include /sw/include
        PATH_SUFFIXES
        include)

find_library(_SSL_LIB
        NAMES ssl
        PATHS
        /usr/local/Cellar/openssl@1.1/1.1.1d /usr/lib /usr/local/lib /opt/local/lib /sw/lib
        PATH_SUFFIXES
        lib)

find_library(_CRYPTO_LIB
        NAMES crypto
        PATHS
        /usr/local/Cellar/openssl@1.1/1.1.1d /usr/lib /usr/local/lib /opt/local/lib /sw/lib
        PATH_SUFFIXES
        lib)


if (MYSQL_INCLUDE_DIR AND EXISTS "${MYSQL_INCLUDE_DIR}/mysql_version.h")
    file(STRINGS "${MYSQL_INCLUDE_DIR}/mysql_version.h"
            MYSQL_VERSION_H REGEX "^#define[ \t]+MYSQL_SERVER_VERSION[ \t]+\"[^\"]+\".*$")
    string(REGEX REPLACE
            "^.*MYSQL_SERVER_VERSION[ \t]+\"([^\"]+)\".*$" "\\1" MYSQL_VERSION_STRING
            "${MYSQL_VERSION_H}")
endif ()

# handle the QUIETLY and REQUIRED arguments and set MYSQL_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MYSQL DEFAULT_MSG MYSQL_LIBRARY MYSQL_INCLUDE_DIR MYSQL_VERSION_STRING)

if(MYSQL_FOUND)
    set(MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR})
    set(MYSQL_LIBRARIES ${MYSQL_LIBRARY})
endif()

mark_as_advanced(MYSQL_INCLUDE_DIR MYSQL_LIBRARY)

find_package_handle_standard_args(ssl DEFAULT_MSG _SSL_LIB _CRYPTO_LIB SSL_INCLUDE_DIR)
mark_as_advanced(SSL_INCLUDE_DIR _SSL_LIB _CRYPTO_LIB)

if(SSL_FOUND)
    set(MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR} ${SSL_INCLUDE_DIR})
    set(MYSQL_LIBRARIES ${MYSQL_LIBRARY} ${_SSL_LIB} ${_CRYPTO_LIB})
endif()

message("------------MySQL------------")
message("MYSQL_VERSION_STRING=${MYSQL_VERSION_STRING}")
message("MYSQL_INCLUDE_DIRS=${MYSQL_INCLUDE_DIRS}")
message("MYSQL_LIBRARIES=${MYSQL_LIBRARIES}")
message("------------MySQL------------")