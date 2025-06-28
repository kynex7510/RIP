cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
include(CMake/CPM.cmake)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
project(rip C ASM)

option(RIP_BACKEND "Set backend" "")
option(RIP_ENABLE_TESTS "Enable tests compilation" OFF)

set(RIP_SOURCES
    Source/C3D.c
    Source/Cache.c
    Source/Convert.c
    Source/Format.c
    Source/Swap.c
    Source/SwapImpl.s
    Source/Tex3DS.c
    Source/Tiling.c
)

add_library(rip STATIC ${RIP_SOURCES})

if (RIP_BACKEND STREQUAL "kygx") # KYGX
    CPMAddPackage("gh:kynex7510/KYGX#9531fa7")
    target_compile_definitions(rip PUBLIC RIP_BACKEND=RIP_BACKEND_KYGX)
    target_link_libraries(rip PUBLIC kygx)
elseif (RIP_BACKEND STREQUAL "libctru") # libctru
    target_compile_definitions(rip PUBLIC RIP_BACKEND=RIP_BACKEND_LIBCTRU)
elseif (RIP_BACKEND STREQUAL "citro3d") # citro3d
    target_compile_definitions(rip PUBLIC RIP_BACKEND=RIP_BACKEND_CITRO3D)
    target_link_libraries(rip PUBLIC citro3d)
else()
    message(FATAL_ERROR "Invalid backend, use one of the following: kygx, libctru, citro3d")
endif()

target_include_directories(rip PUBLIC Include)
target_compile_options(rip PRIVATE -Wall -Werror)

install(TARGETS rip)
install(DIRECTORY Include/ DESTINATION include)

if (RIP_ENABLE_TESTS)
    add_subdirectory(Tests)
endif()