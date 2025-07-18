add_executable(RIP-HOS-VerifyPixelConvert Main.c)
target_link_libraries(RIP-HOS-VerifyPixelConvert PRIVATE rip)
ctr_create_3dsx(RIP-HOS-VerifyPixelConvert)