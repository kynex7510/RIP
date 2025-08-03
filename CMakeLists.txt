cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
include(CMake/CPM.cmake)

# C23, CXX, ASM required for libn3ds.
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
project(rip C CXX ASM)

# Setup baremetal.
if (CTR_BAREMETAL)
    include(BM/ARM11)
endif()

# Setup library.
option(RIP_BACKEND "Set the backend, must be one of: kygx, libctru, citro3d, libn3ds" "")
option(RIP_ENABLE_TESTS "Enable tests compilation" OFF)

set(RIP_SOURCES
    Source/C3D.c
    Source/Cache.c
    Source/Convert.c
    Source/Format.c
    Source/Pixels.c
    Source/PixelsImpl.s
    Source/Tex3DS.c
    Source/Texture.c
    Source/Tiling.c
)

if (CTR_BAREMETAL)
    set(RIP_SOURCES ${RIP_SOURCES} Source/Decompress/Decompress.c)
endif()

add_library(rip STATIC ${RIP_SOURCES})

if (RIP_BACKEND STREQUAL "kygx") # KYGX
    CPMAddPackage("gh:kynex7510/KYGX#e704045")
    target_compile_definitions(rip PUBLIC RIP_BACKEND=RIP_BACKEND_KYGX)
    target_link_libraries(rip PUBLIC kygx)
else()
    if (CTR_BAREMETAL)
        if (RIP_BACKEND STREQUAL "libn3ds") # libn3ds
            include(BM/N3DS11)
            target_compile_definitions(rip PUBLIC RIP_BACKEND=RIP_BACKEND_LIBN3DS)
            target_link_libraries(rip PUBLIC n3ds11)
        else()
            message(FATAL_ERROR "Invalid backend, use one of the following: kygx, libn3ds")
        endif()
    else()
        if (RIP_BACKEND STREQUAL "libctru") # libctru
            target_compile_definitions(rip PUBLIC RIP_BACKEND=RIP_BACKEND_LIBCTRU)
        elseif (RIP_BACKEND STREQUAL "citro3d") # citro3d
            target_compile_definitions(rip PUBLIC RIP_BACKEND=RIP_BACKEND_CITRO3D)
            target_link_libraries(rip PUBLIC citro3d)
        else()
            message(FATAL_ERROR "Invalid backend, use one of the following: kygx, libctru, citro3d")
        endif()
    endif()
endif()

message(STATUS "RIP: Compiling with backend \"${RIP_BACKEND}\"")

target_include_directories(rip PUBLIC Include)
target_compile_options(rip PRIVATE -Wall -Werror)

install(TARGETS rip)
install(DIRECTORY Include/ DESTINATION include)

if (RIP_ENABLE_TESTS)
    add_subdirectory(Tests)
endif()