add_executable(RIP-HOS-BenchmarkPixelSwap Main.c)
target_link_libraries(RIP-HOS-BenchmarkPixelSwap PRIVATE rip)
ctr_create_3dsx(RIP-HOS-BenchmarkPixelSwap)