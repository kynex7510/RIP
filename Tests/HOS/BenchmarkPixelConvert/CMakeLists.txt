add_executable(RIP-HOS-BenchmarkPixelConvert Main.c)
target_link_libraries(RIP-HOS-BenchmarkPixelConvert PRIVATE rip)
ctr_create_3dsx(RIP-HOS-BenchmarkPixelConvert)