set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

IF(WIN32)
include_directories(epoll_windows)
aux_source_directory(epoll_windows/src. DIR_SRCS)
ENDIF()

aux_source_directory(. DIR_SRCS)

parse_asm(JUMP_SRC MAKE_SRC)

message("----------------------------------------------------")
message("CMAKE_SYSTEM_NAME:         ${CMAKE_SYSTEM_NAME}")
message("CMAKE_SYSTEM_PROCESSOR:    ${CMAKE_SYSTEM_PROCESSOR}")
message("CMAKE_CXX_COMPILER_ID:     ${CMAKE_CXX_COMPILER_ID}")
message("ABI_STR:                   ${ABI_STR}")
message("BF_STR:                    ${BF_STR}")
message("CPU_STR:                   ${CPU_STR}")
message("JUMP_SRC:                  ${JUMP_SRC}")
message("MAKE_SRC:                  ${MAKE_SRC}")
message("CMAKE_C_SIZEOF_DATA_PTR:   ${CMAKE_C_SIZEOF_DATA_PTR}")
message("----------------------------------------------------")

list(APPEND DIR_SRCS ${JUMP_SRC})
list(APPEND DIR_SRCS ${MAKE_SRC})

add_library(tarsutil STATIC ${DIR_SRCS})

add_dependencies(tarsutil thirdparty)

install(TARGETS tarsutil
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

if(ENABLE_SHARED)
        ADD_DEFINITIONS( -DUTIL_DLL_EXPORT )
        add_library(tarsutil_shared SHARED ${DIR_SRCS})

        target_link_libraries(tarsutil_shared ${LIB_MYSQL})

        if (TARS_SSL)
                if (WIN32)
                        #windows动态编译需添加依赖库
                        target_link_libraries(tarsutil_shared ${LIB_SSL}.lib ${LIB_CRYPTO}.lib Crypt32)
                endif ()
        endif()
        add_dependencies(tarsutil_shared thirdparty)

        install(TARGETS tarsutil_shared
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib)
        if (WIN32)
                install(FILES $<TARGET_PDB_FILE:tarsutil_shared> DESTINATION bin)
        endif()
endif()

IF(WIN32)
install(DIRECTORY epoll_windows/sys
        DESTINATION include)
ENDIF()        
