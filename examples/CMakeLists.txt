
add_subdirectory(CoroutineDemo)
add_subdirectory(HttpDemo)
add_subdirectory(PushDemo)
add_subdirectory(QuickStartDemo)
add_subdirectory(StressDemo)

add_custom_target(run-quick-start
        DEPENDS QuickStartDemo QuickStartDemoClient
        COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run-quick-start.sh
        COMMENT "call")
