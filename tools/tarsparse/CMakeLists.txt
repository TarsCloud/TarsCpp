
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(TARGETNAME "tarsparse")

aux_source_directory(. DIR_SRCS)

set(DEPENDS_SRC_LIST)

set(DEPENDS_LEC_YACC_SRC_LIST)
list(APPEND DEPENDS_LEC_YACC_SRC_LIST "tars.lex.cpp")
list(APPEND DEPENDS_LEC_YACC_SRC_LIST "tars.tab.hpp")
list(APPEND DEPENDS_LEC_YACC_SRC_LIST "tars.tab.cpp")

set(DEPENDS_LEC_YACC_SRC_DIR "${PROJECT_SOURCE_DIR}/tarsGrammar/")

set(CURRENT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

foreach(LEC_YACC_SRC ${DEPENDS_LEC_YACC_SRC_LIST})

    list(APPEND DEPENDS_SRC_LIST ${CURRENT_SRC_DIR}/${LEC_YACC_SRC})

    add_custom_command(
            OUTPUT ${CURRENT_SRC_DIR}/${LEC_YACC_SRC}
            WORKING_DIRECTORY ${CURRENT_SRC_DIR})
#            COMMAND cp ${DEPENDS_LEC_YACC_SRC_DIR}/${LEC_YACC_SRC} ${CURRENT_SRC_DIR}
#            COMMENT "cp ${DEPENDS_LEC_YACC_SRC_DIR}/${LEC_YACC_SRC} ${CURRENT_SRC_DIR}")

endforeach()

add_library(${TARGETNAME} ${DIR_SRCS} ${DEPENDS_SRC_LIST})

if(UNIX)
    add_dependencies(${TARGETNAME} COPY-LEX-YACC)
endif(UNIX)

install(TARGETS tarsparse
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

FILE(GLOB HEADERS "*.h")
install(FILES  ${HEADERS} DESTINATION include/tarsparse)