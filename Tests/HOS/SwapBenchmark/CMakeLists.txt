add_executable(RIP-HOS-SwapBenchmark Main.c)
target_link_libraries(RIP-HOS-SwapBenchmark PRIVATE rip)
ctr_create_3dsx(RIP-HOS-SwapBenchmark)