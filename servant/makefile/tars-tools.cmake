
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(WIN32)
	set(TARS2CPP "c:\\tars\\cpp\\tools\\tars2cpp")
	set(TARS_PATH "c:\\tars\\cpp")
else()
	set(TARS2CPP "/usr/local/tars/cpp/tools/tars2cpp")
	set(TARS_PATH "/usr/local/tars/cpp")
endif()

if(APPLE)
	link_libraries(iconv)
endif()

IF (UNIX)
	link_libraries(pthread dl)
ENDIF ()

set(TARS_INC "${TARS_PATH}/include")
set(TARS_LIB_DIR "${TARS_PATH}/lib" )

if(WIN32)
	set (LIB_TARS_SERVANT "${TARS_LIB_DIR}/tarsservant.lib")
	set (LIB_TARS_UTIL "${TARS_LIB_DIR}/tarsutil.lib")
else()
	set (LIB_TARS_SERVANT "${TARS_LIB_DIR}/libtarsservant.a")
	set (LIB_TARS_UTIL "${TARS_LIB_DIR}/libtarsutil.a")
endif()

include_directories(${TARS_INC})
link_directories(${TARS_LIB_DIR})
include_directories(${TARS_PATH}/thirdparty/include/mysql)

if(WIN32)
	set(LIB_SSL "libssl")
	set(LIB_CRYPTO "libcrypto")
	set(LIB_MYSQL "libmysql")
	set(LIB_HTTP2 "nghttp2_static")
else()
	set(LIB_SSL "ssl")
	set(LIB_CRYPTO "crypto")
	set(LIB_MYSQL "mysqlclient")
	set(LIB_HTTP2 "nghttp2_static")
endif ()

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "set build type to release default")
IF (CMAKE_BUILD_TYPE STREQUAL "")
	set(CMAKE_BUILD_TYPE "Release")
ENDIF ()

set(TARS_WEB_HOST "" CACHE STRING "set web host")
IF (TARS_WEB_HOST STREQUAL "")
	set(TARS_WEB_HOST "http://web.tars.com")
ENDIF ()

