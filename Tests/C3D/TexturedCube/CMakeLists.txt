cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
project(RIP-C3D-TexturedCube C)

ctr_add_shader_library(vshader Source/VShader.v.pica)
dkp_add_embedded_binary_library(shaders vshader)

add_executable(RIP-C3D-TexturedCube
    Source/Main.c
    Source/stb_image.c
)
target_link_libraries(RIP-C3D-TexturedCube PRIVATE citro3d rip shaders)

ctr_create_3dsx(RIP-C3D-TexturedCube ROMFS RomFS)