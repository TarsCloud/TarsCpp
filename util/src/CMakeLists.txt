set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

aux_source_directory(. DIR_SRCS)

# if (CMAKE_SIZEOF_VOID_P EQUAL 8)
#     list(APPEND DIR_SRCS "tc_jump_x86_64_sysv_elf_gas.s")
#     list(APPEND DIR_SRCS "tc_make_x86_64_sysv_elf_gas.s")
# else ()
#     list(APPEND DIR_SRCS "tc_jump_i386_sysv_elf_gas.s")
#     list(APPEND DIR_SRCS "tc_make_i386_sysv_elf_gas.s")
# endif ()


message("CMAKE_SYSTEM_NAME:${CMAKE_SYSTEM_NAME}, CMAKE_SYSTEM_PROCESSOR:${CMAKE_SYSTEM_PROCESSOR}, CMAKE_CXX_COMPILER_ID:${CMAKE_CXX_COMPILER_ID}")

macro(cpu CPU_STR)
    set(CPU_STR "combined")

    if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i386")
        set(CPU_STR "i386")
    elseif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(CPU_STR "x86_64")
    elseif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(CPU_STR "x86_64")
    elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm")
        set(CPU_STR "arm")
    elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "mips")
        set(CPU_STR "mips")
    elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ppc")
        set(CPU_STR "ppc")
    endif()
endmacro ()


macro(abi ABI_STR)
    set(ABI_STR "sysv")

    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set(ABI_STR "ms")
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "CYGWIN")
        set(ABI_STR "ms")
    elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm")
        set(ABI_STR "aapcs")
    elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "mips")
        set(ABI_STR "o32")
    endif()

endmacro ()

macro(binaryformat BF_STR)
    set(BF_STR "elf")

    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set(BF_STR "pe")
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "CYGWIN")
        set(BF_STR "pe")
    elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AIX")
        set(BF_STR "xcoff")
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        set(BF_STR "macho")
    endif()
endmacro ()

set(ABI_STR)
abi(ABI_STR)

set(BF_STR)
binaryformat(BF_STR)

set(CPU_STR)
cpu(CPU_STR)

