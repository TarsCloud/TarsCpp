# if(TARS_OPENTRACKING)
# include_directories(${PROJECT_SOURCE_DIR} ${OPENTRACKING_INC})
# else()
include_directories(${PROJECT_SOURCE_DIR})
# endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

aux_source_directory(. DIR_SRCS)

add_library(tarsservant STATIC ${DIR_SRCS})

add_dependencies(tarsservant tarsparse COPY-SERVENT-TARS)

install(TARGETS tarsservant
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

if(ENABLE_SHARED)
    add_definitions(-DSVT_DLL_EXPORT -DUTIL_USE_DLL)
    add_library(tarsservant_shared SHARED ${DIR_SRCS})

    # target_compile_definitions(tarsservant_shared PRIVATE SVT_DLL_EXPORT UTIL_USE_DLL)
    target_link_libraries(tarsservant_shared tarsutil_shared tarsparse_shared)

    add_dependencies(tarsservant_shared tarsservant tarsutil_shared tarsparse_shared)
    
    install(TARGETS tarsservant_shared
            RUNTIME DESTINATION lib
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
    if (WIN32)
        install(FILES $<TARGET_PDB_FILE:tarsservant_shared> DESTINATION bin)
    endif()
endif()
