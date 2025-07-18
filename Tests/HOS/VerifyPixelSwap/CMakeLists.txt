add_executable(RIP-HOS-VerifyPixelSwap Main.c)
target_link_libraries(RIP-HOS-VerifyPixelSwap PRIVATE rip)
ctr_create_3dsx(RIP-HOS-VerifyPixelSwap)