project(util)

if(WIN32)
    enable_language(ASM_MASM)
else()
    enable_language(ASM)
endif()


add_subdirectory(src)
add_subdirectory(include)


