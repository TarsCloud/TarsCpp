
macro(build_tars_server MODULE DEPS)

    project(${MODULE})

    include_directories(./)

    aux_source_directory(. DIR_SRCS)

    FILE(GLOB TARS_LIST "${CMAKE_CURRENT_SOURCE_DIR}/*.tars")
    FILE(GLOB PB_LIST "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")

    set(TARS_LIST_DEPENDS)
    set(PB_LIST_DEPENDS)
    if (TARS_LIST)
        set(CLEAN_LIST)

        foreach (TARS_SRC ${TARS_LIST})
            get_filename_component(NAME_WE ${TARS_SRC} NAME_WE)

            set(TARS_H ${NAME_WE}.h)

            set(CUR_TARS_GEN ${CMAKE_CURRENT_SOURCE_DIR}/${TARS_H})
            LIST(APPEND TARS_LIST_DEPENDS ${CUR_TARS_GEN})
            
            add_custom_command(OUTPUT ${CUR_TARS_GEN}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    DEPENDS ${TARS2CPP} 
                    COMMAND ${TARS2CPP} ${TARS_SRC}
                    COMMENT "${TARS2CPP} ${TARS_SRC}")

            list(APPEND CLEAN_LIST ${CMAKE_CURRENT_SOURCE_DIR}/${TARS_H})

        endforeach ()

        set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_LIST}")

        set(TARS_TARGET "TARS_${MODULE}")  
        add_custom_target(${TARS_TARGET} ALL DEPENDS ${TARS_LIST_DEPENDS} tars2cpp)

        add_executable(${MODULE} ${DIR_SRCS})

        add_dependencies(${MODULE} ${TARS_TARGET})
        
    elseif(PB_LIST)
        set(CLEAN_LIST)
        set(_PROTOBUF_PROTOC ${CMAKE_BINARY_DIR}/src/protobuf/bin/protoc)

        foreach (PB_SRC ${PB_LIST})
            get_filename_component(NAME_WE ${PB_SRC} NAME_WE)

            set(PB_H ${NAME_WE}.pb.h)
            set(PB_CC ${NAME_WE}.pb.cc)

            set(CUR_PB_GEN ${CMAKE_CURRENT_SOURCE_DIR}/${PB_H} ${CMAKE_CURRENT_SOURCE_DIR}/${PB_CC})
            LIST(APPEND PB_LIST_DEPENDS ${CUR_PB_GEN})

            add_custom_command(OUTPUT ${CUR_PB_GEN}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    DEPENDS ${PROTO2TARS} ${_PROTOBUF_PROTOC}
                    COMMAND ${_PROTOBUF_PROTOC} -I "${CMAKE_CURRENT_SOURCE_DIR}" 
                                "${PB_SRC}" --cpp_out "${CMAKE_CURRENT_SOURCE_DIR}"        
                    COMMENT "${_PROTOBUF_PROTOC} ${PB_SRC} ${CMAKE_CURRENT_SOURCE_DIR} ${CUR_PB_GEN}")

            list(APPEND CLEAN_LIST ${CMAKE_CURRENT_SOURCE_DIR}/${PB_H} ${CMAKE_CURRENT_SOURCE_DIR}/${PB_CC})
        endforeach ()

        set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_LIST}")

        set(TARS_TARGET "TARS_${MODULE}")  
        add_custom_target(${TARS_TARGET} ALL DEPENDS ${PB_LIST_DEPENDS})
        add_executable(${MODULE} ${CLEAN_LIST} ${DIR_SRCS})
        add_dependencies(${MODULE} ${TARS_TARGET})
    else()
        add_executable(${MODULE} ${DIR_SRCS})
    endif()

    if("${DEPS}" STREQUAL "")
        add_dependencies(${MODULE} tarsservant tarsutil)
    else()
        string(REPLACE " " ";" DEP_LIST ${DEPS})
        add_dependencies(${MODULE} ${DEP_LIST} tarsservant tarsutil)
    endif()

    target_link_libraries(${MODULE} tarsservant tarsutil)

    if(TARS_SSL)
        target_link_libraries(${MODULE} tarsservant tarsutil ${LIB_SSL} ${LIB_CRYPTO})

        if(WIN32)
            target_link_libraries(${MODULE} Crypt32)
        endif()
    endif()

    if(TARS_HTTP2)
        target_link_libraries(${MODULE} ${LIB_HTTP2} ${LIB_PROTOBUF})
    endif()

    if(TARS_GPERF)
        target_link_libraries(${MODULE} ${LIB_GPERF})
    endif(TARS_GPERF)

    SET(MODULE-TGZ "${CMAKE_BINARY_DIR}/${MODULE}.tgz")
    SET(RUN_DEPLOY_COMMAND_FILE "${PROJECT_BINARY_DIR}/run-deploy-${MODULE}.cmake")
    FILE(WRITE ${RUN_DEPLOY_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/deploy/${MODULE})\n")
    IF(WIN32)
        # message(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/${MODULE}.exe)
        FILE(APPEND ${RUN_DEPLOY_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/${MODULE}.exe ${PROJECT_BINARY_DIR}/deploy/${MODULE}/)\n")
        FILE(APPEND ${RUN_DEPLOY_COMMAND_FILE} "EXECUTE_PROCESS(WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/deploy/ \n COMMAND ${CMAKE_COMMAND} -E tar czfv ${MODULE-TGZ} ${MODULE})\n")
    ELSE()
        FILE(APPEND ${RUN_DEPLOY_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${MODULE} ${PROJECT_BINARY_DIR}/deploy/${MODULE}/)\n")
        FILE(APPEND ${RUN_DEPLOY_COMMAND_FILE} "EXECUTE_PROCESS(WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/deploy/ \n COMMAND ${CMAKE_COMMAND} -E tar czfv ${MODULE-TGZ} ${MODULE})\n")
    ENDIF()

    #执行命令
    add_custom_command(OUTPUT ${MODULE-TGZ}
            DEPENDS ${MODULE}
            COMMAND ${CMAKE_COMMAND} -P ${RUN_DEPLOY_COMMAND_FILE}
            COMMENT "call ${RUN_DEPLOY_COMMAND_FILE}")
    
    add_custom_target(${MODULE}-tar DEPENDS ${MODULE-TGZ})
                
endmacro()

#-----------------------------------------------------------------------
