# RIP

Hardware-accelerated raw image processing library for the 3DS.

## Setup

Download a prebuilt version, use as a CMake dependency, or build manually.

### HOS build (KYGX/libctru/citro3d)

```sh
cmake -B BuildHOS -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/cmake/3DS.cmake" -DCMAKE_BUILD_TYPE=Release -DRIP_BACKEND="{BACKEND_HERE}" -DRIP_ENABLE_TESTS=ON
cmake --build BuildHOS --config Release
cmake --install BuildHOS --prefix BuildHOS/Release
```

where `{BACKEND_HERE}` is one of: `kygx`, `libctru`, `citro3d`.

### Baremetal build (KYGX/libn3ds)

```sh
cmake -B BuildBM -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="$CTR_BM_TOOLCHAIN_ROOT/Toolchain.cmake" -DCMAKE_BUILD_TYPE=Release -DRIP_BACKEND="{BACKEND_HERE}" -DRIP_ENABLE_TESTS=ON
cmake --build BuildBM --config Release
cmake --install BuildBM --prefix BuildBM/Release
```

where `${BACKEND_HERE}` is one of: `kygx`, `libn3ds`.

## License

This library is doubly licensed:

- MPL 2.0, for HOS (userland) usage (see [HOS_LICENSE.txt](HOS_LICENSE.txt)).
- GPLv3, for baremetal usage (see [BM_LICENSE.txt](BM_LICENSE.txt)).