
add_subdirectory(UtilDemo)
add_subdirectory(CoroutineDemo)
add_subdirectory(HttpDemo)
add_subdirectory(CustomDemo)
add_subdirectory(AuthDemo)

if(TARS_SSL)
add_subdirectory(SSLDemo)
endif()

add_subdirectory(PushDemo)
add_subdirectory(QuickStartDemo)
add_subdirectory(StressDemo)

if(WIN32)

    add_custom_target(run-quick-start
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            DEPENDS QuickStartDemo QuickStartDemoClient
            USES_TERMINAL
            COMMAND examples/scripts/run-quick-start.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMENT "call quick start")

    add_custom_target(run-http
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            DEPENDS HttpServer HttpClient
            USES_TERMINAL
            COMMAND examples/scripts/run-http.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMENT "call run http")

    add_custom_target(run-auth
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            DEPENDS AuthServer AuthClient
            USES_TERMINAL
            COMMAND examples/scripts/run-auth.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMENT "call run auth")

    if(TARS_HTTP2)
        add_custom_target(run-http2
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                DEPENDS Http2Server Http2Client
                USES_TERMINAL
                COMMAND examples/scripts/run-http2.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
                COMMENT "call run http2")
    endif()

    if(TARS_SSL)
        add_custom_target(run-ssl
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                DEPENDS SSLServer SSLClient
                USES_TERMINAL
                COMMAND examples/scripts/run-ssl.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
                COMMENT "call run ssl")
    endif()

    add_custom_target(run-co
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            DEPENDS CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro
            USES_TERMINAL
            COMMAND examples/scripts/run-co.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMENT "call run co")

    add_custom_target(run-custom
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            DEPENDS CustomServer CustomClient
            USES_TERMINAL
            COMMAND examples/scripts/run-custom.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMENT "call run custom")

    add_custom_target(run-push
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            DEPENDS PushServer PushClient
            USES_TERMINAL
            COMMAND examples/scripts/run-push.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMENT "call run push")

    if(TARS_SSL)
        add_custom_target(run-all
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                DEPENDS QuickStartDemo QuickStartDemoClient HttpServer HttpClient AuthServer AuthClient  SSLServer SSLClient CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro PushServer PushClient
                USES_TERMINAL
                COMMAND examples/scripts/run-quick-start.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
                COMMAND examples/scripts/run-http.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
                COMMAND examples/scripts/run-co.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
                COMMAND examples/scripts/run-push.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
                COMMAND examples/scripts/run-auth.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
                COMMAND examples/scripts/run-ssl.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
                COMMENT "call run all")
    else()
        add_custom_target(run-all
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                USES_TERMINAL
                DEPENDS QuickStartDemo QuickStartDemoClient HttpServer HttpClient AuthServer AuthClient CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro PushServer PushClient
                COMMAND examples/scripts/run-quick-start.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
                COMMAND examples/scripts/run-http.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
                COMMAND examples/scripts/run-co.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
                COMMAND examples/scripts/run-push.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
                COMMAND examples/scripts/run-auth.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
                COMMENT "call run all")
    endif()


    add_custom_target(run-kill
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            USES_TERMINAL
            COMMAND examples/scripts/run-kill.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMENT "call run kill")
else(WIN32)

    add_custom_target(run-quick-start
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS QuickStartDemo QuickStartDemoClient
        COMMAND sh examples/scripts/run-quick-start.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
        COMMENT "call quick start")

    add_custom_target(run-http
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS HttpServer HttpClient
        COMMAND sh examples/scripts/run-http.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
        COMMENT "call run http")

    add_custom_target(run-auth
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            DEPENDS AuthServer AuthClient
            COMMAND sh examples/scripts/run-auth.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMENT "call run auth")

    if(TARS_HTTP2)
    add_custom_target(run-http2
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS Http2Server Http2Client
        COMMAND sh examples/scripts/run-http2.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
        COMMENT "call run http2")
    endif()

    if(TARS_SSL)
    add_custom_target(run-ssl
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS SSLServer SSLClient
        COMMAND sh examples/scripts/run-ssl.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
        COMMENT "call run ssl")
    endif()

    add_custom_target(run-co
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro
        COMMAND sh examples/scripts/run-co.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
        COMMENT "call run co")

    add_custom_target(run-custom
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS CustomServer CustomClient
        COMMAND sh examples/scripts/run-custom.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
        COMMENT "call run custom")

    add_custom_target(run-push
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS PushServer PushClient
        COMMAND sh examples/scripts/run-push.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
        COMMENT "call run push")

    if(TARS_SSL)
    add_custom_target(run-all
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS QuickStartDemo QuickStartDemoClient HttpServer HttpClient AuthServer AuthClient  SSLServer SSLClient CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro PushServer PushClient
        COMMAND sh examples/scripts/run-quick-start.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
        COMMAND sh examples/scripts/run-http.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
        COMMAND sh examples/scripts/run-co.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
        COMMAND sh examples/scripts/run-push.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
        COMMAND sh examples/scripts/run-auth.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
        COMMAND sh examples/scripts/run-ssl.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
        COMMENT "call run all")
    else()
    add_custom_target(run-all
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            DEPENDS QuickStartDemo QuickStartDemoClient HttpServer HttpClient AuthServer AuthClient CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro PushServer PushClient
            COMMAND sh examples/scripts/run-quick-start.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMAND sh examples/scripts/run-http.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMAND sh examples/scripts/run-co.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMAND sh examples/scripts/run-push.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMAND sh examples/scripts/run-auth.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMENT "call run all")
    endif()

    add_custom_target(run-kill
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND sh examples/scripts/run-kill.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}
            COMMENT "call run kill")
endif(WIN32)