macro(get_asm JUMP_SRC MAKE_SRC)
    # ARM/AAPCS/ELF
    if(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "4" AND ${CPU_STR} STREQUAL "arm" AND ${ABI_STR} STREQUAL "aapcs" AND ${BF_STR} STREQUAL "elf")
        set(JUMP_SRC "asm/jump_arm_aapcs_elf_gas.S")
        set(MAKE_SRC "asm/make_arm_aapcs_elf_gas.S")
    # ARM/AAPCS/MACH-O
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "4" AND ${CPU_STR} STREQUAL "arm" AND ${ABI_STR} STREQUAL "aapcs" AND ${BF_STR} STREQUAL "macho")
        set(JUMP_SRC "asm/jump_arm_aapcs_macho_gas.S")
        set(MAKE_SRC "asm/make_arm_aapcs_macho_gas.S")
    # ARM64/AAPCS/ELF
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "8" AND ${CPU_STR} STREQUAL "arm" AND ${ABI_STR} STREQUAL "aapcs" AND ${BF_STR} STREQUAL "elf")
        set(JUMP_SRC "asm/jump_arm64_aapcs_elf_gas.S")
        set(MAKE_SRC "asm/make_arm64_aapcs_elf_gas.S")
    # ARM64/AAPCS/MACH-O
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "8" AND ${CPU_STR} STREQUAL "arm" AND ${ABI_STR} STREQUAL "aapcs" AND ${BF_STR} STREQUAL "macho")
        set(JUMP_SRC "asm/jump_arm64_aapcs_macho_gas.S")
        set(MAKE_SRC "asm/make_arm64_aapcs_macho_gas.S")
    # MIPS/O32/ELF
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "4" AND ${CPU_STR} STREQUAL "mips" AND ${ABI_STR} STREQUAL "o32" AND ${BF_STR} STREQUAL "elf")
        set(JUMP_SRC "asm/jump_mips32_o32_elf_gas.S")
        set(MAKE_SRC "asm/make_mips32_o32_elf_gas.S")
    # MIPS/O32/ELF
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "4" AND ${CPU_STR} STREQUAL "ppc" AND ${ABI_STR} STREQUAL "sysv" AND ${BF_STR} STREQUAL "elf")
        set(JUMP_SRC "asm/jump_ppc32_sysv_elf_gas.S")
        set(MAKE_SRC "asm/make_ppc32_sysv_elf_gas.S")
    # MIPS/O32/MACH-O
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "4" AND ${CPU_STR} STREQUAL "ppc" AND ${ABI_STR} STREQUAL "sysv" AND ${BF_STR} STREQUAL "macho")
        set(JUMP_SRC "asm/jump_ppc32_sysv_macho_gas.S")
        set(MAKE_SRC "asm/make_ppc32_sysv_macho_gas.S")
    # POWERPC_32/SYSV/XCOFF
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "4" AND ${CPU_STR} STREQUAL "ppc" AND ${ABI_STR} STREQUAL "sysv" AND ${BF_STR} STREQUAL "xcoff")
        set(JUMP_SRC "asm/jump_ppc32_sysv_xcoff_gas.S")
        set(MAKE_SRC "asm/make_ppc32_sysv_xcoff_gas.S")
    # POWERPC_64/SYSV/ELF
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "8" AND ${CPU_STR} STREQUAL "ppc" AND ${ABI_STR} STREQUAL "sysv" AND ${BF_STR} STREQUAL "elf")
        set(JUMP_SRC "asm/jump_ppc64_sysv_elf_gas.S")
        set(MAKE_SRC "asm/make_ppc64_sysv_elf_gas.S")
    # POWERPC_64/SYSV/MACH-O
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "8" AND ${CPU_STR} STREQUAL "ppc" AND ${ABI_STR} STREQUAL "sysv" AND ${BF_STR} STREQUAL "macho")
        set(JUMP_SRC "asm/jump_ppc64_sysv_macho_gas.S")
        set(MAKE_SRC "asm/make_ppc64_sysv_macho_gas.S")
    # POWERPC_64/SYSV/XCOFF(to do 32_64)
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "8" AND ${CPU_STR} STREQUAL "ppc" AND ${ABI_STR} STREQUAL "sysv" AND ${BF_STR} STREQUAL "macho")
        set(JUMP_SRC "asm/jump_ppc32_ppc64_sysv_macho_gas.S")
        set(MAKE_SRC "asm/make_ppc32_ppc64_sysv_macho_gas.S")
    # X86/SYSV/ELF
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "4" AND ${CPU_STR} STREQUAL "i386" AND ${ABI_STR} STREQUAL "sysv" AND ${BF_STR} STREQUAL "elf")
        set(JUMP_SRC "asm/jump_i386_sysv_elf_gas.S")
        set(MAKE_SRC "asm/make_i386_sysv_elf_gas.S")
    # X86/SYSV/MACH-O
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "4" AND ${CPU_STR} STREQUAL "i386" AND ${ABI_STR} STREQUAL "sysv" AND ${BF_STR} STREQUAL "macho")
        set(JUMP_SRC "asm/jump_i386_sysv_macho_gas.S")
        set(MAKE_SRC "asm/make_i386_sysv_macho_gas.S")
    # X86/MS/PE
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "4" AND ${CPU_STR} STREQUAL "i386" AND ${ABI_STR} STREQUAL "ms" AND ${BF_STR} STREQUAL "pe")
        if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
            set(JUMP_SRC "asm/jump_i386_ms_pe_gas.asm")
            set(MAKE_SRC "asm/make_i386_ms_pe_gas.asm")
        else()
            set(JUMP_SRC "asm/jump_i386_ms_pe_masm.asm")
            set(MAKE_SRC "asm/make_i386_ms_pe_masm.asm")
        endif()
    # X86_64/SYSV/ELF
    elseif((${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "8") AND (${CPU_STR} STREQUAL "x86_64") AND (${ABI_STR} STREQUAL "sysv") AND (${BF_STR} STREQUAL "elf"))
        set(JUMP_SRC "asm/jump_x86_64_sysv_elf_gas.S")
        set(MAKE_SRC "asm/make_x86_64_sysv_elf_gas.S")
    # X86_64/SYSV/MACH-O
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "4" AND ${CPU_STR} STREQUAL "x86_64" AND ${ABI_STR} STREQUAL "sysv" AND ${BF_STR} STREQUAL "macho")
        set(JUMP_SRC "asm/jump_x86_64_sysv_macho_gas.S")
        set(MAKE_SRC "asm/make_x86_64_sysv_macho_gas.S")
    # X86_64/MS/PE
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "8" AND ${CPU_STR} STREQUAL "x86_64" AND ${ABI_STR} STREQUAL "ms" AND ${BF_STR} STREQUAL "pe")
        if((${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang"))
            set(JUMP_SRC "asm/jump_x86_64_ms_pe_gas.asm")
            set(MAKE_SRC "asm/make_x86_64_ms_pe_gas.asm")
        else()
            set(JUMP_SRC "asm/jump_x86_64_ms_pe_masm.asm")
            set(MAKE_SRC "asm/make_x86_64_ms_pe_masm.asm")
        endif()
    # X86_64/SYSV/X32
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "8" AND ${CPU_STR} STREQUAL "x86_64" AND ${ABI_STR} STREQUAL "sysv" AND ${BF_STR} STREQUAL "macho")
        set(JUMP_SRC "asm/jump_x86_64_sysv_macho_gas.S")
        set(MAKE_SRC "asm/make_x86_64_sysv_macho_gas.S")
    # X86 universal
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "8" AND ${CPU_STR} STREQUAL "universal" AND ${ABI_STR} STREQUAL "sysv" AND ${BF_STR} STREQUAL "macho")
        set(JUMP_SRC "asm/jump_i386_x86_64_sysv_macho_gas.S")
        set(MAKE_SRC "asm/make_i386_x86_64_sysv_macho_gas.S")
    # COMBINED
    elseif(${CMAKE_C_SIZEOF_DATA_PTR} STREQUAL "8" AND ${CPU_STR} STREQUAL "combined" AND ${ABI_STR} STREQUAL "sysv" AND ${BF_STR} STREQUAL "macho")
        set(JUMP_SRC "asm/jump_combined_sysv_macho_gas.S")
        set(MAKE_SRC "asm/make_combined_sysv_macho_gas.S")
    endif()


endmacro ()

get_asm(JUMP_SRC MAKE_SRC)

message("ABI_STR:${ABI_STR}, BF_STR:${BF_STR}, CPU_STR:${CPU_STR}, JUMP_SRC:${JUMP_SRC}, MAKE_SRC:${MAKE_SRC}, CMAKE_C_SIZEOF_DATA_PTR:${CMAKE_C_SIZEOF_DATA_PTR}")

list(APPEND DIR_SRCS ${JUMP_SRC})
list(APPEND DIR_SRCS ${MAKE_SRC})

add_library(tarsutil ${DIR_SRCS})

if(${TARS_SSL} EQUAL 1)
    target_link_libraries(tarsutil; crypto; ssl)
else()
    target_link_libraries(tarsutil)
endif()
if(${TARS_HTTP2} EQUAL 1)
    target_link_libraries(tarsutil; nghttp2)
else()
    target_link_libraries(tarsutil)
endif()

install(TARGETS tarsutil
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
