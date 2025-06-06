cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
project(rip C)

option(RIP_BACKEND "Set backend" "")
option(RIP_ENABLE_TESTS "Enable tests compilation" OFF)

# Setup library.
set(RIP_SOURCES
    Source/C3D.c
    Source/Cache.c
    Source/Convert.c
    Source/Format.c
    Source/Swap.c
    Source/Tiling.c
)

add_library(rip STATIC ${RIP_SOURCES})

if (RIP_BACKEND STREQUAL "kygx")
    target_compile_definitions(rip PUBLIC RIP_BACKEND=RIP_BACKEND_KYGX)
elseif (RIP_BACKEND STREQUAL "libctru")
    target_compile_definitions(rip PUBLIC RIP_BACKEND=RIP_BACKEND_LIBCTRU)
elseif (RIP_BACKEND STREQUAL "citro3d")
    target_compile_definitions(rip PUBLIC RIP_BACKEND=RIP_BACKEND_CITRO3D)
endif()

target_include_directories(rip PUBLIC Include)
target_compile_options(rip PRIVATE -Wall -Werror)

install(TARGETS rip)
install(DIRECTORY Include/ DESTINATION include)

if (KYGX_ENABLE_TESTS)
    #add_subdirectory(Tests)
endif()