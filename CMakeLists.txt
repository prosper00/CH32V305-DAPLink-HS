set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.26)

set(CROSS_COMPILE "riscv-none-elf-")
# specify cross-compilers and tools
set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)
set(CMAKE_ASM_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_AR ${CROSS_COMPILE}ar)
set(CMAKE_OBJCOPY ${CROSS_COMPILE}objcopy)
set(CMAKE_OBJDUMP ${CROSS_COMPILE}objdump)
set(SIZE ${CROSS_COMPILE}size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(CH32V305_DAP C ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/Ld/Link.ld)
add_compile_options(-march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore)

include_directories(
        "Core"
        "Debug"
        "Peripheral/inc"
        "User"
        "CherryUSB/class/cdc"
        "CherryUSB/class/msc"
        "CherryUSB/common"
        "CherryUSB/core"
        "CherryUSB/port/ch32"
        "CherryRB"
        "CherryDAP"
        "CherryDAP/DAP/Include"
        "DAPHost"
)

file(GLOB_RECURSE SOURCES
        "Core/core_riscv.c"
        "Debug/debug.c"
        "Peripheral/src/*.c"
        "Startup/startup_ch32v30x_D8C.S"
        "User/*.c"
        "CherryUSB/class/cdc/usbd_cdc.c"
        "CherryUSB/core/usbd_core.c"
        "CherryUSB/port/ch32/usb_dc_usbhs.c"
        "CherryRB/chry_ringbuffer.c"
        "DAPHost/swd_host.c"
        "CherryDAP/DAP/Source/*.c"
        "CherryDAP/dap_main.c"
)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    # flto
#    add_compile_options(-flto)
    add_compile_options(-O3 -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-O0 -g -gdwarf-2)
endif ()

add_compile_options(
        -fmessage-length=0
        -fsigned-char
        -ffunction-sections
        -fdata-sections
        -fno-common
        -Wunused
        -Wuninitialized
)

add_link_options(
        -march=rv32imacxw -mabi=ilp32
        -T ${LINKER_SCRIPT}
        -nostartfiles
        -Xlinker
        --gc-sections
        -lm
        -Wl,--print-memory-usage
        -Wl,-Map,${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map
        --specs=nano.specs --specs=nosys.specs
)
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
set(LST_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.lst)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMAND ${CMAKE_OBJDUMP} --all-headers --demangle --disassemble $<TARGET_FILE:${PROJECT_NAME}.elf> > ${LST_FILE}
        COMMAND ${SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}.elf>
)