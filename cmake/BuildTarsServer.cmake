
macro(build_tars_server MODULE)

    include_directories(./)

    aux_source_directory(. DIR_SRCS)

    # message("MODULE: ${MODULE}, DIR_SRCS:${DIR_SRCS}")

    FILE(GLOB TARS_LIST "${CMAKE_CURRENT_SOURCE_DIR}/*.tars")

    # message("TARS_LIST:${TARS_LIST}")

    set(TARS_LIST_DEPENDS)

    if (TARS_LIST)
        set(CLEAN_LIST)

        foreach (TARS_SRC ${TARS_LIST})
            get_filename_component(NAME_WE ${TARS_SRC} NAME_WE)

            set(TARS_H ${NAME_WE}.h)

            set(CUR_TARS_GEN ${CMAKE_CURRENT_SOURCE_DIR}/${TARS_H})
            LIST(APPEND TARS_LIST_DEPENDS ${CUR_TARS_GEN})
            
            # message("TARS_H:${CMAKE_CURRENT_SOURCE_DIR}/${TARS_H}")

            add_custom_command(OUTPUT ${CUR_TARS_GEN}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    DEPENDS ${TARS2CPP} 
                    COMMAND ${TARS2CPP} ${TARS_SRC}
                    COMMENT "${TARS2CPP} ${TARS_SRC}")

            list(APPEND CLEAN_LIST ${CMAKE_CURRENT_SOURCE_DIR}/${TARS_H})

        endforeach ()

        set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_LIST}")

        set(TARS_TARGET "TARS_${MODULE}")  
        add_custom_target(${TARS_TARGET} ALL DEPENDS ${TARS_LIST_DEPENDS})

        add_executable(${MODULE} ${DIR_SRCS})

        add_dependencies(${MODULE} ${TARS_TARGET})

    else(TARS_LIST)
        add_executable(${MODULE} ${DIR_SRCS})

    endif (TARS_LIST)

    add_dependencies(${MODULE} tarsservant tarsutil)

    target_link_libraries(${MODULE} tarsservant tarsutil)
        
endmacro()
