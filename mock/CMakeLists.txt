
include_directories(../)
file(GLOB_RECURSE SRC_FILES *.cpp)
add_library(tarsmock STATIC ${SRC_FILES})

add_dependencies(tarsmock tarsservant)

install(DIRECTORY . DESTINATION include/mock FILES_MATCHING PATTERN "*.h")

install(TARGETS tarsmock RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

if(ENABLE_SHARED)
   add_library(tarsmock_shared SHARED ${SRC_FILES})
   target_compile_definitions(tarsmock_shared PRIVATE SVT_USE_DLL UTIL_USE_DLL)
   target_link_libraries(tarsmock_shared tarsservant_shared tarsutil_shared)
   add_dependencies(tarsmock_shared tarsservant_shared)

   install(TARGETS tarsmock_shared RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

   if (WIN32)
      install(FILES $<TARGET_PDB_FILE:tarsmock_shared> DESTINATION bin)
   endif()

endif()
