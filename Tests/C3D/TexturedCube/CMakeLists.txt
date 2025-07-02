ctr_add_shader_library(vshader Source/VShader.v.pica)
dkp_add_embedded_binary_library(shaders vshader)

add_executable(RIP-C3D-TexturedCube
    Source/Main.c
    Source/stb_image.c
)
target_link_libraries(RIP-C3D-TexturedCube PRIVATE rip shaders)

ctr_create_3dsx(RIP-C3D-TexturedCube ROMFS RomFS)