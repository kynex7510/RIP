add_executable(RIP-HOS-VerifySwap Main.c)
target_link_libraries(RIP-HOS-VerifySwap PRIVATE rip)
ctr_create_3dsx(RIP-HOS-VerifySwap)