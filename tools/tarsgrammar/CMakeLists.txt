
macro(complice_lex OUT_DEPENDS_LEX_LIST)
    set(DEPENDS_LIST)

    FILE(GLOB SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/*.l")

    foreach (FILE ${SRC_LIST})

        get_filename_component(NAME_WE ${FILE} NAME_WE)

        set(LEX_IN ${FILE})
        set(LEX_CPP ${NAME_WE}.lex.cpp)

        add_custom_command(
                OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${LEX_CPP}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                DEPENDS ${LEX_IN}
                COMMAND flex --nounist ${LEX_IN}
                COMMENT "flex --nounist ${LEX_IN}")

        list(APPEND DEPENDS_LIST ${CMAKE_CURRENT_SOURCE_DIR}/${LEX_CPP})

        get_filename_component(PATH ${FILE} PATH)

    endforeach (FILE)

    set(OUT_DEPENDS_LEX_LIST ${DEPENDS_LIST})

endmacro()

macro(complice_yacc OUT_DEPENDS_YACC_LIST)
    set(DEPENDS_LIST)

    FILE(GLOB SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/*.y")

    foreach (FILE ${SRC_LIST})

        get_filename_component(NAME_WE ${FILE} NAME_WE)

        set(YACC_IN ${FILE})
        set(YACC_H ${NAME_WE}.tab.hpp)
        set(YACC_CPP ${NAME_WE}.tab.cpp)

        add_custom_command(
                OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${YACC_H}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                DEPENDS ${YACC_IN}
                COMMAND bison  ${YACC_IN} -o ${YACC_CPP}
                COMMENT "bison  ${YACC_IN}  -o ${YACC_CPP}")

        list(APPEND DEPENDS_LIST ${CMAKE_CURRENT_SOURCE_DIR}/${YACC_H})

        get_filename_component(PATH ${FILE} PATH)

    endforeach (FILE)

endmacro()

##########################################################################################
#生成.l和.y文件生成的代码文件copy到tarsparse目录下

set(OUT_DEPENDS_LEX_LIST)

complice_lex(OUT_DEPENDS_LEX_LIST)

FILE(GLOB SRC_LEX_LIST "*.l")

#set(CLEAN_LIST)

#将生成代码文件copy到对应的目录
foreach (FILE ${SRC_LEX_LIST})

    get_filename_component(NAME_WE ${FILE} NAME_WE)
    set(LEX_CPP ${NAME_WE}.lex.cpp)
    set(CP_SOURCE_CPP_DIR "${PROJECT_SOURCE_DIR}/tarsparse")

    add_custom_command(
            OUTPUT ${CP_SOURCE_CPP_DIR}/${LEX_CPP}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMAND cp -rf ${LEX_CPP} ${CP_SOURCE_CPP_DIR}
            DEPENDS ${LEX_CPP}
            COMMENT "cp -rf ${LEX_CPP} ${CP_SOURCE_CPP_DIR}")

    #设置依赖
    list(APPEND OUT_DEPENDS_LEX_LIST ${CP_SOURCE_CPP_DIR}/${LEX_CPP})

endforeach ()

set(OUT_DEPENDS_YACC_LIST)

complice_yacc(OUT_DEPENDS_YACC_LIST)

FILE(GLOB SRC_YACC_LIST "*.y")

#将生成代码文件copy到对应的目录
foreach (FILE ${SRC_YACC_LIST})

    get_filename_component(NAME_WE ${FILE} NAME_WE)
    set(YACC_H ${NAME_WE}.tab.hpp)
    set(YACC_CPP ${NAME_WE}.tab.cpp)
    set(CP_SOURCE_H_DIR "${PROJECT_SOURCE_DIR}/tarsparse")

    add_custom_command(
            OUTPUT ${CP_SOURCE_H_DIR}/${YACC_H}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMAND cp -rf ${YACC_H} ${YACC_CPP} ${CP_SOURCE_H_DIR}
            DEPENDS ${YACC_H}
            COMMENT "cp -rf ${YACC_H} ${YACC_CPP} ${CP_SOURCE_H_DIR}")

    #设置依赖
    list(APPEND OUT_DEPENDS_YACC_LIST ${CP_SOURCE_H_DIR}/${YACC_H})

endforeach ()

add_custom_target(COPY-LEX-YACC ALL DEPENDS ${OUT_DEPENDS_LEX_LIST} ${OUT_DEPENDS_YACC_LIST})

