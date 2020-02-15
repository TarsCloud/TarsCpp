
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
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS QuickStartDemo QuickStartDemoClient
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-quick-start.bat
        COMMENT "call quick start: ${CMAKE_BINARY_DIR}")

    add_custom_target(run-http
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS HttpServer HttpClient
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-http.bat
        COMMENT "call run http")

    add_custom_target(run-auth
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS AuthServer AuthClient
            COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-auth.bat
            COMMENT "call run auth")

    if(TARS_HTTP2)
    add_custom_target(run-http2
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS Http2Server Http2Client
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-http2.bat
        COMMENT "call run http2")
    endif()

    if(TARS_SSL)
    add_custom_target(run-ssl
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS SSLServer SSLClient
            COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-ssl.bat
            COMMENT "call run ssl")
    endif()

    add_custom_target(run-co
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-co.bat
        COMMENT "call run co")

    add_custom_target(run-custom
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS CustomServer CustomClient
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-custom.bat
        COMMENT "call run custom")

    add_custom_target(run-push
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS PushServer PushClient
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-push.bat
        COMMENT "call run push")

    if(TARS_SSL)
    add_custom_target(run-all
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS QuickStartDemo QuickStartDemoClient HttpServer HttpClient AuthServer AuthClient  SSLServer SSLClient CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro PushServer PushClient
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-quick-start.bat
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-http.bat
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-co.bat
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-push.bat
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-auth.bat
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-ssl.bat
        COMMENT "call run all ${CMAKE_BINARY_DIR}")
    else()
    add_custom_target(run-all
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS QuickStartDemo QuickStartDemoClient HttpServer HttpClient AuthServer AuthClient CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro PushServer PushClient
            COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-quick-start.bat
            COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-http.bat
            COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-co.bat
            COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-push.bat
            COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-auth.bat
            COMMENT "call run all ${CMAKE_BINARY_DIR}")
    endif()

    add_custom_target(run-kill
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-kill.bat
            COMMENT "call run kill")

else(WIN32)

    add_custom_target(run-quick-start
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS QuickStartDemo QuickStartDemoClient
        COMMAND sh ../examples/scripts/run-quick-start.sh
        COMMENT "call quick start")

    add_custom_target(run-http
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS HttpServer HttpClient
        COMMAND sh ../examples/scripts/run-http.sh
        COMMENT "call run http")

    add_custom_target(run-auth
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS AuthServer AuthClient
            COMMAND sh ../examples/scripts/run-auth.sh
            COMMENT "call run auth")

    if(TARS_HTTP2)
    add_custom_target(run-http2
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS Http2Server Http2Client
        COMMAND sh ../examples/scripts/run-http2.sh
        COMMENT "call run http2")
    endif()

    if(TARS_SSL)
    add_custom_target(run-ssl
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS SSLServer SSLClient
        COMMAND sh ../examples/scripts/run-ssl.sh
        COMMENT "call run ssl")
    endif()

    add_custom_target(run-co
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro
        COMMAND sh ../examples/scripts/run-co.sh
        COMMENT "call run co")

    add_custom_target(run-custom
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS CustomServer CustomClient
        COMMAND sh ../examples/scripts/run-custom.sh
        COMMENT "call run custom")

    add_custom_target(run-push
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS PushServer PushClient
        COMMAND sh ../examples/scripts/run-push.sh
        COMMENT "call run push")

    if(TARS_SSL)
    add_custom_target(run-all
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS QuickStartDemo QuickStartDemoClient HttpServer HttpClient AuthServer AuthClient  SSLServer SSLClient CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro PushServer PushClient
        COMMAND sh ../examples/scripts/run-quick-start.sh
        COMMAND sh ../examples/scripts/run-http.sh
        COMMAND sh ../examples/scripts/run-co.sh
        COMMAND sh ../examples/scripts/run-push.sh
        COMMAND sh ../examples/scripts/run-auth.sh
        COMMAND sh ../examples/scripts/run-ssl.sh
        COMMENT "call run all")
    else()
    add_custom_target(run-all
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS QuickStartDemo QuickStartDemoClient HttpServer HttpClient AuthServer AuthClient CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro PushServer PushClient
            COMMAND sh ../examples/scripts/run-quick-start.sh
            COMMAND sh ../examples/scripts/run-http.sh
            COMMAND sh ../examples/scripts/run-co.sh
            COMMAND sh ../examples/scripts/run-push.sh
            COMMAND sh ../examples/scripts/run-auth.sh
            COMMENT "call run all")
    endif()

    add_custom_target(run-kill
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND sh ../examples/scripts/run-kill.sh
            COMMENT "call run kill")
endif(WIN32)