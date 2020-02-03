
add_subdirectory(UtilDemo)
add_subdirectory(CoroutineDemo)
add_subdirectory(HttpDemo)
add_subdirectory(PushDemo)
add_subdirectory(QuickStartDemo)
add_subdirectory(StressDemo)

add_custom_target(run-quick-start
        DEPENDS QuickStartDemo QuickStartDemoClient
        COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run-quick-start.sh
        COMMENT "call quick start")

add_custom_target(run-http
        DEPENDS HttpServer HttpClient
        COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run-http.sh
        COMMENT "call run http")

add_custom_target(run-co
        DEPENDS CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro
        COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run-co.sh
        COMMENT "call run co")

add_custom_target(run-push
        DEPENDS PushServer PushClient
        COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run-push.sh
        COMMENT "call run push")

add_custom_target(run-all
        DEPENDS QuickStartDemo QuickStartDemoClient HttpServer HttpClient CoroutineDemoAServer CoroutineDemoBServer CoroutineDemoClient testCoro testParallelCoro PushServer PushClient
        COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run-quick-start.sh
        COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run-http.sh
        COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run-co.sh
        COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run-push.sh
        COMMENT "call run all")