set(PLATFORM)
IF (UNIX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11  -Wno-deprecated -fno-strict-aliasing -Wno-overloaded-virtual")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-builtin-macro-redefined -D__FILE__='\"$(notdir $(abspath $<))\"'")

	set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -g")
	set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall -fno-strict-aliasing")

	set(PLATFORM "linux")
	IF (APPLE)
		set(PLATFORM "mac")
		SET(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
		SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
		SET(CMAKE_C_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
		SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
	ENDIF (APPLE)

ELSEIF (WIN32)
	set(PLATFORM "window")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4101 /wd4244 /wd4996 /wd4091 /wd4503 /wd4819 /wd4200 /wd4800 /wd4267")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj ")

ELSE ()
	MESSAGE(STATUS "================ ERROR: This platform is unsupported!!! ================")
ENDIF (UNIX)

set(TARS_RELEASE "${PROJECT_BINARY_DIR}/run-release.cmake")
set(TARS_UPLOAD "${PROJECT_BINARY_DIR}/run-upload.cmake")
set(TARS_TAR "${PROJECT_BINARY_DIR}/run-tar.cmake")

FILE(WRITE ${TARS_RELEASE} "EXECUTE_PROCESS(COMMAND echo release all)\n")
FILE(WRITE ${TARS_UPLOAD} "EXECUTE_PROCESS(COMMAND echo upload all)\n")

function(gen_tars TARGET)

	file(GLOB_RECURSE TARS_INPUT *.tars)

	set(OUT_TARS_H_LIST)

	if (TARS_INPUT)

		foreach(TARS_FILE ${TARS_INPUT})
			get_filename_component(TARS_NAME ${TARS_FILE} NAME_WE)
			get_filename_component(TARS_PATH ${TARS_FILE} PATH)

			set(CUR_TARS_GEN ${TARS_PATH}/${TARS_NAME}.h)

			add_custom_command(
					OUTPUT ${CUR_TARS_GEN}
					WORKING_DIRECTORY ${TARS_PATH}
					COMMAND ${TARS2CPP} ${TARS_TOOL_FLAG} ${TARS_FILE}
					COMMENT "${TARS2CPP} ${TARS_TOOL_FLAG} ${TARS_FILE}"
					DEPENDS ${TARS2CPP} ${TARS_FILE}
			)

			list(APPEND OUT_TARS_H_LIST ${CUR_TARS_GEN})

		endforeach()

		add_custom_target(${TARGET} ALL DEPENDS ${OUT_TARS_H_LIST})

		set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${OUT_TARS_H_LIST}")

	endif()

endfunction()

#生成带tars文件的可执行程序
macro(gen_server APP TARGET)

	include_directories(${PROJECT_SOURCE_DIR})

	FILE(GLOB_RECURSE SRC_FILES  "*.cc" "*.cpp" ".c")

	add_executable(${TARGET} ${SRC_FILES})
	file(GLOB_RECURSE TARS_INPUT *.tars)

	if(TARS_INPUT)
		gen_tars(tars-${TARGET})
		add_dependencies(${TARGET} tars-${TARGET})
	endif()

	if(TARS_SSL)
		target_link_libraries(${TARGET} ${LIB_SSL} ${LIB_CRYPTO})

		if(WIN32)
			target_link_libraries(${TARGET} Crypt32)
		endif()
	endif()

	if(TARS_HTTP2)
		target_link_libraries(${TARGET} ${LIB_HTTP2})
	endif()

	target_link_libraries(${TARGET}  ${LIB_TARS_SERVANT} ${LIB_TARS_UTIL})

	#make tar #########################################################################
	SET(RUN_TAR_COMMAND_FILE "${CMAKE_BINARY_DIR}/run-tar-${TARGET}.cmake")
	FILE(WRITE ${RUN_TAR_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/${TARGET})\n")
	IF(WIN32)
		FILE(APPEND ${RUN_TAR_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/${TARGET}.exe ${CMAKE_BINARY_DIR}/${TARGET}/)\n")
	ELSE()
		FILE(APPEND ${RUN_TAR_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CMAKE_BINARY_DIR}/bin/${TARGET} ${CMAKE_BINARY_DIR}/${TARGET}/)\n")
	ENDIF()
	FILE(APPEND ${RUN_TAR_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND cmake -E tar czfv ${CMAKE_BINARY_DIR}/${TARGET}.tgz ${CMAKE_BINARY_DIR}/${TARGET})\n")

	#执行命令
	add_custom_command(OUTPUT ${TARGET}.tgz
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			COMMAND ${CMAKE_COMMAND} -P ${RUN_TAR_COMMAND_FILE}
			COMMENT "call ${RUN_TAR_COMMAND_FILE}")

	add_custom_target(${TARGET}-tar DEPENDS ${TARGET}.tgz ${TARGET})

	FILE(APPEND ${TARS_TAR} "EXECUTE_PROCESS(COMMAND cmake -P ${RUN_TAR_COMMAND_FILE})\n")

	#make upload #########################################################################
	SET(RUN_UPLOAD_COMMAND_FILE "${PROJECT_BINARY_DIR}/run-upload-${TARGET}.cmake")
	IF(WIN32)
		FILE(WRITE ${RUN_UPLOAD_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${TARGET}.tgz ${APP}.${TARGET}.tgz)\n")
		FILE(APPEND ${RUN_UPLOAD_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND ${TARS_PATH}\\cpp\\\\script\\\\WinSCP.exe  sftp://${TARS_WEB_SERVER}/usr/local/app/patchs/tars.upload/ /upload ${APP}.${TARGET}.tgz)\n")
		FILE(APPEND ${RUN_UPLOAD_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND ${TARS_PATH}\\cpp\\\\script\\\\busybox.exe wget -O ${APP}.${TARGET}.wget.out http://${TARS_WEB_HOST}/doUpload.jsp?SERVER=${APP}.${TARGET}&TGZ=/usr/local/app/patchs/tars.upload/${APP}.${TARGET}.tgz)\n")
	ELSE()
		FILE(WRITE ${RUN_UPLOAD_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND echo ${TARS_WEB_HOST}/pages/server/api/upload_and_publish -Fsuse=@${TARGET}.tgz -Fapplication=${APP} -Fmodule_name=${TARGET} -Fcomment=dev)\n")
		FILE(APPEND ${RUN_UPLOAD_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND curl ${TARS_WEB_HOST}/pages/server/api/upload_and_publish -Fsuse=@${TARGET}.tgz -Fapplication=${APP} -Fmodule_name=${TARGET} -Fcomment=dev)\n")
		FILE(APPEND ${RUN_UPLOAD_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND echo \n---------------------------------------------------------------------------)\n")

	ENDIF()

	#执行命令
	add_custom_target(${TARGET}-upload
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			DEPENDS ${TARGET}-tar
			COMMAND cmake -P ${RUN_UPLOAD_COMMAND_FILE}
			COMMENT "upload ${APP}.${TARGET}.tgz and publish...")

	FILE(APPEND ${TARS_UPLOAD} "EXECUTE_PROCESS(COMMAND cmake -P ${RUN_UPLOAD_COMMAND_FILE})\n")

	#make release #########################################################################
	SET(RUN_RELEASE_COMMAND_FILE "${PROJECT_BINARY_DIR}/run-release-${TARGET}.cmake")

	if (TARS_INPUT)
		foreach(TARS_FILE ${TARS_INPUT})
			get_filename_component(TARS_NAME ${TARS_FILE} NAME_WE)
			get_filename_component(TARS_PATH ${TARS_FILE} PATH)

			set(CUR_TARS_GEN ${TARS_PATH}/${TARS_NAME}.h)

			if(WIN32)
				FILE(WRITE ${RUN_RELEASE_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND make_directory c:\\tarsproto\\protocol\\${APP}\\${TARGET})\n")
				FILE(APPEND ${RUN_RELEASE_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND echo cp -rf ${CUR_TARS_GEN} c:\\tarsproto\\protocol\\${APP}\\${TARGET})\n")
				FILE(APPEND ${RUN_RELEASE_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CUR_TARS_GEN} c:\\tarsproto\\protocol\\${APP}\\${TARGET})\n")
			elseif(APPLE)
				FILE(WRITE ${RUN_RELEASE_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND make_directory $ENV{HOME}/tarsproto/protocol/${APP}/${TARGET})\n")
				FILE(APPEND ${RUN_RELEASE_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND echo cp -rf ${CUR_TARS_GEN} $ENV{HOME}/tarsproto/protocol/${APP}/${TARGET})\n")
				FILE(APPEND ${RUN_RELEASE_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CUR_TARS_GEN} $ENV{HOME}/tarsproto/protocol/${APP}/${TARGET})\n")
			elseif(LINUX)
				FILE(WRITE ${RUN_RELEASE_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND make_directory /home/tarsproto/${APP}/${TARGET})\n")
				FILE(APPEND ${RUN_RELEASE_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND echo cp -rf ${CUR_TARS_GEN} /home/tarsproto/${APP}/${TARGET})\n")
				FILE(APPEND ${RUN_RELEASE_COMMAND_FILE} "EXECUTE_PROCESS(COMMAND cmake -E copy ${CUR_TARS_GEN} /home/tarsproto/${APP}/${TARGET})\n")
			endif()
		endforeach(TARS_FILE ${TARS_INPUT})
	endif ()

	#执行命令
	add_custom_target(${TARGET}-release
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			DEPENDS ${TARGET}
			COMMAND cmake -P ${RUN_RELEASE_COMMAND_FILE}
			COMMENT "call ${RUN_RELEASE_COMMAND_FILE}")

	FILE(APPEND ${TARS_RELEASE} "EXECUTE_PROCESS(COMMAND cmake -P ${RUN_RELEASE_COMMAND_FILE})\n")
endmacro()

add_custom_target(upload
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMAND cmake -P ${TARS_UPLOAD})

add_custom_target(release
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMAND cmake -P ${TARS_RELEASE})

add_custom_target(tar
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMAND cmake -P ${TARS_TAR})

message("-------------------------------------------------------------------------------------")
message("CMAKE_SOURCE_DIR:          ${CMAKE_SOURCE_DIR}")
message("CMAKE_BINARY_DIR:          ${CMAKE_BINARY_DIR}")
message("PROJECT_SOURCE_DIR:        ${PROJECT_SOURCE_DIR}")
message("CMAKE_BUILD_TYPE:          ${CMAKE_BUILD_TYPE}")
message("PLATFORM:                  ${PLATFORM}")
message("TARS2CPP:                  ${TARS2CPP}")
message("TARS_MYSQL:                ${TARS_MYSQL}")
message("TARS_HTTP2:                ${TARS_HTTP2}")
message("TARS_SSL:                  ${TARS_SSL}")
message("TARS_WEB_HOST:             ${TARS_WEB_HOST}")
message("-------------------------------------------------------------------------------------")

