
add_subdirectory(UtilDemo)
add_subdirectory(CoroutineDemo)
add_subdirectory(HttpDemo)
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

    add_custom_target(run-co
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-co.bat
        COMMENT "call run co")

    add_custom_target(run-push
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS PushServer PushClient
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-push.bat
        COMMENT "call run push")

    add_custom_target(run-all
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS QuickStartDemo QuickStartDemoClient HttpServer HttpClient CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro PushServer PushClient
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-quick-start.bat
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-http.bat
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-co.bat
        COMMAND ../servant/script/busybox.exe bash ../examples/scripts/run-push.bat
        COMMENT "call run all ${CMAKE_BINARY_DIR}")

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

    add_custom_target(run-co
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro
        COMMAND sh ../examples/scripts/run-co.sh
        COMMENT "call run co")

    add_custom_target(run-push
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS PushServer PushClient
        COMMAND sh ../examples/scripts/run-push.sh
        COMMENT "call run push")

    add_custom_target(run-all
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS run-quick-start run-http run-co run-push
        COMMAND sh ../examples/scripts/run-quick-start.sh
        COMMAND sh ../examples/scripts/run-http.sh
        COMMAND sh ../examples/scripts/run-co.sh
        COMMAND sh ../examples/scripts/run-push.sh
        COMMENT "call run all")
endif(WIN32)